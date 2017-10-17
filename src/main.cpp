#include <iostream>

#include "../include/Parser.h"
#include "../include/Tagger.h"
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, const char *argv[]) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "Produce help message")
                ("tag,t", "Tags the selected file/all the files in the folder")
                ("clear,c", "Clears the tags, if -t is used the tags will be cleared before being set")
                ("file,f", po::value<std::string>(), "Select and apply on a single .mp3 file")
                ("dir,d", po::value<std::string>(), "Select and apply on all .mp3 files in a directory")
                ("picture,p", po::value<std::string>(), "The folder containing the images to apply to the songs");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        if (!(vm.count("file") || vm.count("dir"))) {
            std::cerr << "Please select a file or a directory \nUse --help to show the available arguments\n";
            return 1;
        }

        Parser parser;

        if (vm.count("file")) {
            std::string filename = vm["file"].as<std::string>();

            if (!parser.fileExists(filename)) {
                std::cerr << "The chosen file doesn't exist!\n";
                return 2;
            }

            // TODO: DUPLICATE CODE, FIND A WAY TO REMOVE

            std::vector<std::string> path_split = parser.splitString(filename, '/');
            std::string song_name = parser.splitString(path_split[path_split.size() - 1], '.')[0];
            std::string author = parser.splitString(song_name, '-')[0];
            std::string song_title = parser.splitString(song_name, '-')[1];

            parser.trim(song_name);
            parser.trim(author);
            parser.trim(song_title);

            Tagger tagger;

            if (vm.count("clear")) {
                std::cout << "Clearing " << song_name << " tags...\n";

                if (!tagger.removeTags(filename)) {
                    std::cerr << "There was an error removing the tags of " << song_name << "\n";
                    return 1;
                };
            }


            if (vm.count("tag")) {
                std::cout << "Tagging " << song_name << "...\n";
                tagger.tagFile(filename, song_title, author);
            }

            if (vm.count("picture")) {
                std::string image_dir = vm["picture"].as<std::string>();
                std::cout << "Setting " << song_name << " picture...\n";

                if (!tagger.addPicture(filename, song_name, author, image_dir)) {
                    std::cerr << "There was an error setting picture for " << song_name << "\n";
                    return 1;
                }
            }
            // TODO: DUPLICATE CODE, FIND A WAY TO REMOVE

        } else if (vm.count("dir")) {
            std::string dirname = vm["dir"].as<std::string>();
            if (!parser.isDir(dirname)) {
                std::cerr << "The chosen path isn't a directory or the directory doesn't exist!\n";
                return 2;
            }

            fs::path dirpath = dirname;
            fs::directory_iterator end_it;
            for (fs::directory_iterator itr(dirpath); itr != end_it; ++itr)
            {
                std::string current_filename = itr->path().string();
                if (parser.splitString(current_filename, '.')[1] == "mp3") {

                    // TODO: DUPLICATE CODE, FIND A WAY TO REMOVE

                    std::vector<std::string> current_path_split = parser.splitString(current_filename, '/');
                    std::string current_song_name = parser.splitString(current_path_split[current_path_split.size() - 1], '.')[0];
                    std::string current_author = parser.splitString(current_song_name, '-')[0];
                    std::string current_song_title = parser.splitString(current_song_name, '-')[1];

                    parser.trim(current_song_name);
                    parser.trim(current_author);
                    parser.trim(current_song_title);

                    Tagger tagger;

                    if (vm.count("clear")) {
                        std::cout << "Clearing " << current_song_name << " tags...\n";

                        if (!tagger.removeTags(current_filename)) {
                            std::cerr << "There was an error removing the tags of " << current_song_name << "\n";
                        };
                    }


                    if (vm.count("tag")) {
                        std::cout << "Tagging " << current_song_name << "...\n";
                        tagger.tagFile(current_filename, current_song_title, current_author);
                    }

                    if (vm.count("picture")) {
                        std::string image_dir = vm["picture"].as<std::string>();
                        std::cout << "Setting " << current_song_name << " picture...\n";

                        if (!tagger.addPicture(current_filename, current_song_name, current_author, image_dir)) {
                            std::cerr << "There was an error setting picture for " << current_song_name << "\n";
                        }
                    }

                    // TODO: DUPLICATE CODE, FIND A WAY TO REMOVE
                }
            }
        }

        std::cout << "Tagging complete, enjoy your music!\n";

        return 0;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << ", application will now exit" << std::endl;
        return 2;
    }

}