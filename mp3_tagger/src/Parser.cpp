//
// Created by francesco simone arrigo on 21.09.17.
//

#include "../include/Parser.h"
#include <windows.h>
#include <cstring>
#include <algorithm>
#include <cwctype>
#include <sstream>

std::vector<std::wstring> Parser::splitString(std::wstring str, wchar_t splitChar) {
	std::wstringstream temp(str);
	std::wstring segment;
	std::vector<std::wstring> seglist;

	while (std::getline(temp, segment, splitChar)) {
		if (!segment.empty())
			seglist.push_back(segment);
	}
	return seglist;
}

bool Parser::fileExists(const std::wstring& name) {
	return (_waccess(name.c_str(), 0) == 0);
}

bool Parser::isDir(const std::string& name) {
	struct stat buffer {};
	if (stat(name.c_str(), &buffer) == 0) {
		if (buffer.st_mode & S_IFDIR) {
			return true;
		}
	}
	return false;
}

std::wstring Parser::string_to_wstring(const std::string& s) {
	if (s.empty()) return L"";
	int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), NULL, 0);
	if (size == 0) return L"";
	std::wstring out(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), &out[0], size);
	return out;
}

std::string Parser::wstring_to_string(const std::wstring& ws) {
	if (ws.empty()) return "";
	int size = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), (int)ws.size(), NULL, 0, NULL, NULL);
	if (size == 0) return "";
	std::string out(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), (int)ws.size(), &out[0], size, NULL, NULL);
	return out;
}

void Parser::ltrim(std::wstring& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](wint_t ch) {
		return !std::iswspace(ch);
		}));
}

void Parser::rtrim(std::wstring& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](wint_t ch) {
		return !std::iswspace(ch);
		}).base(), s.end());
}

void Parser::trim(std::wstring& s) {
	ltrim(s);
	rtrim(s);
}
