#include "Hai.h"
#include <iostream>
#include <utility>
#include <queue>

using namespace cv;
using namespace std;

typedef pair<int, int> pii;

void swapCorners(Mat &src)
{
	int cx = src.cols / 2;
	int cy = src.rows / 2;

	Mat q0(src, Rect(0, 0, cx, cy));
	Mat q1(src, Rect(cx, 0, cx, cy));
	Mat q2(src, Rect(0, cy, cx, cy));
	Mat q3(src, Rect(cx, cy, cx, cy));

	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

void IdealLowpassFilter(Mat &src, Mat &dst, float threshold)
{
	src.convertTo(dst, CV_32F);
	dft(dst, dst, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT);

	swapCorners(dst);
	Mat planes[2];
	split(dst, planes);

	float d0 = MAX(planes[0].rows, planes[1].rows) / 2.0 * threshold;
	float mm = planes[0].rows / 2.0;
	float nn = planes[0].cols / 2.0;

	for (int x = 0; x < planes[0].rows; ++x)
	for (int y = 0; y < planes[0].cols; ++y)
	{
		float d = sqrt((1.0*x - mm)*(1.0*x - mm) + (1.0*y - nn)*(1.0*y - nn));
		if (d > d0)
			planes[0].at<float>(x, y) = planes[1].at<float>(x, y) = 0;
	}

	merge(planes, 2, dst);
	swapCorners(dst);

	dft(dst, dst, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

	dst.convertTo(dst, CV_8U);
}

void ButterworthLowpassFilter(Mat &src, Mat &dst, float threshold)
{
	src.convertTo(dst, CV_32F);
	dft(dst, dst, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT);

	swapCorners(dst);
	Mat planes[2];
	split(dst, planes);

	float d0 = MAX(planes[0].rows, planes[1].rows) / 2.0 * threshold;
	float mm = planes[0].rows / 2.0;
	float nn = planes[0].cols / 2.0;

	for (int x = 0; x < planes[0].rows; ++x)
	for (int y = 0; y < planes[0].cols; ++y)
	{
		float d = sqrt((1.0*x - mm)*(1.0*x - mm) + (1.0*y - nn)*(1.0*y - nn));
		d = 1 + (d / d0)*(d / d0);
		planes[0].at<float>(x, y) /= d;
		planes[1].at<float>(x, y) /= d;
	}

	merge(planes, 2, dst);
	swapCorners(dst);

	dft(dst, dst, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

	dst.convertTo(dst, CV_8U);
}

void GaussianLowpassFilter(Mat &src, Mat &dst, float threshold)
{
	src.convertTo(dst, CV_32F);
	dft(dst, dst, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT);

	swapCorners(dst);
	Mat planes[2];
	split(dst, planes);

	float d0 = MAX(planes[0].rows, planes[1].rows) / 2.0 * threshold;
	float mm = planes[0].rows / 2.0;
	float nn = planes[0].cols / 2.0;

	for (int x = 0; x < planes[0].rows; ++x)
	for (int y = 0; y < planes[0].cols; ++y)
	{
		float d = sqrt((1.0*x - mm)*(1.0*x - mm) + (1.0*y - nn)*(1.0*y - nn));
		d = exp(-d*d / (2 * d0*d0));
		planes[0].at<float>(x, y) *= d;
		planes[1].at<float>(x, y) *= d;
	}

	merge(planes, 2, dst);
	swapCorners(dst);

	dft(dst, dst, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

	dst.convertTo(dst, CV_8U);
}

void IdealHighpassFilter(Mat &src, Mat &dst, float threshold)
{
	src.convertTo(dst, CV_32F);
	dft(dst, dst, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT);

	swapCorners(dst);
	Mat planes[2];
	split(dst, planes);

	float d0 = MAX(planes[0].rows, planes[1].rows) / 2.0 * threshold;
	float mm = planes[0].rows / 2.0;
	float nn = planes[0].cols / 2.0;

	for (int x = 0; x < planes[0].rows; ++x)
	for (int y = 0; y < planes[0].cols; ++y)
	{
		float d = sqrt((1.0*x - mm)*(1.0*x - mm) + (1.0*y - nn)*(1.0*y - nn));
		if (d < d0)
			planes[0].at<float>(x, y) = planes[1].at<float>(x, y) = 0;
	}

	merge(planes, 2, dst);
	swapCorners(dst);

	dft(dst, dst, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

	dst.convertTo(dst, CV_8U);
}

void ButterworthHighpassFilter(Mat &src, Mat &dst, float threshold)
{
	src.convertTo(dst, CV_32F);
	dft(dst, dst, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT);

	swapCorners(dst);
	Mat planes[2];
	split(dst, planes);

	float d0 = MAX(planes[0].rows, planes[1].rows) / 2.0 * threshold;
	float mm = planes[0].rows / 2.0;
	float nn = planes[0].cols / 2.0;

	for (int x = 0; x < planes[0].rows; ++x)
	for (int y = 0; y < planes[0].cols; ++y)
	{
		float d = sqrt((1.0*x - mm)*(1.0*x - mm) + (1.0*y - nn)*(1.0*y - nn));
		d = 1 + (d0 / d)*(d0 / d);
		planes[0].at<float>(x, y) /= d;
		planes[1].at<float>(x, y) /= d;
	}

	merge(planes, 2, dst);
	swapCorners(dst);

	dft(dst, dst, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

	dst.convertTo(dst, CV_8U);
}

void GaussianHighpassFilter(Mat &src, Mat &dst, float threshold)
{
	src.convertTo(dst, CV_32F);
	dft(dst, dst, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT);

	swapCorners(dst);
	Mat planes[2];
	split(dst, planes);

	float d0 = MAX(planes[0].rows, planes[1].rows) / 2.0 * threshold;
	float mm = planes[0].rows / 2.0;
	float nn = planes[0].cols / 2.0;

	for (int x = 0; x < planes[0].rows; ++x)
	for (int y = 0; y < planes[0].cols; ++y)
	{
		float d = sqrt((1.0*x - mm)*(1.0*x - mm) + (1.0*y - nn)*(1.0*y - nn));
		d = 1 - exp(-d*d / (2 * d0*d0));
		planes[0].at<float>(x, y) *= d;
		planes[1].at<float>(x, y) *= d;
	}

	merge(planes, 2, dst);
	swapCorners(dst);

	dft(dst, dst, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

	dst.convertTo(dst, CV_8U);
}