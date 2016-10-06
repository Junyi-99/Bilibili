#pragma once
#include <string>
#include "Founctions.h"
#include "rapidjson\document.h"
#include <fstream>  
#include <streambuf>  
using namespace std;
/* 此类仅用来获取 Cookies 等信息 */
/* 请在 Live 类的析构函数里删除缓存文件 */
class bili
{
public:
	struct AVInfo
	{
		int current_level;
		int current_min;
		int current_exp;
		int next_exp;
		int bCoins;
		double coins;
		string face;
		string sex;
		string nameplate_current;
		string uname;
		int official_verify;
	}avInfo;
	struct LiveInfo
	{
		string uname;
		string face;
		int silver;
		int gold;
		int achieve;
		int vip;
		int svip;
		int user_level;
		int user_next_level;
		int user_intimacy;
		int user_next_intimacy;
		string user_level_rank;
		int billCoin;
	}liveInfo;
	struct SignInfo
	{
		string text;	/* 签到奖励 */
		int status;		/* 是否已签到 */
		string allDays;	/* 本月天数 */
		string curMonth;/* 当前月份 */
		int hadSignDays;/* 已经签到天数 */
		int newTask;
	}signInfo;
	struct LiveRoomInfo
	{
		int masterID;	/* 播主ID（RUID） */
		int roomID;		/* 房间ID */
		int level;		/* 当前等级 */
		int currentExp;	/* 当前经验值 */
		int allExp;		/* 总共需要的经验值 */
	}liveRoomInfo;
	struct FreeSilverTask
	{
		int minute;
		int silver;
		int time_start;
		int time_end;
	}currentTask;
	struct GiftBag
	{
		int id;
		int uid;
		int giftID;
		int giftNum;
		int expireat;

	}giftBag;
	int Login(const string username, const string password);
	int FreshUserInfo();
	int ExchangeSilver2Coin();
	int GetUserID();
	bool getLoginStatus() { return isLogin_; };
	void setLoginStatus(bool status) { isLogin_ = status; };
	wstring GetUserNameByUID(int UID);
	string GetUsername()const { return username_; };
	string GetNickname()const { return uname; };
	string GetCaptchaFilePath()const { return captchaFile; };
	bili() { isLogin_ = false; };
	~bili() {};
protected:
	int GetMasterID(int liveRoomID);//Get RUID
	string uname;
	string username_;
	string captchaFile;
	bool isLogin_;
	string CheckLogin();
	string cookieFile;
	string cacheFile;
	Founctions f;
private:

	int GetUserInfoLive(LiveInfo &l);
	int GetUserInfoAV(AVInfo &a);
	int GetSignInfo(SignInfo &s);
	int GetLiveRoomInfo(LiveRoomInfo &lr);
	
};