#pragma once
#include <string>
#include <locale.h>
#include <sstream>
#include <fstream>
#include <codecvt>
using namespace std;
class Founctions
{
public:
	string UrlEncode(const string& szToEncode);
	string UrlDecode(const string& szToDecode);
	wstring StringToWstring(const string str);
	string WstringToString(const wstring str);
	wstring readFileWstring(const char* filename);
	string readFileString(const char* filename);
	wstring findText(const wstring origin, const wstring find1, const wstring find2);/* 取出中间文本 */
	wstring HttpGetWithoutProp(const char* url, const char* cacheFilePath, const char* cookieFilePath);
	wstring HttpPostWithoutProp(const char* url, const char* postData, const char* cacheFilePath, const char* cookieFilePath);
	Founctions();
	~Founctions();
};

