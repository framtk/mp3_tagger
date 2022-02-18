//
// Created by francesco simone arrigo on 21.09.17.
//

#include "../include/Parser.h"
#include <sstream>
#include <sys/stat.h>

std::vector<std::wstring> Parser::splitString(std::wstring str, wchar_t splitChar) {
    std::wstringstream temp(str);
    std::wstring segment;
    std::vector<std::wstring> seglist;

    while(std::getline(temp, segment, splitChar)) {
        if (!segment.empty())
            seglist.push_back(segment);
    }
    return seglist;
}

bool Parser::fileExists (const std::wstring& name) {
    struct _stat64i32 buffer{};
    return (_wstat(name.c_str(), &buffer) == 0);
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

void Parser::ltrim(std::wstring &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void Parser::rtrim(std::wstring &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void Parser::trim(std::wstring &s) {
    ltrim(s);
    rtrim(s);
}