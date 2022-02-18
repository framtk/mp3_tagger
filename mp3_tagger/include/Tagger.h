//
// Created by francesco simone arrigo on 29.09.17.
//

#ifndef MP3_TAG_FROM_TITLE_TAGGER_H
#define MP3_TAG_FROM_TITLE_TAGGER_H

#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#ifdef _WIN32
#define DIR_SEPARATOR "\\"
#else
#define DIR_SEPARATOR "/"
#endif

namespace po = boost::program_options;
namespace fs = boost::filesystem;

class Tagger {
    public:
        bool addPicture(fs::directory_iterator mp3_path_itr, std::wstring song_name, std::wstring author, std::wstring image_folder);
        bool tagFile(fs::directory_iterator mp3_path_itr, std::wstring song_name, std::wstring author);
        bool removeTags(fs::directory_iterator mp3_path_itr);
        bool apply(po::variables_map vm, fs::directory_iterator file_path_itr);
};


#endif //MP3_TAG_FROM_TITLE_TAGGER_H
