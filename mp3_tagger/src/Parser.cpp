//
// Created by francesco simone arrigo on 21.09.17.
//

#include "../include/Parser.h"
#include <sstream>
#include <cstring>
#include <algorithm>

#ifdef __linux__
#include <unistd.h>
#include <sys/stat.h>
#endif

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
#ifdef WIN32
    return (_waccess(name.c_str(), 0) == 0);
#else
    // Max bytes plusterminator
    size_t outSize = name.size() * sizeof(wchar_t) + 1;

    char* conv = new char[outSize];
    memset(conv, 0, outSize);

    char* oldLocale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "en_US.UTF-8");

    size_t wcsSize = wcstombs(conv, name.c_str(), outSize);

    setlocale(LC_ALL, oldLocale);

    bool file_exists = false;

    if (access(conv, F_OK) == 0){
        file_exists = true;
    }

    delete[] conv;

    return file_exists;
#endif
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
