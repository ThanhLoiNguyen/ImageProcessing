#pragma once

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;

void doSth(Mat &src, Mat &dst);

// 5.3. Lọc thông tần số thấp
// threshold có giá trị từ 0 đến 1

void IdealLowpassFilter(Mat &src, Mat &dst, float threshold);
void ButterworthLowpassFilter(Mat &src, Mat &dst, float threshold);
void GaussianLowpassFilter(Mat &src, Mat &dst, float threshold);


// 5.4. Lọc thông tần số cao
// threshold có giá trị từ 0 đến 1

void IdealHighpassFilter(Mat &src, Mat &dst, float threshold);
void ButterworthHighpassFilter(Mat &src, Mat &dst, float threshold);
void GaussianHighpassFilter(Mat &src, Mat &dst, float threshold);