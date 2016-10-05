#pragma once
#include "cv.h"  
#include "cxcore.h"  
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "highgui.h"
#include <algorithm>
using namespace cv;
using namespace std;
class CaptchaRecognition
{
public:
	int getCaptchaValue(const char * captchaPath);
	CaptchaRecognition();
	~CaptchaRecognition();
};

