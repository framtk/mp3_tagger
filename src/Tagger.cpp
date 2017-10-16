//
// Created by francesco simone arrigo on 29.09.17.
//

#include "../include/Tagger.h"
#include "../include/Parser.h"
#include <boost/program_options.hpp>
#include <taglib/tbytevector.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/tstringlist.h>
#include <taglib/attachedpictureframe.h>
#include <fstream>

bool Tagger::addPicture(std::string mp3_path, std::string song_name, std::string author, std::string image_folder){
    try {

        Parser parser;

        TagLib::MPEG::File mp3_file(mp3_path.c_str());
        TagLib::ID3v2::Tag *mp3_tag;
        mp3_tag = mp3_file.ID3v2Tag(true);

        std::cout << mp3_tag->title() << "\n";

        auto *mp3_frames = new TagLib::ID3v2::FrameList(mp3_tag->frameList());

        // remove previous picture to not have the mp3 file grow infinitely
        for (auto &frame : *mp3_frames) {
            if (frame->frameID() == "APIC"){
                mp3_tag->removeFrame(frame, true);
                break;
            }
        }

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
        mp3_file.save();

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

        auto *mp3_frames = new TagLib::ID3v2::FrameList(mp3_tag->frameList());

        // remove previous picture to not have the mp3 file grow infinitely
        for (auto &frame : *mp3_frames) {
            if (frame->frameID() == "APIC"){
                mp3_tag->removeFrame(frame, true);
                break;
            }
        }

        mp3_tag->setTitle(TagLib::String::null);
        mp3_tag->setAlbum(TagLib::String::null);
        mp3_tag->setArtist(TagLib::String::null);
        mp3_tag->setComment(TagLib::String::null);
        mp3_tag->setGenre(TagLib::String::null);
        mp3_tag->setTrack(0);
        mp3_tag->setYear(0);

        mp3_file.save();

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

        mp3_tag->setTitle(song_name);
        mp3_tag->setArtist(author);
        mp3_tag->setComment("Tagged with mp3_tagger");

        mp3_file.save();

    } catch (std::exception &e){
        return false;
    }
    return false;
}
