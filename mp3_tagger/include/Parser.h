//
// Created by francesco simone arrigo on 21.09.17.
//

#pragma once

#include <vector>
#include <string>

class Parser {
public:
	std::vector<std::wstring> splitString(std::wstring str, wchar_t splitChar) const;
	bool fileExists(const std::wstring& name) const;
	bool isDir(const std::string& name) const;
	std::wstring string_to_wstring(const std::string& s) const;
	std::string wstring_to_string(const std::wstring& ws) const;
	void ltrim(std::wstring& s);
	void rtrim(std::wstring& s);
	void trim(std::wstring& s);
};



