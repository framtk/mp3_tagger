//
// Created by francesco simone arrigo on 21.09.17.
//

#pragma once

#include <vector>
#include <string>
#include <locale>
#include <string>

class Parser {
public:
	std::vector<std::wstring> splitString(std::wstring str, wchar_t splitChar);
	bool fileExists(const std::wstring& name);
	bool isDir(const std::string& name);
	std::wstring string_to_wstring(const std::string& s);
	std::string wstring_to_string(const std::wstring& ws);
	void ltrim(std::wstring& s);
	void rtrim(std::wstring& s);
	void trim(std::wstring& s);
};



