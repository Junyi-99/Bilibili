#include "stdafx.h"
#include "bili.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace std;
using namespace rapidjson;
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

int bili::Login(const string username, const string password)
{//登陆 Bilibili
	username_ = username;
	string postData = "userid=" + username + "&pwd=" + f.UrlEncode(password) + "&captcha=&keep=1";
	cookieFile = username + ".bili.cookie";
	cacheFile = username + ".bili.cache";
	wstring ret = f.HttpPostWithoutProp("https://passport.bilibili.com/ajax/miniLogin/login", postData.c_str(), cacheFile.c_str(), cookieFile.c_str());

	rapidjson::Document doc;
	doc.Parse(f.WstringToString(ret).c_str());

	if (!doc.IsObject())
		return -1;
	if (!doc.HasMember("status"))
		return -2;
	if (!doc["status"].IsBool())
		return -3;
	if (doc["status"].GetBool())
	{
		isLogin_ = true;
		return 0;
	}
	else
	{
		if (!doc.HasMember("message"))
			return -4;
		if (!doc["message"]["code"].IsInt())
			return -5;
		return doc["message"]["code"].GetInt();
	}
	return -99;
}

string bili::CheckLogin()
{//检查登陆
	wstring ret = f.HttpGetWithoutProp("http://live.bilibili.com/User/getUserInfo", cacheFile.c_str(), cookieFile.c_str());
	return f.WstringToString(ret);
}

int bili::GetUserInfoLive(LiveInfo &l)
{//获取Bilibili直播站用户信息
	string ret = CheckLogin();
	rapidjson::Document doc;
	doc.Parse(ret.c_str());
	if (!doc.IsObject())
		return -1;
	if (doc.HasMember("code") && doc["code"].IsString())
	{
		if (strcmp(doc["code"].GetString(), "REPONSE_OK"))
			return -2;
	}
	if (!doc.HasMember("data"))
		return -3;

	if (doc["data"].HasMember("uname") && doc["data"]["uname"].IsString())
		l.uname = doc["data"]["uname"].GetString();

	if (doc["data"].HasMember("face") && doc["data"]["face"].IsString())
		l.face = doc["data"]["face"].GetString();
	
	if (doc["data"].HasMember("silver") && doc["data"]["silver"].IsInt())
		l.silver = doc["data"]["silver"].GetInt();
	
	if (doc["data"].HasMember("gold") && doc["data"]["gold"].IsInt())
		l.gold = doc["data"]["gold"].GetInt();
	
	if (doc["data"].HasMember("achieve") && doc["data"]["achieve"].IsInt())
		l.achieve = doc["data"]["achieve"].GetInt();
	
	if (doc["data"].HasMember("vip") && doc["data"]["vip"].IsInt())
		l.vip = doc["data"]["vip"].GetInt();
	
	if (doc["data"].HasMember("svip") && doc["data"]["svip"].IsInt())
		l.svip = doc["data"]["svip"].GetInt();
	
	if (doc["data"].HasMember("user_level") && doc["data"]["user_level"].IsInt())
		l.user_level = doc["data"]["user_level"].GetInt();
	
	if (doc["data"].HasMember("user_next_level") && doc["data"]["user_next_level"].IsInt())
		l.user_next_level = doc["data"]["user_next_level"].GetInt();
	
	if (doc["data"].HasMember("user_intimacy") && doc["data"]["user_intimacy"].IsInt())
		l.user_intimacy = doc["data"]["user_intimacy"].GetInt();
	
	if (doc["data"].HasMember("user_next_intimacy") && doc["data"]["user_next_intimacy"].IsInt())
		l.user_next_intimacy = doc["data"]["user_next_intimacy"].GetInt();
	
	if (doc["data"].HasMember("user_level_rank") && doc["data"]["user_level_rank"].IsString())
		l.user_level_rank = doc["data"]["user_level_rank"].GetString();
	
	if (doc["data"].HasMember("billCoin") && doc["data"]["billCoin"].IsInt())
		l.billCoin = doc["data"]["billCoin"].GetInt();
	return 0;
}
int bili::GetUserInfoAV(AVInfo &a)
{//获取Bilibili直播站用户信息
	wstring ret = f.HttpGetWithoutProp("https://account.bilibili.com/home/userInfo", cacheFile.c_str(), cookieFile.c_str());
	rapidjson::Document doc;
	doc.Parse(f.WstringToString(ret).c_str());
	if (!doc.IsObject())
		return -1;
	if (doc.HasMember("code") && doc["code"].IsInt())
	{
		if (doc["code"].GetInt() != 0)
			return -2;
	}
	if (!doc.HasMember("data"))
		return -3;
	//我就偷一下懒（就一下下），不判断元素是否存在啦~
	a.current_level = doc["data"]["level_info"]["current_level"].GetInt();
	a.current_min = doc["data"]["level_info"]["current_min"].GetInt();
	a.current_exp = doc["data"]["level_info"]["current_exp"].GetInt();
	a.next_exp = doc["data"]["level_info"]["next_exp"].GetInt();
	a.bCoins = doc["data"]["bCoins"].GetInt();
	a.coins = doc["data"]["coins"].GetDouble();
	a.face = doc["data"]["face"].GetString();
	a.sex = doc["data"]["sex"].GetString();
	a.nameplate_current = doc["data"]["nameplate_current"].GetString();
	a.uname = doc["data"]["uname"].GetString();
	a.official_verify = doc["data"]["official_verify"].GetInt();
	
	unsigned char *gbk = new unsigned char[200];
	unsigned char *utf8 = new unsigned char[200];
	strcpy((char*)utf8, a.uname.c_str());
	
	f.UTF8ToGBK(utf8, gbk, 200);
	a.uname = (char*)gbk;
	uname = a.uname;
	delete[] utf8;
	delete[] gbk;
	
	return 0;
}

