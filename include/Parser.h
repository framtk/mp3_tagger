//
// Created by francesco simone arrigo on 21.09.17.
//

#ifndef MP3_TAG_FROM_TITLE_PARSER_H
#define MP3_TAG_FROM_TITLE_PARSER_H

#include <vector>
#include <string>


class Parser {
    public:
        std::vector<std::string> splitString(std::string str, char splitChar);
        bool fileExists (const std::string& name);
        bool isDir (const std::string& name);
};


#endif //MP3_TAG_FROM_TITLE_PARSER_H
