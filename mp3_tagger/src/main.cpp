#include <iostream>

#include "../include/Parser.h"
#include "../include/Tagger.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, const char* argv[]) {
	try {
		po::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "Produce help message")
			("tag,t", "Tags the selected file/all the files in the folder")
			("clear,c", "Clears the tags, if -t is used the tags will be cleared before being set")
			("file,f", po::value<std::string>(), "Select and apply on a single .mp3 file")
			("dir,d", po::value<std::string>(), "Select and apply on all .mp3 files in a directory")
			("picture,p", po::value<std::string>(), "The folder containing the images to apply to the songs")
			("verbose,v", "Sets verbose output");

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
			std::cout << desc << "\n";
			return 0;
		}

		if (!(vm.count("file") || vm.count("dir"))) {
			std::cerr << "Please select a file or a directory \nUse --help or -h to show the available arguments\n";
			return 1;
		}

		Parser parser;
		Tagger tagger;

		if (vm.count("file")) {
			std::wstring filename = parser.converter.from_bytes(vm["file"].as<std::string>());

			if (!parser.fileExists(filename)) {
				std::cerr << "The chosen file doesn't exist!\n";
				return 2;
			}

			fs::path file_path = filename;
			fs::directory_iterator itr(filename);
			if (!tagger.apply(vm, itr)) {
				std::wcerr << "There was an error tagging the file " << filename << "\n";
				return 2;
			}

		}
		else if (vm.count("dir")) {
			std::string dirname = vm["dir"].as<std::string>();
			if (!parser.isDir(dirname)) {
				std::cerr << "The chosen path isn't a directory or the directory doesn't exist!\n";
				return 2;
			}

			int total = 0;
			int count = 0;
			std::wstring current_filename;

			fs::path dirpath = dirname;
			fs::directory_iterator end_it;
			for (fs::directory_iterator itr(dirpath); itr != end_it; ++itr) {
				current_filename = itr->path().filename().wstring();
				if (parser.splitString(current_filename, L'.').back() == L"mp3")
					total++;
			}

			// This fixed wcout breaking for some characters when printing
			std::ios_base::sync_with_stdio(false);

			std::locale utf8(std::locale(), new std::codecvt_utf8_utf16<wchar_t>);
			std::wcout.imbue(utf8);
			std::wcerr.imbue(utf8);

			for (fs::directory_iterator itr(dirpath); itr != end_it; ++itr) {
				current_filename = itr->path().filename().wstring();
				if (parser.splitString(current_filename, L'.').back() == L"mp3") {
					if (!tagger.apply(vm, itr)) {
						std::wcerr << "There was an error tagging the file " << current_filename << "\n";
					}
					count++;

					std::wcout << "Done " << count << " of " << total << " - " << current_filename << "\n";
				}
			}
		}

		std::cout << "Tagging complete, enjoy your music!\n";

		return 0;
	}
	catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << ", application will now exit" << std::endl;
		return 2;
	}

}
