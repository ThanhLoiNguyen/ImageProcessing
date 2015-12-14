#pragma once
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
using namespace cv;

//6.1 Phân tích thành phần chính
//6.2 Rút trích đặc trưng cho tập ảnh mặt người dựa vào phân tích thành phần chính
void extractFaceFeature();

//7.2 Phân đoạn ảnh dùng phương pháp K-means
// KMean sử dụng thư viện kmeans
// src: Ảnh màu nguồn
// dst: Ảnh kết quả
// clusterCount: Số đoạn ảnh muốn phân
void KMeansLib(Mat src, Mat &dst, int clusterCount = 5);
// Kmean tự code
void Kmeans(Mat src, Mat &dst, int clusterCount = 5);
