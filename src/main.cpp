#include <iostream>

#include "../include/Parser.h"
#include <taglib/tbytevector.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>
#include <boost/program_options.hpp>
#include <fstream>

namespace po = boost::program_options;

int main(int argc, const char *argv[]) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "Produce help message")
                ("clear,c", "Clears the tags before setting the title and author")
                ("file,f", po::value<std::string>(), "Select and apply on a single .mp3 file")
                ("dir,d", po::value<std::string>(), "Select and apply on all .mp3 files in a directory")
                ("rename,r", "The file will be renamed with only the song title");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 1;
        }

        if (!(vm.count("file") || vm.count("dir"))) {
            std::cerr << "Please select a file or a directory \nUse --help to show the available arguments\n";
            return 1;
        }

        Parser parser;

        if (vm.count("file")) {
            std::string filename = vm["file"].as<std::string>();

            if (!parser.fileExists(filename)){
                std::cerr << "The chosen file doesn't exist!\n";
                return 2;
            }

            if (parser.isDir(filename)){
                // is directory, do fix
            }

            std::vector<std::string> path_split = parser.splitString(filename, '/');

            TagLib::MPEG::File mp3_file(filename.c_str());
            TagLib::ID3v2::Tag *mp3_tag;

            mp3_tag = mp3_file.ID3v2Tag(true);

            std::ifstream image("/Users/arrigf/Desktop/Music/maxresdefault.jpg", std::ios::binary | std::ios::ate);
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
        }

        return 0;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << ", application will now exit" << std::endl;
        return 2;
    }
}