//
// Created by francesco simone arrigo on 29.09.17.
//

#include "../include/Tagger.h"
#include "../include/Parser.h"
#include <taglib/tbytevector.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/tstringlist.h>
#include <taglib/attachedpictureframe.h>
#include <fstream>
#include <taglib/urllinkframe.h>

namespace po = boost::program_options;

bool Tagger::addPicture(std::string mp3_path, std::string song_name, std::string author, std::string image_folder){
    try {

        Parser parser;

        TagLib::MPEG::File mp3_file(mp3_path.c_str());
        TagLib::ID3v2::Tag *mp3_tag;
        mp3_tag = mp3_file.ID3v2Tag(true);

        // remove previous pictures to not have the mp3 file grow infinitely
        mp3_tag->removeFrames("APIC");

        std::stringstream stream;
        stream << image_folder << "/" << song_name << ".jpg";

        std::ifstream image;

        if (parser.fileExists(stream.str())) {
            image = std::ifstream(stream.str(), std::ios::binary | std::ios::ate);
            stream.str(std::string());
        } else {
            stream.str(std::string());
            stream << image_folder << author << ".jpg";
            if (parser.fileExists(stream.str())) {
                image = std::ifstream(stream.str(), std::ios::binary | std::ios::ate);
                stream.str(std::string());
            } else {
                std::cerr << "No file named " << song_name << ".jpg" << " or " << author << ".jpg" << " in " << image_folder << "\n";
                return false;
            }
        }

        const auto fileSize = image.tellg();
        image.seekg(0);
        TagLib::ByteVector image_data((unsigned int) fileSize, 0);
        image.read(image_data.data(), fileSize);
        image.close();

        auto *picture = new TagLib::ID3v2::AttachedPictureFrame();
        picture->setMimeType("image/jpeg");
        picture->setDescription("Cover");
        picture->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
        picture->setPicture(image_data);
        mp3_tag->addFrame(picture);
        mp3_file.save(TagLib::MPEG::File::ID3v2);

        return true;

    } catch (std::exception &e){
        return false;
    }
}

bool Tagger::removeTags(std::string mp3_path){

    try {

        TagLib::MPEG::File mp3_file(mp3_path.c_str());
        TagLib::ID3v2::Tag *mp3_tag;
        mp3_tag = mp3_file.ID3v2Tag(true);

        mp3_tag->removeFrames("TCON");
        mp3_tag->removeFrames("TRCK");
        mp3_tag->removeFrames("TDRC");
        mp3_tag->removeFrames("TIT2");
        mp3_tag->removeFrames("TPE1");
        mp3_tag->removeFrames("COMM");
        mp3_tag->removeFrames("APIC");
        mp3_tag->removeFrames("WXXX");

        mp3_file.save(TagLib::MPEG::File::ID3v2);

        return true;

    }catch (std::exception &e){
        return false;
    }
}

bool Tagger::tagFile(std::string mp3_path, std::string song_name, std::string author) {
    try {

        TagLib::MPEG::File mp3_file(mp3_path.c_str());
        TagLib::ID3v2::Tag *mp3_tag;
        mp3_tag = mp3_file.ID3v2Tag(true);

        auto *url = new TagLib::ID3v2::UserUrlLinkFrame();
        url->setUrl("https://github.com/framtk/mp3_tagger");
        url->setText("Git repo");
        mp3_tag->setTitle(song_name);
        mp3_tag->setArtist(author);
        mp3_tag->setComment("Tagged with mp3_tagger");
        mp3_tag->addFrame(url);

        mp3_file.save(TagLib::MPEG::File::ID3v2);

    } catch (std::exception &e){
        return false;
    }
    return true;
}

bool Tagger::apply(po::variables_map vm, std::string filename) {

    try {
        bool verbose = vm.count("verbose") != 0;

        Parser parser;

        std::vector<std::string> path_split = parser.splitString(filename, '/');
        std::string song_name = parser.splitString(path_split[path_split.size() - 1], '.').end()[-2];
        std::string author = parser.splitString(song_name, '-')[0];
        std::string song_title = parser.splitString(song_name, '-')[1];

        parser.trim(song_name);
        parser.trim(author);
        parser.trim(song_title);

        if (vm.count("clear")) {
            if (verbose)
                std::cout << "Clearing " << song_name << " tags...\n";
            if (!removeTags(filename)) {
                std::cerr << "There was an error removing the tags of " << song_name << "\n";
                return false;
            };
        }


        if (vm.count("tag")) {
            if (verbose)
                std::cout << "Tagging " << song_name << "...\n";
            if (!tagFile(filename, song_title, author)) {
                std::cerr << "There was an error tagging " << song_name << "\n";
                return false;
            }
        }

        if (vm.count("picture")) {
            std::string image_dir = vm["picture"].as<std::string>();
            if (verbose)
                std::cout << "Setting " << song_name << " picture...\n";

            if (!addPicture(filename, song_name, author, image_dir)) {
                std::cerr << "There was an error setting picture for " << song_name << "\n";
                return false;
            }
        }
    } catch (std::exception &e){
        return false;
    }
    return true;
}
