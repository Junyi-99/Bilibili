#pragma once
#include "Windows.h"
#include "stdafx.h"
class Picture
{
public:
	struct Point
	{
		int x, y;
	};
	struct bitmap
	{
		int width, height;
		bool array[250 * 50];//你妈逼！二维数组神tm有偏移！不信的话自己试试去
	};
	//二值化
	void Binaryzation(const char* sourcePath, const char* destPath, bitmap &bm);
	//找图 用一位数组代替二维bitmap
	Point FindPicture(bitmap source, bitmap pic);
	void ReadBMP(const char* picPath, bitmap &bm);
	Picture();
	~Picture();

};

