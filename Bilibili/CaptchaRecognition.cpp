#include "stdafx.h"
#include "CaptchaRecognition.h"
using namespace cv;
using namespace std;

int calcDiff(Mat uncvtColor, Mat cvtedColor)
{
	/*感谢 http://blog.csdn.net/wangyaninglm/article/details/43853435 */
	cv::Mat matDst1, matDst2;
	cv::resize(uncvtColor, matDst1, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	cv::resize(cvtedColor, matDst2, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	cv::cvtColor(matDst1, matDst1, CV_BGR2GRAY);//因为cvteColor已经是 GRAY 了！！！！摔！！！所以你再cvtColor一次就会异常！！！！！神他妈我调了2小时才弄出来！
												//cv::cvtColor(matDst2, matDst2, CV_BGR2GRAY);  <-- 你tm要是敢取消注释我弄死你！
	threshold(matDst1, matDst1, 100, 255, CV_THRESH_BINARY_INV);
	threshold(matDst2, matDst2, 100, 255, CV_THRESH_BINARY_INV);

	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64], arr2[64];
	for (int i = 0; i < 8; i++)
	{
		uchar* data1 = matDst1.ptr<uchar>(i);
		uchar* data2 = matDst2.ptr<uchar>(i);

		int tmp = i * 8;

		for (int j = 0; j < 8; j++)
		{
			int tmp1 = tmp + j;

			arr1[tmp1] = data1[j] / 4 * 4;
			arr2[tmp1] = data2[j] / 4 * 4;

			iAvg1 += arr1[tmp1];
			iAvg2 += arr2[tmp1];
		}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	for (int i = 0; i < 64; i++)
	{
		arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}

	int iDiffNum = 0;

	for (int i = 0; i < 64; i++)
		if (arr1[i] != arr2[i])
			++iDiffNum;

	/*cout << "iDiffNum = " << iDiffNum << endl;
	if (iDiffNum <= 5)
	cout << "two images are very similar!" << endl;
	else if (iDiffNum > 10)
	cout << "they are two different images!" << endl;
	else
	cout << "two image are somewhat similar!" << endl;*/
	return iDiffNum;
}
int findNumber(Mat cvtedImg)
{
	Mat now;
	int ret = 99;
	for (int i = 0; i < 12; i++)
	{
		char *a = new char;
		sprintf(a, "a_%d.bmp", i);
		now = imread(a, CV_LOAD_IMAGE_COLOR);
		int ret = calcDiff(now, cvtedImg);
		if (ret < 5)
		{
			return i;
		}
	}
	return -1;
}
bool cmp(vector<Point> &a, vector<Point> &b)//注意：本函数的参数的类型一定要与vector中元素的类型一致  
{
	Rect rc1 = boundingRect(a);
	Rect rc2 = boundingRect(b);
	return rc1.x < rc2.x;//升序排列  
}
int splitCaptcha(const char* imgPath, vector<Mat> &recv)
{
	Mat imgSrc = imread(imgPath, CV_LOAD_IMAGE_COLOR);
	if (imgSrc.empty() || !imgSrc.data)
	{
		return -1;
	}
	Mat src_gray, dst;
	cvtColor(imgSrc, src_gray, CV_BGR2GRAY);
	threshold(src_gray, dst, 100, 255, CV_THRESH_BINARY_INV);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Mat> result;
	findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	sort(contours.begin(), contours.end(), cmp);
	for (unsigned int i = 0; i < contours.size(); i++) {
		Rect rc = boundingRect(contours[i]);
		Mat temp(src_gray, rc);
		result.push_back(temp);
	}
	recv = result;
	return 0;
}
int CaptchaRecognition::getCaptchaValue(const char* captchaPath)
{
	vector<Mat> result;
	int ret = splitCaptcha(captchaPath, result);
	if (ret == -1)
		return -1;
	int res[5] = { 0 };
	for (unsigned int i = 0; i < result.size(); i++)
	{
		//imshow(to_string(i), result[i]);
		res[i] = findNumber(result[i]);
	}
	//printf("%d %d %d %d\n", res[0], res[1], res[2], res[3]);
	return res[2] == 10 ? res[0] * 10 + res[1] + res[3] : res[0] * 10 + res[1] - res[3];
}

CaptchaRecognition::CaptchaRecognition()
{
}


CaptchaRecognition::~CaptchaRecognition()
{
}
