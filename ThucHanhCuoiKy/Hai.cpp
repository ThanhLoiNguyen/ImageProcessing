#include "Hai.h"
#include <iostream>
#include <utility>
#include <queue>

using namespace cv;
using namespace std;

typedef pair<int, int> pii;

void GrayscaletoBinary(Mat &src, Mat &dst)
{
	normalize(src, dst, 0, 1, NORM_MINMAX);
	normalize(dst, dst, 0, 255, NORM_MINMAX);
}

double pixelDistance(Vec3b p1, Vec3b p2)
{
	double res = 0;
	for (int i = 0; i < 3; ++i)
	{
		res += (1.0*p1[i] - p2[i])*(1.0*p1[i] - p2[i]);
	}
	return sqrt(res);
}

void RegionGrowingSegmentation(Mat &src, Mat &dst, int seedx, int seedy, double threshold)
{
	const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	const int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	Mat res = src.clone();
	Mat visited = Mat::zeros(src.size(), CV_8U);

	queue<pii> q;
	q.push(pii(seedx, seedy));
	visited.at<uchar>(seedx, seedy) = 1;

	while (!q.empty())
	{
		int ux = q.front().first, uy = q.front().second;
		q.pop();

		for (int i = 0; i < 8; ++i)
		{
			int adjx = ux + dx[i], adjy = uy + dy[i];
			if (adjx >= 0 && adjx < src.rows && adjy >= 0 && adjy < src.cols
				&& !visited.at<uchar>(adjx, adjy)
				&& pixelDistance(src.at<Vec3b>(seedx,seedy), src.at<Vec3b>(adjx, adjy)) < threshold)
			{
				res.at<Vec3b>(adjx, adjy) = Vec3b(255, 255, 255) - src.at<Vec3b>(seedx, seedy);
				visited.at<uchar>(adjx, adjy) = 1;
				q.push(pii(adjx, adjy));
			}
		}
	}

	dst = res.clone();
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