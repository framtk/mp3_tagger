//
// Created by francesco simone arrigo on 21.09.17.
//

#ifndef MP3_TAG_FROM_TITLE_PARSER_H
#define MP3_TAG_FROM_TITLE_PARSER_H

#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <string>

class Parser {
    public:
        std::vector<std::wstring> splitString(std::wstring str, wchar_t splitChar);
        bool fileExists (const std::wstring& name);
        bool isDir (const std::string& name);
        void ltrim (std::wstring &s);
        void rtrim (std::wstring &s);
        void trim (std::wstring &s);

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
};


#endif //MP3_TAG_FROM_TITLE_PARSER_H
