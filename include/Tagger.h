//
// Created by francesco simone arrigo on 29.09.17.
//

#ifndef MP3_TAG_FROM_TITLE_TAGGER_H
#define MP3_TAG_FROM_TITLE_TAGGER_H

class Tagger {
    public:
        bool tagFile(std::string mp3_path, std::string song_name, std::string image_folder = "");
};


#endif //MP3_TAG_FROM_TITLE_TAGGER_H
