#pragma once
#include "stdafx.h"
#include <string>
#include <locale.h>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <ctime>
#include <vector>
#include "CaptchaRecognition.h"
using namespace std;
class Founctions : public CaptchaRecognition
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
	std::vector<std::string> split(std::string str, std::string pattern);
	time_t GetTimeStamp();
	int UTF8ToGBK(unsigned char * lpUTF8Str, unsigned char * lpGBKStr, int nGBKStrLen);
	int GBKToUTF8(unsigned char * lpGBKStr, unsigned char * lpUTF8Str, int nUTF8StrLen);
	Founctions();
	~Founctions();
};

