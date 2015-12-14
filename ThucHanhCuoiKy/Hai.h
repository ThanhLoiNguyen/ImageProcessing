#pragma once

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;

// Chuyển ảnh xám sang ảnh nhị phân

void GrayscaletoBinary(Mat &src, Mat &dst);


// 7.1. Phương pháp Region Growing
// User seeding

void RegionGrowingSegmentation(Mat &src, Mat &dst, int seedx, int seedy, double threshold);


// 8.1. Toán tử hình thái học trên ảnh nhị phân
// Đối với các phần tử cấu trúc, giá trị -1 là "don't care"

void BinaryDilation(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory);
void BinaryDilationLib(Mat &src, Mat &dst);

void BinaryErosion(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory);
void BinaryErosionLib(Mat &src, Mat &dst);

void BinaryOpening(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory);
void BinaryOpeningLib(Mat &src, Mat &dst);

void BinaryClosing(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory);
void BinaryClosingLib(Mat &src, Mat &dst);

void BinaryBoundaryExtraction(Mat &src, Mat &dst);

void BinaryRegionFilling(Mat &src, Mat &dst, int seedx, int seedy);

void BinaryHitOrMiss(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory);

void BinaryThinning(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory);

void BinaryThickening(Mat &src, Mat &dst, int **kernel, int sz, int anchorx, int anchory);