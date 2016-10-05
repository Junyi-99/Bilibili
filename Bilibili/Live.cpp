#include "stdafx.h"
#include "Live.h"
int Live::DoSign()
{
	wstring ret = f.HttpGetWithoutProp("http://live.bilibili.com/sign/doSign", cacheFile.c_str(), cookieFile.c_str());
	wcout << ret << endl;
	rapidjson::Document doc;
	doc.Parse(f.WstringToString(ret).c_str());
	if (!doc.IsObject())
		return -1;
	if (!doc.HasMember("code"))
		return -2;
	if (doc["code"].GetInt() == 0)
		return 0;
	return doc["code"].GetInt();
}
int Live::FreeSilverHeart()
{//银瓜子心跳
	wstring ret = f.HttpGetWithoutProp("http://live-feed.bilibili.com/freeSilver/heart", cacheFile.c_str(), cookieFile.c_str());
	return 0;
}


int Live::FreeSilverGetCurrentTask(bili::FreeSilverTask &ct)
{
	string url("http://live.bilibili.com/FreeSilver/getCurrentTask?_=");
	url.append(to_string(f.GetTimeStamp()));
	cout << url << endl;
	wstring ret = f.HttpGetWithoutProp(url.c_str(), cacheFile.c_str(), cookieFile.c_str());
	wcout << ret << endl;
	/*  {"code":0,"msg":"","data":{"minute":6,"silver":80,"time_start":1471700319,"time_end":1471700679}} */
	rapidjson::Document doc;
	doc.Parse(f.WstringToString(ret).c_str());
	
	if (!doc.IsObject())
		return -1;
	if (!doc.HasMember("code"))
		return -2;
	ct.minute = doc["data"]["minute"].GetInt();
	ct.silver = doc["data"]["silver"].GetInt();
	ct.time_start = doc["data"]["time_start"].GetInt();
	ct.time_end = doc["data"]["time_end"].GetInt();
	return doc["code"].GetInt();
}

int Live::Heart()
{//-403 非法心跳
	wstring ret = f.HttpGetWithoutProp("http://live.bilibili.com/User/userOnlineHeart", cacheFile.c_str(), cookieFile.c_str());
	/* {"code":-403,"msg":"\u975e\u6cd5\u5fc3\u8df3","data":[1475575312,1475575146]} */
	rapidjson::Document doc;
	doc.Parse(f.WstringToString(ret).c_str());
	if (!doc.IsObject())
		return -1;
	if (!doc.HasMember("code"))
		return -2;
	return doc["code"].GetInt();;
}

string Live::GetToken()
{
	string text = f.readFileString(cookieFile.c_str());
	text = f.WstringToString(f.findText(f.StringToWstring(text), L"LIVE_LOGIN_DATA", L".bilibil"));
	while (text.find("\n") != string::npos)
	{
		text.erase(text.find("\n"), 1);
	}
	return text.substr(1, text.length()-1);//迷之不能删除空格
}
int Live::SendGift(int giftID, int roomID, int num, bool coinType, int bagID)
{//发送礼物 
//礼物ID GiftBag类型中的 gift_id 成员
//房间ID
//赠送数量
//false：银瓜子 true假：金瓜子
//GiftBag类型中的 id 成员
	string postData("giftId=");
	postData.append(to_string(giftID));
	postData.append("&roomid=");
	postData.append(to_string(roomID));
	postData.append("&ruid=");
	postData.append(to_string(GetMasterID(roomID)));
	postData.append("&num=");
	postData.append(to_string(num));
	if(coinType)
		postData.append("&coinType=gold");
	else
		postData.append("&coinType=silver");
	postData.append("&Bag_id=");
	postData.append(to_string(bagID));
	postData.append("&timestamp=");
	postData.append(to_string(f.GetTimeStamp()));
	postData.append("&rnd=");
	postData.append(to_string(f.GetTimeStamp() - 20));
	postData.append("&token=");
	postData.append(GetToken());
	wstring ret = f.HttpPostWithoutProp("http://live.bilibili.com/giftBag/send", postData.c_str(), cacheFile.c_str(), cookieFile.c_str());
	rapidjson::Document doc;
	doc.Parse(f.WstringToString(ret).c_str());
	if (!doc.IsObject())
		return -1;
	if (!doc.HasMember("msg"))
		return -2;
	if (strcmp(doc["msg"].GetString(), "ok"))
		return -3;
	return 0;
}

int Live::FreeSilverGetCaptcha()
{
	return 0;
}

int Live::FreeSilverGet()
{
	return 0;
}

Live::Live()
{
}


Live::~Live()
{
}
