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
	string GetToken();
	Live();
	~Live();
private:
	int FreeSilverHeart();
	int FreeSilverGetCaptcha();
	
};