int bili::GetSignInfo(SignInfo &s)
{//获取签到信息
	wstring ret = f.HttpGetWithoutProp("http://live.bilibili.com/sign/GetSignInfo", cacheFile.c_str(), cookieFile.c_str());
	rapidjson::Document doc;
	doc.Parse(f.WstringToString(ret).c_str());
	if (!doc.IsObject())
		return -1;
	if (!doc.HasMember("msg"))
		return -2;
	if (strcmp("ok", doc["msg"].GetString()) && doc["msg"].IsString())
	{//如果不是 ok
		return -3;
	}
	if (doc["data"].HasMember("text") && doc["data"]["text"].IsString())
		s.text = doc["data"]["text"].GetString();
	
	if (doc["data"].HasMember("status") && doc["data"]["status"].IsInt())
		s.status = doc["data"]["status"].GetInt();
	
	if (doc["data"].HasMember("allDays") && doc["data"]["allDays"].IsString())
		s.allDays = doc["data"]["allDays"].GetString();
	
	if (doc["data"].HasMember("curMonth") && doc["data"]["curMonth"].IsString())
		s.curMonth = doc["data"]["curMonth"].GetString();
	
	if (doc["data"].HasMember("hadSignDays") && doc["data"]["hadSignDays"].IsInt())
		s.hadSignDays = doc["data"]["hadSignDays"].GetInt();
	
	if (doc["data"].HasMember("newTask") && doc["data"]["newTask"].IsInt())
		s.newTask = doc["data"]["newTask"].GetInt();
	return 0;
}

int bili::GetLiveRoomInfo(LiveRoomInfo &lr)
{//获取直播间信息
	wstring ret = f.HttpGetWithoutProp("http://live.bilibili.com/i/room", cacheFile.c_str(), cookieFile.c_str());
	wstring temp = f.findText(ret, L">UP.", L"<");
	lr.level = atoi(f.WstringToString(temp).c_str());
	temp = f.findText(ret, L"直播间ID：", L"/a></p>");
	temp = f.findText(temp, L">", L"<");
	lr.roomID = atoi(f.WstringToString(temp).c_str());
	temp = f.findText(ret, L"anchor-exp-progress-bar", L"/span>");
	temp = f.findText(temp, L"<span>", L"<");
	lr.currentExp = atoi(f.WstringToString(  temp.substr(0, temp.find(L"/"))   ).c_str());
	lr.allExp     = atoi(f.WstringToString(  temp.substr(temp.find(L"/")+1, temp.length()-temp.find(L"/")-2)   ).c_str());
	lr.masterID = GetMasterID(lr.roomID);
	return 0;
}

int bili::GetMasterID(int liveRoomID)
{//获取播主ID（亦作 RUID）
	string url = "http://live.bilibili.com/live/getInfo?roomid=" + to_string(liveRoomID);
	wstring ret = f.HttpGetWithoutProp(url.c_str(), cacheFile.c_str(), cookieFile.c_str());
	rapidjson::Document doc;
	doc.Parse(f.WstringToString(ret).c_str());
	if (!doc.IsObject())
		return -1;
	if(doc["data"].HasMember("MASTERID"))
		return doc["data"]["MASTERID"].GetInt();
	return -2;
}

int bili::ExchangeSilver2Coin()
{//银瓜子换硬币
	wstring ret = f.HttpPostWithoutProp("http://live.bilibili.com/exchange/silver2coin", "", cacheFile.c_str(), cookieFile.c_str());
	rapidjson::Document doc;
	doc.Parse(f.WstringToString(ret).c_str());
	if (!doc.IsObject())
		return -1;
	if (doc.HasMember("code") && doc["code"].IsInt())
		return doc["code"].GetInt();
	return -2;
}

int bili::FreshUserInfo()
{
	int ret[4];
	ret[0] = GetUserInfoAV(avInfo) == 0 ? 0 : 1;
	ret[1] = GetUserInfoLive(liveInfo) == 0 ? 0 : 1;
	ret[2] = GetLiveRoomInfo(liveRoomInfo) == 0 ? 0 : 1;
	ret[3] = GetSignInfo(signInfo) == 0 ? 0 : 1;
	//printf("%d%d%d%d\n", ret[0], ret[1], ret[2], ret[3]);
	return 2 * 2 * 2 * ret[0] + 2 * 2 * ret[1] + 2 * ret[2] + ret[3];//可表示4种情况
}

int bili::GetUserID()
{
	wstring ret = f.HttpGetWithoutProp("http://space.bilibili.com/", cacheFile.c_str(), cookieFile.c_str());
	ret = f.findText(ret, L"bili_space_mid = ", L";");
	return atoi(f.WstringToString(ret).c_str());
}

wstring bili::GetUserNameByUID(int UID)
{
	string url("http://space.bilibili.com/");
	url.append(to_string(UID));
	wstring ret = f.HttpGetWithoutProp(url.c_str(), cacheFile.c_str(), cookieFile.c_str());
	return f.findText(ret, L"<title>", L"的个人空间 - 哔哩哔哩");
}


bili::~bili()
{
}
