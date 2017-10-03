#include <iostream>

#include "../include/Parser.h"
#include "../include/Tagger.h"
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
                ("picture,p", po::value<std::string>(), "The folder containing the images to apply to the songs");

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
            std::string song_name = parser.splitString(path_split[path_split.size() - 1],'.')[0];
            std::string author = parser.splitString(song_name,'-')[0];

            parser.trim(song_name);
            parser.trim(author);

            std::string image_dir = vm["picture"].as<std::string>();

            Tagger tagger;

            if (!tagger.addPicture(filename, song_name, author, image_dir)){
                std::cerr << "There was an error setting picture for " << song_name << "\n";
                return 1;
            }

        }

        return 0;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << ", application will now exit" << std::endl;
        return 2;
    }
}