#include "stdafx.h"
#include "Picture.h"
using namespace std;
// 这个动态申请二维内存(指针数组)的函数很有意思
template<typename T>
void NewArr(T**& Array, int row, int line)
{
	Array = new T*[row];
	for (int i = 0; i != row; i++)
	{
		Array[i] = new T[line];
		memset(Array[i], 0, sizeof(T) * line);
	}
}
// 这个动态释放二维内存的函数也很有意思
template<typename T>
void DeleteArray(T**& Array, int line)
{
	for (int i = 0; i < line; i++)
	{
		delete[] Array[i];
		Array[i] = NULL;
	}
	delete[] Array;
	Array = NULL;
}
// path 文件图片的绝对地址
// bf, bi作为两个输出参数, 函数内部完成初始化
// lpBit, 图像数据, 一个超级大的二维数组(一级指针数组), 可以丢一个空指针进去, 函数内部代为申请内存
void ReadBitmap(const char* path, BITMAPFILEHEADER& bf, BITMAPINFOHEADER& bi, unsigned char**& lpBit)
{
	FILE* fp = fopen(path, "rb");
	if (fp == NULL)
		exit(0);

	fread(&bf, sizeof(BITMAPFILEHEADER), 1, fp); // 写入文件头
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, fp); // 写入信息头

	NewArr(lpBit, bi.biHeight, (bi.biWidth * 3 + 3) / 4 * 4);
	for (int i = 0; i < bi.biHeight; i++)
		for (int j = 0; j < (bi.biWidth * 3 + 3) / 4 * 4; j++)
			fread(&lpBit[i][j], sizeof(unsigned char), 1, fp); // 每次读入一个字节
			
	fclose(fp);
}
// path 文件图片的绝对地址
// bf, bi作为两个输入参数,
// lpBit, 图像数据, 经过ReadBitmap初始化后作为输入参数
void WriteBitmap(const char* path, BITMAPFILEHEADER& bf, BITMAPINFOHEADER& bi, unsigned char**& lpBit, BYTE limit, Picture::bitmap &bm)
{
	FILE* fp;
	fp = fopen(path, "wb");
	if (fp == NULL)
		exit(0);

	BITMAPFILEHEADER newbf; // 初始化文件头
	memcpy(&newbf, &bf, sizeof(BITMAPFILEHEADER));
	int newbfWidth = (bi.biWidth + 3) / 4 * 4; // 4字节补齐
	int newbfImageSize = newbfWidth * bi.biHeight; // 位图数据实际大小
	newbf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	newbf.bfSize = newbfImageSize + newbf.bfOffBits; // 位图文件实际大小

	BITMAPINFOHEADER newbi; // 初始化信息头
	memcpy(&newbi, &bi, sizeof(BITMAPINFOHEADER));
	newbi.biSizeImage = newbfImageSize;
	newbi.biBitCount = 8;

	fwrite(&newbf, sizeof(BITMAPFILEHEADER), 1, fp); // 写入文件头
	fwrite(&newbi, sizeof(BITMAPINFOHEADER), 1, fp); // 写入信息头

	RGBQUAD* pRGB = new RGBQUAD[256]; // 初始化调色板
	for (int i = 0; i != 256; i++)
		pRGB[i].rgbRed = pRGB[i].rgbGreen = pRGB[i].rgbBlue = i;
	fwrite(pRGB, sizeof(RGBQUAD), 256, fp); // 写入调色板

	unsigned char** Array;
	NewArr(Array, newbi.biHeight, newbfWidth);
	bm.height = newbi.biHeight;
	bm.width = newbfWidth;
	for (int i = 0; i < newbi.biHeight; i++)
	{
		for (int j = 0; j < newbfWidth; j++)
		{
			//                        newImagedata[Pos + j] = (ImageData[i][j * 3] + ImageData[i][j * 3 + 1] + ImageData[i][j * 3 + 2]) / 3; // 一次写入
			Array[i][j] = lpBit[i][j * 3] * 0.114 +
				lpBit[i][j * 3 + 1] * 0.587 +
				lpBit[i][j * 3 + 2] * 0.299; // 三个依次为 B G R的颜色分量 进行灰度化
			Array[i][j] > limit ? Array[i][j] = 255 : Array[i][j] = 0; // 此处设置阀值
			if ((int)Array[i][j] == 255)
			{
				bm.array[i*bm.width + j] = false;
			}
			else
			{
				bm.array[i*bm.width + j] = true;
			}
			fwrite(&Array[i][j], sizeof(unsigned char), 1, fp); // 按每次一个字节写入
		}
	}
	DeleteArray(Array, newbi.biHeight);
	fclose(fp);
}
//二值化
void Picture::Binaryzation(const char* sourcePath, const char* destPath, Picture::bitmap &bm)
{
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	memset(&bf, 0, sizeof(BITMAPFILEHEADER));
	memset(&bi, 0, sizeof(BITMAPINFOHEADER));
	unsigned char** lpBit = NULL;
	// 函数内部代为分配内存, 参数为二级指针, 所以函数内部动态传递出内存则形参为二级指针的引用或三级指针
	ReadBitmap(sourcePath, bf, bi, lpBit);
	WriteBitmap(destPath, bf, bi, lpBit, 128, bm);
	DeleteArray(lpBit, bi.biHeight);
}
//找图 用一位数组代替二维bitmap
Picture::Point Picture::FindPicture(Picture::bitmap source, Picture::bitmap pic)
{
	Picture::Point p;
	bool flag = false;

	freopen("1.txt", "w", stdout);
	for (int i = 0; i < pic.height; i++)
	{
		for (int j = 0; j < pic.width-1; j++)
		{
			if (pic.array[i*pic.width + j])
			{
				cout << ".";
			}
			else
				cout << " ";
		}
		cout << endl;
	}
	
	for (int i = 0; i <= source.height - pic.height; i++)//枚举欲寻找图片左上角坐标
	{
		for (int j = 0; j <= source.width - pic.width; j++)
		{
			flag = false;
			for (int k = 0; k < pic.height-1; k++)
			{
				for (int l = 0; l < pic.width-1; l++)
				{
					if (pic.array[k*pic.width + l] == source.array[(i + k)*source.width + (j + l)])
					{
						flag = true;
					}
					else
					{
						flag = false;
						break;
					}
				}
				if (!flag)
					break;
			}
			if (flag)
			{
				p.x = j;
				p.y = i;
				return p;
			}
		}
	}
	p.x = p.y = -1;
	return p;
}
void Picture::ReadBMP(const char* picPath, Picture::bitmap &bm)
{
	string dstPath(picPath); dstPath.append("_.bmp");
	Binaryzation(picPath, dstPath.c_str(), bm);
}
Picture::Picture()
{
}


Picture::~Picture()
{
}
