#include "stdafx.h"
#include <stdio.h>
#include "bili.h"
#include "Live.h"
#include "Founctions.h"
#include <vector>
#include "Thread.h"
//编译OpenSSL的时候，用低版本编译，比如1.0.1o
//https://github.com/openssl/openssl/releases/tag/OpenSSL_1_0_1o
//需要编译 libcurl - OpenSSL 版本的静态库

const int MAX_USER = 50;
using namespace std;
vector <Live> l;
Founctions f;
Live newLive;

void debugMode()
{
	
	//l[0].FreeSilverGetCaptcha();
	//cout << "验证码识别结果：" << f.getCaptchaValue(l[0].GetCaptchaFilePath().c_str()) << endl;
	//cout << f.getCaptchaValue("C:\\Users\\Junyi\\Desktop\\1.jpg") << endl;
	//cout << f.getCaptchaValue("C:\\Users\\Junyi\\Desktop\\2.jpg") << endl;
	//cout << f.getCaptchaValue("C:\\Users\\Junyi\\Desktop\\3.jpg") << endl;
	tmd_create();
	//cout << tmd::getSize();
	printf("> ");
}
void setColor(const char* color)
{
	if (!strcmp(color, "white"))
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	else if (!strcmp(color, "green"))
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	else if (!strcmp(color, "yellow"))
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	else if (!strcmp(color, "pink"))
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
	else if (!strcmp(color, "blue"))
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	else if (!strcmp(color, "red"))
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	else if (!strcmp(color, "yellow"))
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	else if (!strcmp(color, "pink"))
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
	else if (!strcmp(color, "cyan"))
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);//设置颜色，没有添加颜色，故为原色

}
void printHeader()
{
    system("cls");
	setColor("white");
    printf("\
 __   \n\
[__) * | * |_  * | *\n\
[__) | | | [_) | | |\n\
\n\
");

	printf("\
                   _     _              _   \n\
     /\\           (_)   | |            | |  \n\
    /  \\   ___ ___ _ ___| |_ __ _ _ __ | |_ \n\
   / /\\ \\ / __/ __| / __| __/ _` | '_ \\| __|\n\
  / ____ \\\\__ \\__ \\ \\__ \\ || (_| | | | | |_ \n\
 /_/    \\_\\___/___/_|___/\\__\\__,_|_| |_|\\__|\n\
		\n");
	setColor("default");
	printf("想来肛我吗！！！！哈哈哈哈哈来啊\n");
	setColor("white");
}
void printHelp()
{
    printHeader();
    printf("\n\
  > home   \t\t\t 回到主页面     \n\
  > new <username> <password>  \t 添加一个新用户     \n\
  > del <username>  \t\t 删除一个用户	\n\
  > login <username>  \t\t 使 <username> 上线\n\
  > login /all  \t\t 登陆所有用户 \n\
  > logout <username>	 \t 使 <username> 下线\n\
  > logout /all  \t\t 使所有用户下线	\n\
  > list  \t\t\t 查看已保存的用户	\n\
  > info <username>  \t\t 查看 <username> 的信息	\n");
	setColor("yellow");
	printf("  > --debug  \t\t\t 进入 Debug 模式	\n");
	setColor("white");
	printf("> ");
}
void newUser(string command)
{
    vector<string> result = f.split(command, " ");
    if (result.size() != 3)
    {
        printf("输入格式不正确！\n> ");
		return;
    }
	for (unsigned int i = 0; i < l.size(); i++)
	{//判重（其实用 map<Live> 也行啊 捂脸）
		if (l[i].GetUsername().compare(result[1]) == 0)
		{
			setColor("yellow");
			printf("用户 %s 已存在\n", result[1].c_str());
			setColor("white");
			printf("> ");
			return;
		}
	}

	if (l.size() == 50)
	{
		setColor("yellow");
		printf("已存在 %d 个用户，不能再添加了！\n", MAX_USER);
		setColor("white");
		printf("> ");
		return;
	}
	l.push_back(newLive);//先让他进去
    int ret = l.back().Login(result[1], result[2]);
    if (ret == 0)
    {
		setColor("green");
        printf("登陆成功！正在刷新用户信息\n");
		l.back().FreshUserInfo();
        printf("用户信息刷新完成！\n");
    }
    else
    {
		/*1 请求登陆失败 -626 用户不存在 -627 密码错误 -105 刚改完密码无法使用此登录方式 否则返回未知代码 */
		l.pop_back();//弹出刚才进入 vector 的元素
		setColor("red");
        if (ret == -626)
            printf("登录失败：用户不存在\n");
        else if (ret == -627)
            printf("登录失败：密码错误\n");
        else if (ret == -105)
            printf("登录失败：刚修改完密码不能免验证码登陆\n");
        else printf("登录失败：未知代码 %d\n", ret);
    }
	setColor("white");
    printf("> ");
}
void loginUser(string command)
{
	bool flag = false;
	vector<string> result = f.split(command, " ");
	if (result.size() != 2)
	{
		printf("输入格式不正确！\n> ");
		return;
	}
	setColor("yellow");
	for (unsigned int i = 0; i < l.size(); i++)
	{
		if (result[1].compare("/all") == 0)
		{
			l[i].setLoginStatus(true);
			printf("用户 %s 已上线\n", l[i].GetUsername().c_str());
			flag = true;
		}
		if (l[i].GetUsername().compare(result[1]) == 0)
		{
			if (!l[i].getLoginStatus())
			{
				l[i].setLoginStatus(true);
				printf("用户 %s 已上线\n", result[1].c_str());
				flag = true;
				break;
			}
			else
			{
				printf("用户 %s 已在线上\n", result[1].c_str());
				flag = true;
				break;
			}
		}
	}
	if (!flag)
	{
		printf("找不到用户 %s \n", result[1].c_str());
	}
	setColor("white");
	printf("> ");
}
void logoutUser(string command)
{
	bool flag = false;
	vector<string> result = f.split(command, " ");
	if (result.size() != 2)
	{
		printf("输入格式不正确！\n> ");
		return;
	}
	setColor("yellow");
	for (unsigned int i = 0; i < l.size(); i++)
	{
		if (result[1].compare("/all") == 0)
		{
			l[i].setLoginStatus(false);
			printf("用户 %s 已下线\n", l[i].GetUsername().c_str());
			flag = true;
		}
		else if (l[i].GetUsername().compare(result[1]) == 0)
		{
			if (l[i].getLoginStatus())
			{
				l[i].setLoginStatus(false);
				printf("用户 %s 已下线\n", result[1].c_str());
				flag = true;
				break;
			}
			else
			{
				printf("用户 %s 未在线\n", result[1].c_str());
				flag = true;
				break;
			}
		}
	}
	if (!flag)
	{
		printf("找不到用户 %s \n", result[1].c_str());
	}
	setColor("white");
	printf("> ");
}
void delUser(string command)
{
	//BUG解决：http://blog.csdn.net/pecywang/article/details/8990795
	bool flag = false;
	Live temp;
	vector<string> result = f.split(command, " ");
	if (result.size() != 2)
	{
		printf("输入格式不正确！\n> ");
		return;
	}
	setColor("yellow");
	vector<Live>::iterator it;
	int index = 0;
	for (it = l.begin(); it != l.end(); )//注意这里的 erase 和 it 的写法
	{
		if ((*it).GetUsername().compare(result[1]) == 0)
		{
			it = l.erase(it);
			printf("已删除用户 %s\n", result[1].c_str());
			flag = true;
		}
		else
		{
			++it;
		}
	}
	if (!flag)
	{
		printf("找不到用户 %s \n", result[1].c_str());
	}
	setColor("white");
	printf("> ");
}
void userList()
{
    printf("%-20s%-20s%-20s\n", "用户名", "昵称", "状态");
    for (unsigned int i = 0; i < l.size(); i++)
    {
        string username, uname;
        username = l[i].GetUsername();
        uname = l[i].GetNickname();
		setColor("cyan");
		printf("%-20s", username.c_str());
		setColor("pink");
        printf("%-20s", uname.c_str());
        if (l[i].getLoginStatus())
        {
			setColor("green");
            printf("%-20s\n", "● Online");
			setColor("white");
        }
        else
        {
			setColor("default");
            printf("%-20s\n", "● Offline");
        }
    }
	setColor("white");
    printf("\n> ");
}
void userInfo(string command)
{
	bool flag = false;
	vector<string> result = f.split(command, " ");
	if (result.size() != 2)
	{
		printf("输入格式不正确！\n> ");
		return;
	}
	for (unsigned int i = 0; i < l.size(); i++)
	{
		if (l[i].GetUsername().compare(result[1]) == 0)
		{
			printHeader();
			printf("用户 %s 的信息:\n\n", result[1].c_str());
			setColor("white");
			printf("  是否老爷用户：");
			if (l[i].liveInfo.vip == 1)
			{
				setColor("green");
				printf("是");
			}
			else
			{
				setColor("yellow");
				printf("否");
			}
			setColor("default");
			printf("   硬币：%.1f  B坷垃：%d\n", l[i].avInfo.coins, l[i].avInfo.bCoins);
			printf("  成就值：");
			setColor("cyan");
			printf("%d", l[i].liveInfo.achieve);
			setColor("white");
			printf("  银瓜子：%d 金瓜子： ", l[i].liveInfo.silver);
			setColor("yellow");
			printf("%d\n", l[i].liveInfo.gold);
			setColor("cyan");
			printf("  主站等级：%d (%d / %d) \n   距离升级：%d\n", l[i].avInfo.current_level, l[i].avInfo.current_exp, l[i].avInfo.next_exp, 
				l[i].avInfo.next_exp - l[i].avInfo.current_exp);
			setColor("pink");
			printf("  直播站等级：%d (%d / %d) \n   距离升级：%d\n", l[i].liveInfo.user_level, 
				l[i].liveInfo.user_intimacy, l[i].liveInfo.user_next_intimacy, l[i].liveInfo.user_next_intimacy - l[i].liveInfo.user_intimacy);
			setColor("green");
			printf("  主播等级：%d (%d / %d) \n   距离升级：%d\n", l[i].liveRoomInfo.level, 
				l[i].liveRoomInfo.currentExp, l[i].liveRoomInfo.allExp, l[i].liveRoomInfo.allExp - l[i].liveRoomInfo.currentExp);
			flag = true;
		}
	}
	setColor("yellow");
	if (!flag)
		printf("找不到用户 %s\n", result[1].c_str());
	setColor("white");
	printf("> ");
}
int main(void)
{
	setColor("white");
    printHeader();
    printf("\n欢迎使用 Bilibili 助手\n输入 /help 查看帮助\n> ");
    string command;
    while (1)
    {
        getline(cin, command);
        if (!command.compare("exit"))
        {
            return 0;
        }
		else if (!command.compare("home"))
		{
			printHeader();
			printf("\n> ");
		}
        else if (!command.compare("/h"))
        {
            printHelp();
        }
        else if (!command.compare("/help") || !command.compare("help"))
        {
            printHelp();
        }
        else if (!command.compare("-h") || !command.compare("-H"))
        {
            printHelp();
        }
        else if (!command.compare("h") || !command.compare("H"))
        {
            printHelp();
        }
        else if (!strcmp(command.substr(0, 3).c_str(), "new"))
        {
            newUser(command);
        }
		else if (!strcmp(command.substr(0, 6).c_str(), "logout"))
		{
			logoutUser(command);
		}
		else if (!strcmp(command.substr(0, 5).c_str(), "login"))
		{
			loginUser(command);
		}
		else if (!strcmp(command.substr(0, 3).c_str(), "del"))
		{
			delUser(command);
		}
		else if (!strcmp(command.substr(0, 4).c_str(), "info"))
		{
			userInfo(command);
		}
        else if (!command.compare("list"))
        {
            userList();
        }
        else if (!command.compare("l"))
        {
            userList();
        }
		else if (!command.compare("--debug"))
		{
			debugMode();
		}
		else if (!command.compare("\n"))
		{
			printf("> ");
		}
        else { printf("未知命令 输入 /help 查看帮助\n> "); }
    }
    system("pause");
    return 0;
}