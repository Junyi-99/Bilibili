#include "stdafx.h"
#include <stdio.h>
#include "bili.h"

using namespace std;

int main(void)
{
	bili b;
	int a = b.Login("15552878163", "Kt!OW0n@");/*1 请求登陆失败 -626 用户不存在 -627 密码错误 -105 刚改完密码无法使用此登录方式 否则返回未知代码 */
	if (a == 0)
		printf("Login success!\n");
	bili::LiveInfo l;
	bili::SignInfo s;
	bili::LiveRoomInfo lr;
	//a = b.GetUserInfoLive(l);
	//cout << "GetUserInfo: " << a << endl;
	//a = b.GetSignInfo(s);
	//cout << "GetSignInfo: " << a << endl;
	a = b.GetLiveRoomInfo(lr);
	cout << "GetLiveRoomInfo: " << a << endl;
	system("pause");
	return 0;
}