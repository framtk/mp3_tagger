//
// Created by francesco simone arrigo on 21.09.17.
//

#include "../include/Parser.h"
#include <sstream>
#include <sys/stat.h>

std::vector<std::string> Parser::splitString(std::string str, char splitChar) {
    std::stringstream temp(str);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(temp, segment, splitChar)) {
        if (!segment.empty())
            seglist.push_back(segment);
    }
    return seglist;
}

bool Parser::fileExists (const std::string& name) {
    struct stat buffer{};
    return (stat (name.c_str(), &buffer) == 0);
}

bool Parser::isDir (const std::string& name) {
    struct stat buffer{};
    if (stat (name.c_str(), &buffer) == 0){
        if (buffer.st_mode & S_IFDIR){
            return true;
        }
    }
    return false;
}