#include "stdafx.h"
#include "Founctions.h"
using namespace std;
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

string Founctions::UrlEncode(const string& szToEncode)
{

	std::string src = szToEncode;
	char hex[] = "0123456789ABCDEF";
	string dst;
	for (size_t i = 0; i < src.size(); ++i)
	{
		unsigned char cc = src[i];
		if (isascii(cc))
		{
			if (cc == ' ')
			{
				dst += "%20";

			}
			else
				dst += cc;
		}
		else
		{

			unsigned char c = static_cast<unsigned char>(src[i]);
			dst += '%';
			dst += hex[c / 16];
			dst += hex[c % 16];
		}

	}
	return dst;
}
string Founctions::UrlDecode(const string& szToDecode)
{
	std::string result;
	int hex = 0;
	for (size_t i = 0; i < szToDecode.length(); ++i)
	{
		switch (szToDecode[i])
		{
		case '+':
			result += ' ';
			break;
		case '%':
			if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))
			{
				std::string hexStr = szToDecode.substr(i + 1, 2);
				hex = strtol(hexStr.c_str(), 0, 16);
				//字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@] 

				//可以不经过编码直接用于URL  
				if (!((hex >= 48 && hex <= 57) || //0-9  
					(hex >= 97 && hex <= 122) ||   //a-z  
					(hex >= 65 && hex <= 90) ||    //A-Z  
												   //一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]  
					hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29
					|| hex == 0x2a || hex == 0x2b || hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f
					|| hex == 0x3A || hex == 0x3B || hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f
					))
				{
					result += char(hex);
					i += 2;
				}
				else result += '%';
			}
			else {
				result += '%';
			}
			break;
		default:
			result += szToDecode[i];
			break;
		}

	}
	return result;
}
// 需包含locale、string头文件、使用setlocale函数。
/*http://www.cnblogs.com/SunboyL/archive/2013/03/31/stringandwstring.html*/
wstring Founctions::StringToWstring(const string str)
{// string转wstring
	unsigned len = str.size() * 2;// 预留字节数
	setlocale(LC_CTYPE, "");     //必须调用此函数
	wchar_t *p = new wchar_t[len];// 申请一段内存存放转换后的字符串
	mbstowcs(p, str.c_str(), len);// 转换
	std::wstring str1(p);
	delete[] p;// 释放申请的内存
	return str1;
}
string Founctions::WstringToString(const wstring str)
{// wstring转string
	unsigned len = str.size() * 4;
	setlocale(LC_CTYPE, "");
	char *p = new char[len];
	wcstombs(p, str.c_str(), len);
	std::string str1(p);
	delete[] p;
	return str1;
}
wstring Founctions::readFileWstring(const char* filename)
{
	std::wifstream wif(filename);
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstringstream wss;
	wss << wif.rdbuf();
	return wss.str();
}
string Founctions::readFileString(const char* filename)
{
	ifstream text(filename);
	std::string str((std::istreambuf_iterator<char>(text)), std::istreambuf_iterator<char>());
	return str;
}
wstring Founctions::findText(const wstring origin, const wstring find1, const wstring find2)
{
	size_t found1, found2;
	found1 = origin.find(find1);
	if ((int)found1 == 0)
		return L"";
	found2 = origin.find(find2, found1);
	if ((int)found2 == 0)
		return L"";
	return origin.substr(found1 + find1.length(), found2 - found1 - find1.length());
}
wstring Founctions::HttpGetWithoutProp(const char* url, const char* cacheFilePath, const char* cookieFilePath)
{
	CURL *curl;
	CURLcode res;
	FILE *pagefile = fopen(cacheFilePath, "wb");
	if (!pagefile)
	{
		return L"";
	}
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookieFilePath);/* 设置发送的 Cookies */
		curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookieFilePath);/* 设置接受的 Cookies */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //对返回的数据进行操作的函数地址 
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);/* write the page body to this file handle */
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		curl_easy_cleanup(curl);
	}
	fclose(pagefile);
	curl_global_cleanup();
	return readFileWstring(cacheFilePath);
}

wstring Founctions::HttpPostWithoutProp(const char* url, const char* postData, const char* cacheFilePath, const char* cookieFilePath)
{
	CURL *curl;
	CURLcode res;
	FILE *pagefile = fopen(cacheFilePath, "wb");
	if (!pagefile)
	{
		return L"";
	}
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookieFilePath);/* 设置发送的 Cookies */
		curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookieFilePath);/* 设置接受的 Cookies */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //对返回的数据进行操作的函数地址 
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);/* write the page body to this file handle */
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	fclose(pagefile);
	curl_global_cleanup();
	return readFileWstring(cacheFilePath);
}
Founctions::Founctions()
{
}
Founctions::~Founctions()
{
}
