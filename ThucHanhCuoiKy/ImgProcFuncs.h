#pragma once

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>

using namespace cv;
int** newKernel();
int** newKernel(int);
void repairKernel(int** kernel);
void deleteKernel(int **kernel);
// Chuyển ảnh xám sang ảnh nhị phân

void GrayscaletoBinary(Mat &src, Mat &dst);

//5.1 Biến đổi Fourier FFT thuận, nghịch
void FFT(Mat src, Mat &dst);
void inverseFFT(Mat src, Mat &dst);

//6.1 Phân tích thành phần chính
//6.2 Rút trích đặc trưng cho tập ảnh mặt người dựa vào phân tích thành phần chính
void extractFaceFeature();

// 7.1. Phương pháp Region Growing
// User seeding

void RegionGrowingSegmentation(Mat &src, Mat &dst, int seedx, int seedy, double threshold);

//7.2 Phân đoạn ảnh dùng phương pháp K-means
// KMean sử dụng thư viện kmeans
// src: Ảnh màu nguồn
// dst: Ảnh kết quả
// clusterCount: Số đoạn ảnh muốn phân
void KMeansLib(Mat src, Mat &dst, int clusterCount = 5);
// Kmean tự code
void Kmeans(Mat src, Mat &dst, int clusterCount = 5);



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


// 8.2 Toán tử hình thái học trên ảnh xám (chưa làm)
void GrayScaleDilation(Mat &src, Mat &dst, int **kernel, int sz);
void GrayScaleDilationLib(Mat &src, Mat &dst);

void GrayScaleErosion(Mat &src, Mat &dst, int **kernel, int sz);
void GrayScaleErosionLib(Mat &src, Mat &dst);

void GrayScaleOpening(Mat &src, Mat &dst, int **kernel, int sz);
void GrayScaleOpeningLib(Mat &src, Mat &dst);

void GrayScaleClosing(Mat &src, Mat &dst, int **kernel, int sz);
void GrayScaleClosingLib(Mat &src, Mat &dst);

void GrayscaleSmoothing(Mat &src, Mat &dst, int **kernel, int sz);
void GrayscaleSmoothingLib(Mat &src, Mat &dst);

void GrayscaleMorphologyGradient(Mat &src, Mat &dst, int **kernel, int sz);
void GrayscaleMorphologyGradientLib(Mat &src, Mat &dst);

void TopHatTransformation(Mat &src, Mat &dst, int **kernel, int sz);
void TopHatTransformationLib(Mat &src, Mat &dst);

void TexturalSegmentation(Mat &src, Mat &dst, int **kernel, int sz);
void TexturalSegmentationLib(Mat &src, Mat &dst);

void Granulometry();

void Reconstruction();