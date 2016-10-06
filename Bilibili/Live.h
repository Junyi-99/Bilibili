#pragma once
#include "bili.h"
#include "Founctions.h"
#include "stdafx.h"
class Live : public bili
{
public:
	int DoSign();
	int FreeSilverGetCurrentTask(bili::FreeSilverTask &currentTask);
	int FreeSilverGet();
	int Heart();
	int SendGift(int giftID, int roomID, int num, bool coinType, int bagID);
	int FreeSilverGetCaptcha();
	string GetToken();
	Live()
	{
		//cout << "bilibili 被创建了！" << endl;
	};
	~Live()
		/*如果你把这两个cout取消注释，键入new <username> <password> 会发现构造函数并没有被执行，当你 del <username> 的时候*/
		/* 会发现 构造函数跟析构函数一起被执行了 */
		/* 我也不知道这是什么魔法... */
		/* 似乎不管它，也没啥问题.... */
	{
		//cout << "bilibili 销毁！" << endl;

		//remove(cookieFile.c_str());
		//remove(cacheFile.c_str());
	};
private:
	int FreeSilverHeart();
	
	
};

