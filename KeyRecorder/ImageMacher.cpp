#include "ImageMacher.h"

#include <iostream>
#include <QDebug>

using namespace cv;

ImageMacher::ImageMacher()
{

}
int ImageMacher::enhanceImage()
{
	Mat image = imread("test1.jpg", 1);
	if (image.empty())
	{
		std::cout << "��ͼƬʧ��,����" << std::endl;
		return -1;
	}
	imshow("ԭͼ��", image);
	//Mat imageRGB[3];
	//split(image, imageRGB);
	//for (int i = 0; i < 3; i++)
	//{
	//	equalizeHist(imageRGB[i], imageRGB[i]);
	//}
	//merge(imageRGB, 3, image);
	//imshow("ֱ��ͼ���⻯ͼ����ǿЧ��", image);
	//waitKey();

	Mat dst = Mat::zeros(512, 512, CV_8UC3); //��Ҫת��Ϊ512*512��С��
	resize(image, dst, dst.size());

	imshow("�ߴ����֮��", dst);

	return 0;
}

int ImageMacher::grayImage(QByteArray& srcData, QByteArray& dstData, int nThreshold)
{
	Mat img, imgGray, result;

	_InputArray pic_arr(srcData.data(), srcData.count());
	img = cv::imdecode(pic_arr, IMREAD_COLOR);

	//img = imread("test1.jpg");
	if (!img.data) {
		qDebug() << "Please input image path" << endl;
		return 0;
	}
	imshow("ԭͼ", img);
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	imshow("�Ҷ�ͼ", imgGray);
	//blur(imgGray, imgGray, Size(3, 3));
	threshold(imgGray, result, nThreshold, 255, THRESH_BINARY);
	imshow("��ֵ�����ͼ", result);

	//expend(result);

	std::vector<uchar> dstBuf;
	imencode(".jpg", result, dstBuf);

	//imwrite("��ֵ���Ķ�ά��.jpg", result);
	//qDebug() << "ͼƬ�ѱ���" << endl;
	//waitKey();

	for (std::vector<uchar>::iterator itr = dstBuf.begin(); itr != dstBuf.end(); ++itr)
	{
		dstData.append(*itr);
	}
}


Mat ImageMacher::expend(Mat& roi)
{
	int w = roi.cols * 2;
	int h = roi.rows * 2;
	//��������ɫ��������
	Mat image = Mat::zeros(h, w, CV_8UC3);
	image.setTo(cv::Scalar(0, 0, 0));
	imshow("original", image);

	//��ȡ������ͼƬ
	imshow("roi", roi);

	//���û����������򲢸���
	int x = roi.cols / 2;
	int y = roi.rows / 2;
	Rect roi_rect = Rect(x, y, roi.cols, roi.rows);
	roi.copyTo(image(roi_rect));
	imshow("result", image);

	roi = image;

	return image;
}

bool ImageMacher::matchImage(QByteArray &srcBytes, QString &tmplatePath)
{
	//Mat src = imread("src3.png", 1);
	//Mat templ = imread("template2.png", 1);
	_InputArray pic_arr(srcBytes.data(), srcBytes.count());
	Mat src = cv::imdecode(pic_arr, IMREAD_COLOR);

	Mat templ = imread(tmplatePath.toStdString(), 1);

	double minVal, maxVal = -1;
	Point minLoc, maxLoc;
	Mat ftmp;
	matchTemplate(src, templ, ftmp, TM_SQDIFF);//��6��ƥ�䷽ʽ
	//normalize(ftmp, ftmp, 1, 0, NORM_MINMAX);
	minMaxLoc(ftmp, &minVal, &maxVal, &minLoc, &maxLoc); //�ҵ����ƥ���
	rectangle(src, Rect(minLoc.x, minLoc.y, templ.cols, templ.rows), cv::Scalar(0, 255, 0), 2, 0);

	imshow("src", src);
	imshow("template", templ);
	
	if (minVal < 10) {
		return true;
	}
	return false;


}