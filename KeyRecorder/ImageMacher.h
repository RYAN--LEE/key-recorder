#pragma once
#include <QByteArray>
#include <QRect>
#include <opencv2/highgui/highgui.hpp>    
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class ImageMacher
{
public:
	ImageMacher();

	int enhanceImage();
	int grayImage(QByteArray& srcData, QByteArray& dstData, int nThreshold=100);

	bool matchImage(QByteArray& srcBytes, QString &tmplatePath, QRect& matchRect);

private:
	Mat expend(Mat& roi);

};

