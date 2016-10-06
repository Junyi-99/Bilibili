#include "stdafx.h"
#include <thread>
#include <vector>
#include "Live.h"
#include "Thread.h"
#include "Founctions.h"
#define MAX_USER 50
using namespace std;
extern vector <Live> l;
Founctions f2;
std::time_t result = std::time(nullptr);
bool lock;//锁。其它线程如果想要发送请求或者打印的话，先检查锁，等别人处理完了再轮到自己
bool isLocked = false;

std::thread liveHeart[MAX_USER];
std::thread refreshInfo[MAX_USER];
std::thread freeSilver[MAX_USER];

void thread_LiveHeart(Live &l)
{
	int code;
	while (1)
	{
		if (!isLocked)
			return;//如果未上锁，那么退出所有正在运行的线程;
		while(lock){std::this_thread::sleep_for(std::chrono::milliseconds(100));}
		lock = true;
		code = l.Heart();
		if (code == 0)
		{
			cout << std::asctime(std::localtime(&result)) << " 用户 " << l.GetUsername() << " 心跳一次" << endl;
		}
		else if (code == -403)
		{
			cout << std::asctime(std::localtime(&result)) << " 用户 " << l.GetUsername() << " 非法心跳" << endl;
		}
		else if (code == -101)
		{//请先登录
			cout << std::asctime(std::localtime(&result)) << " 用户 " << l.GetUsername() << " 请先登录" << endl;
		}
		else
		{
			cout << std::asctime(std::localtime(&result)) << " 用户 " << l.GetUsername() << " 未知代码" << endl;
		}
		lock = false;
		std::this_thread::sleep_for(std::chrono::minutes(5));//线程休眠 5 分钟；
	}
}

void thread_RefreshInfo(Live &l)
{
	while (1)
	{
		if (!isLocked)
			return;//如果未上锁，那么退出所有正在运行的线程;
		std::this_thread::sleep_for(std::chrono::hours(4));//线程 4 小时；
		while (lock) { std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
		lock = true;
		cout << std::asctime(std::localtime(&result)) << " 用户 " << l.GetUsername() << " 刷新信息" << endl;
		l.FreshUserInfo();
		lock = false;
	}
}

void thread_FreeSilver(Live &l)
{
	int code, failedTimes = 0;
	while (1)
	{
		if (!isLocked)
			return;//如果未上锁，那么退出所有正在运行的线程;
		while (lock) { std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
		lock = true;
		code = l.FreeSilverGetCurrentTask(l.currentTask);
		lock = false;
		if (code == -101)
		{//请先登录
			break;
		}
		else if (code == 0)
		{
			while ((l.currentTask.time_end - (int)f2.GetTimeStamp()) > 0)//一直休眠到可以领瓜子为止
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));//线程休眠 1 秒
			}
			while (lock) { std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
			lock = true;
			l.FreeSilverGet();
			lock = false;
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
		else if (code == -10017)
		{//今日已领完银瓜子
			break;
		}
		else
		{
			failedTimes++;
			std::this_thread::sleep_for(std::chrono::minutes(5));//失败， 5 分钟后重试
			if (failedTimes == 5)
				break;
		}
	}
	cout << " 用户 " << l.GetUsername() << " thread_FreeSilver线程退出" << endl;
}
extern void setColor(const char* color);
void tmd_create()
{
	if (isLocked)
	{//防止重复创建线程，并且可以让线程检测自己是否可以存活
		setColor("red");
		printf("can not create more thread!\n");
		setColor("white");
		printf("> ");
	}
	isLocked = true;
	for (unsigned int i = 0; i < l.size(); i++)
	{
		liveHeart[i] = thread(thread_LiveHeart, l[i]);
		refreshInfo[i] = thread(thread_RefreshInfo, l[i]);
		freeSilver[i] = thread(thread_FreeSilver, l[i]);
	}
	while (1)
	{
		string a;
		cin >> a;
		if (a.compare("!q"))//这个地方有BUG。重现过程： [--debug !q !q <Enter> <Enter> --debug] 未响应
		{
			isLocked = false;//设置这个标志，可以不用杀死线程，让线程自己退出
			lock = false;
			setColor("white");
			printf("\n\n> ");
			return;
		}
	}
}