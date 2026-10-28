//
// Created by francesco simone arrigo on 29.09.17.
//

#ifndef MP3_TAG_FROM_TITLE_TAGGER_H
#define MP3_TAG_FROM_TITLE_TAGGER_H

#include <string>
#include <boost/program_options.hpp>
#include <filesystem>

#define DIR_SEPARATOR "\\"

namespace po = boost::program_options;
namespace fs = std::filesystem;

class Tagger {
public:
	bool addPicture(const fs::directory_entry& mp3_path_itr, std::wstring song_name, std::wstring author, std::wstring image_folder);
	bool tagFile(const fs::directory_entry& mp3_path_itr, std::wstring song_name, std::wstring author);
	bool removeTags(const fs::directory_entry& mp3_path_itr);
	bool apply(po::variables_map vm, const fs::directory_entry& file_path_entry);
};


#endif //MP3_TAG_FROM_TITLE_TAGGER_H
