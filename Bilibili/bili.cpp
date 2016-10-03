#include "stdafx.h"
#include "bili.h"
using namespace std;

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

int bili::Login(const string username, const string password)
{//登陆 Bilibili
	
	string postData = "userid=" + username + "&pwd=" + f.UrlEncode(password) + "&captcha=&keep=1";
	cookieFile = username + ".bili.cookie";
	cacheFile = username + ".bili.cache";
	uname = username;
	wstring ret = f.HttpPostWithoutProp("https://passport.bilibili.com/ajax/miniLogin/login", postData.c_str(), cacheFile.c_str(), cookieFile.c_str());

	rapidjson::Document document;
	document.Parse(f.WstringToString(ret).c_str());
	
	assert(document.IsObject());
	assert(document.HasMember("status"));
	assert(document["status"].IsBool());
	if (document["status"].GetBool())
	{
		return 0;
	}
	else
	{
		assert(document.HasMember("message"));
		assert(document["message"]["code"].IsInt());
		return document["message"]["code"].GetInt();
	}
	return -1;
}

string bili::CheckLogin()
{
	wstring ret = f.HttpGetWithoutProp("http://live.bilibili.com/User/getUserInfo", cacheFile.c_str(), cookieFile.c_str());
	return f.WstringToString(ret);
}

int bili::GetUserInfoLive(LiveInfo &l)
{
	string ret = CheckLogin();
	rapidjson::Document document;
	document.Parse(ret.c_str());
	assert(document.IsObject());
	assert(document.HasMember("code"));
	assert(document["code"].IsString());
	if (document["code"] != "REPONSE_OK")
		return -1;
	assert(document.HasMember("data"));
	assert(document["data"].HasMember("uname"));			assert(document["data"]["uname"].IsString());
	assert(document["data"].HasMember("face"));				assert(document["data"]["face"].IsString());
	assert(document["data"].HasMember("silver"));			assert(document["data"]["silver"].IsInt());
	assert(document["data"].HasMember("gold"));				assert(document["data"]["gold"].IsInt());
	assert(document["data"].HasMember("achieve"));			assert(document["data"]["achieve"].IsInt());
	assert(document["data"].HasMember("vip"));				assert(document["data"]["vip"].IsInt());
	assert(document["data"].HasMember("svip"));				assert(document["data"]["svip"].IsInt());
	assert(document["data"].HasMember("user_level"));		assert(document["data"]["user_level"].IsInt());
	assert(document["data"].HasMember("user_next_level"));	assert(document["data"]["user_next_level"].IsInt());
	assert(document["data"].HasMember("user_intimacy"));	assert(document["data"]["user_intimacy"].IsInt());
	assert(document["data"].HasMember("user_next_intimacy"));assert(document["data"]["user_next_intimacy"].IsInt());
	assert(document["data"].HasMember("user_level_rank"));	 assert(document["data"]["user_level_rank"].IsString());
	assert(document["data"].HasMember("billCoin"));			 assert(document["data"]["billCoin"].IsInt());
	l.uname		 	  = document["data"]["uname"].GetString();
	l.face			  = document["data"]["face"].GetString();
	l.silver		  = document["data"]["silver"].GetInt();
	l.gold			  = document["data"]["gold"].GetInt();
	l.achieve		  = document["data"]["achieve"].GetInt();
	l.vip			  = document["data"]["vip"].GetInt();
	l.svip			  = document["data"]["svip"].GetInt();
	l.user_level	  = document["data"]["user_level"].GetInt();
	l.user_next_level = document["data"]["user_next_level"].GetInt();
	l.user_intimacy   = document["data"]["user_intimacy"].GetInt();
	l.user_next_intimacy = document["data"]["user_next_intimacy"].GetInt();
	l.user_level_rank = document["data"]["user_level_rank"].GetString();
	l.billCoin		  = document["data"]["billCoin"].GetInt();
	return 0;
}

int bili::GetSignInfo(SignInfo &s)
{
	wstring ret = f.HttpGetWithoutProp("http://live.bilibili.com/sign/GetSignInfo", cacheFile.c_str(), cookieFile.c_str());
	rapidjson::Document document;
	document.Parse(f.WstringToString(ret).c_str());
	assert(document.IsObject());
	assert(document.HasMember("msg"));
	assert(document["msg"].IsString());
	if (strcmp("ok", document["msg"].GetString()))
	{//如果不是 ok
		return -1;
	}
	assert(document["data"].HasMember("text"));			 assert(document["data"]["text"].IsString());
	assert(document["data"].HasMember("status"));		 assert(document["data"]["status"].IsInt());
	assert(document["data"].HasMember("allDays"));		 assert(document["data"]["allDays"].IsString());
	assert(document["data"].HasMember("curMonth"));		 assert(document["data"]["curMonth"].IsString());
	assert(document["data"].HasMember("hadSignDays"));	 assert(document["data"]["hadSignDays"].IsInt());
	assert(document["data"].HasMember("newTask"));		 assert(document["data"]["newTask"].IsInt());
	s.text = document["data"]["text"].GetString();
	s.allDays = document["data"]["allDays"].GetString();
	s.curMonth = document["data"]["curMonth"].GetString();
	s.status = document["data"]["status"].GetInt();
	s.hadSignDays = document["data"]["hadSignDays"].GetInt();
	s.newTask = document["data"]["newTask"].GetInt();
	return 0;
}

int bili::GetLiveRoomInfo(LiveRoomInfo &lr)
{
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
	cout << "Level:" << lr.level << " roomID: " << lr.roomID << " currentExp: " << lr.currentExp << " allExp: " << lr.allExp 
		 << " MasterID " << lr.masterID << endl;
	cout << endl;
	return 0;
}

int bili::GetMasterID(int liveRoomID)
{
	string url = "http://live.bilibili.com/live/getInfo?roomid=" + to_string(liveRoomID);
	wstring ret = f.HttpGetWithoutProp(url.c_str(), cacheFile.c_str(), cookieFile.c_str());
	rapidjson::Document document;
	document.Parse(f.WstringToString(ret).c_str());
	assert(document.IsObject());
	assert(document["data"].HasMember("MASTERID"));
	return document["data"]["MASTERID"].GetInt();
}

bili::bili()
{
}


bili::~bili()
{
}
