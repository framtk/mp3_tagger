//
// Created by francesco simone arrigo on 29.09.17.
//

#ifndef MP3_TAG_FROM_TITLE_TAGGER_H
#define MP3_TAG_FROM_TITLE_TAGGER_H

#include <string>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class Tagger {
    public:
        bool addPicture(std::string mp3_path, std::string song_name, std::string author, std::string image_folder);
        bool tagFile(std::string mp3_path, std::string song_name, std::string author);
        bool removeTags(std::string mp3_path);
        bool apply(po::variables_map vm, std::string filename);
};


#endif //MP3_TAG_FROM_TITLE_TAGGER_H
