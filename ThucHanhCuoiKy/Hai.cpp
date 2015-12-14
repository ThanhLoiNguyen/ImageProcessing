#include "Hai.h"
#include <iostream>

using namespace cv;

void GrayscaletoBinary(Mat &src, Mat &dst)
{
	normalize(src, dst, 0, 1, NORM_MINMAX);
	normalize(dst, dst, 0, 255, NORM_MINMAX);
}

void BinaryDilation(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory)
{
	normalize(src, src, 0, 1, NORM_MINMAX);
	Mat res = src.clone();

	for (int x = 0; x < src.rows - sz; ++x)
	for (int y = 0; y < src.cols - sz; ++y)
	{
		uchar tmp = 0;

		for (int i = 0; i < sz; ++i)
		for (int j = 0; j < sz; ++j)
		if (kernel[i][j] != -1)
		{
			tmp |= kernel[i][j] & src.at<uchar>(x + i, y + j);
		}
		res.at<uchar>(x + anchorx, y + anchory) = tmp;
	}

	dst = res.clone();
	normalize(dst, dst, 0, 255, NORM_MINMAX);
	normalize(src, src, 0, 255, NORM_MINMAX);
}

void BinaryDilationLib(Mat &src, Mat &dst)
{
	dilate(src, dst, Mat());
}

void BinaryErosion(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory)
{
	normalize(src, src, 0, 1, NORM_MINMAX);
	Mat res = src.clone();

	for (int x = 0; x < src.rows - sz; ++x)
	for (int y = 0; y < src.cols - sz; ++y)
	{
		uchar tmp = 1;

		for (int i = 0; i < sz; ++i)
		for (int j = 0; j < sz; ++j)
		if (kernel[i][j] != -1)
		{
			tmp &= (!kernel[i][j]) | src.at<uchar>(x + i, y + j);
		}
		res.at<uchar>(x + anchorx, y + anchory) = tmp;
	}

	dst = res.clone();
	normalize(dst, dst, 0, 255, NORM_MINMAX);
	normalize(src, src, 0, 255, NORM_MINMAX);
}

void BinaryErosionLib(Mat &src, Mat &dst)
{
	erode(src, dst, Mat());
}

void BinaryOpening(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory)
{
	BinaryErosion(src, dst, kernel, sz, anchorx, anchory);
	BinaryDilation(dst, dst, kernel, sz, anchorx, anchory);
}

void BinaryOpeningLib(Mat &src, Mat &dst)
{
	Mat kernel = (cv::Mat_<uchar>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	erode(src, dst, kernel);
	dilate(dst, dst, kernel);
}

void BinaryClosing(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory)
{
	BinaryDilation(dst, dst, kernel, sz, anchorx, anchory);
	BinaryErosion(src, dst, kernel, sz, anchorx, anchory);
}

void BinaryClosingLib(Mat &src, Mat &dst)
{
	Mat kernel = (cv::Mat_<uchar>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	dilate(src, dst, kernel);
	erode(dst, dst, kernel);
}

void BinaryBoundaryExtraction(Mat &src, Mat &dst)
{
	Mat tmp = src.clone();
	Mat kernel = (cv::Mat_<uchar>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);

	erode(src, dst, kernel);
	
	dst = tmp - dst;
}

void BinaryRegionFilling(Mat &src, Mat &dst, int seedx, int seedy)
{
	normalize(src, src, 0, 1, NORM_MINMAX);
	Mat res = Mat::zeros(src.size(), CV_8U);
	Mat prev;
	res.at<uchar>(seedx, seedy) = 1;
	int tmp;
	Mat ker = (Mat_<uchar>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	do
	{
		res.copyTo(prev);
		dilate(res, res, ker);
		res &= (1 - src);
	} while ((tmp = countNonZero(res - prev)) > 0);

	normalize(src, src, 0, 255, NORM_MINMAX);
	normalize(res, res, 0, 255, NORM_MINMAX);
	dst = res.clone();
}

void BinaryHitOrMiss(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory)
{
	normalize(src, src, 0, 1, NORM_MINMAX);
	Mat res = src.clone();

	for (int x = 0; x < src.rows - sz; ++x)
	for (int y = 0; y < src.cols - sz; ++y)
	{
		bool hit = true;
		for (int i = 0; i < sz; ++i)
		for (int j = 0; j < sz; ++j)
		if (kernel[i][j] != -1)
		{
			hit &= !(src.at<uchar>(x + i, y + j) ^ kernel[i][j]);
		}
		res.at<uchar>(x + anchorx, y + anchory) = hit;
	}
	
	dst = res.clone();
	normalize(src, src, 0, 255, NORM_MINMAX);
	normalize(dst, dst, 0, 255, NORM_MINMAX);
}

void BinaryThinning(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory)
{
	Mat res;
	BinaryHitOrMiss(src, res, kernel, sz, anchorx, anchory);
	res = src - res;
	dst = res.clone();
}

void BinaryThickening(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory)
{
	Mat res;
	BinaryHitOrMiss(src, res, kernel, sz, anchorx, anchory);
	res = res + src;
	dst = res.clone();
}