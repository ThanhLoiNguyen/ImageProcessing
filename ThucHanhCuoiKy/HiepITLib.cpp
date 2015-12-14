#include "HiepITLib.h"
#include <iostream>
using namespace std;

// Normalizes a given image into a value range between 0 and 255.
Mat norm_0_255_helper(const Mat& src) {
	// Create and return normalized image:
	Mat dst;
	switch (src.channels()) {
	case 1:
		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

// Converts the images given in src into a row matrix.
Mat asRowMatrix_helper(const vector<Mat>& src, int rtype, double alpha = 1, double beta = 0) {
	// Number of samples:
	size_t n = src.size();
	// Return empty matrix if no matrices given:
	if (n == 0)
		return Mat();
	// dimensionality of (reshaped) samples
	size_t d = src[0].total();
	// Create resulting data matrix:
	Mat data(n, d, rtype);
	// Now copy data:
	for (int i = 0; i < n; i++) {
		//
		if (src[i].empty()) {
			string error_message = format("Image number %d was empty, please check your input data.", i);
			CV_Error(CV_StsBadArg, error_message);
		}
		// Make sure data can be reshaped, throw a meaningful exception if not!
		if (src[i].total() != d) {
			string error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, d, src[i].total());
			CV_Error(CV_StsBadArg, error_message);
		}
		// Get a hold of the current row:
		Mat xi = data.row(i);
		// Make reshape happy by cloning for non-continuous matrices:
		if (src[i].isContinuous()) {
			src[i].reshape(1, 1).convertTo(xi, rtype, alpha, beta);
		}
		else {
			src[i].clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
		}
	}
	return data;
}

//6.1 Phân tích thành phần chính
//6.2 Rút trích đặc trưng cho tập ảnh mặt người dựa vào phân tích thành phần chính
void extractFaceFeature()
{
	// Holds some images:
	vector<Mat> db;
	string prefix = "Resources/att_faces/";

	db.push_back(imread(prefix + "s1/1.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s1/2.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s1/3.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s1/4.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s1/5.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s1/6.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s1/7.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s1/8.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s1/9.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s1/10.pgm", IMREAD_GRAYSCALE));

	db.push_back(imread(prefix + "s2/1.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s2/2.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s2/3.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s2/4.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s2/5.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s2/6.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s2/7.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s2/8.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s2/9.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s2/10.pgm", IMREAD_GRAYSCALE));


	db.push_back(imread(prefix + "s3/1.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s3/2.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s3/3.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s3/4.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s3/5.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s3/6.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s3/7.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s3/8.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s3/9.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s3/10.pgm", IMREAD_GRAYSCALE));

	db.push_back(imread(prefix + "s4/1.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s4/2.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s4/3.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s4/4.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s4/5.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s4/6.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s4/7.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s4/8.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s4/9.pgm", IMREAD_GRAYSCALE));
	db.push_back(imread(prefix + "s4/10.pgm", IMREAD_GRAYSCALE));

	// Build a matrix with the observations in row:
	Mat data = asRowMatrix_helper(db, CV_32FC1);

	// Number of components to keep for the PCA:
	int num_components = 10;

	// Perform a PCA:
	PCA pca(data, Mat(), CV_PCA_DATA_AS_ROW, num_components);

	// And copy the PCA results:
	Mat mean = pca.mean.clone();
	Mat eigenvalues = pca.eigenvalues.clone();
	Mat eigenvectors = pca.eigenvectors.clone();

	// The mean face:
	imshow("Mean Face", norm_0_255_helper(mean.reshape(1, db[0].rows)));

	// The first three eigenfaces:
	imshow("Eigenfaces 1", norm_0_255_helper(pca.eigenvectors.row(0)).reshape(1, db[0].rows));
	imshow("Eigenfaces 2", norm_0_255_helper(pca.eigenvectors.row(1)).reshape(1, db[0].rows));
	imshow("Eigenfaces 3", norm_0_255_helper(pca.eigenvectors.row(2)).reshape(1, db[0].rows));
	
	// Show the images:
	waitKey(0);
}

//7.2 Phân đoạn ảnh dùng phương pháp K-means
// KMean sử dụng thư viện kmeans
void KMeansLib(Mat src, Mat &dst, int clusterCount)
{
	std::cout << "Kmean Processing. It can take some seconds, please wait!" << std::endl;

	Mat samples(src.rows * src.cols, 3, CV_32F);
	for (int y = 0; y < src.rows; y++)
	for (int x = 0; x < src.cols; x++)
	for (int z = 0; z < 3; z++)
		samples.at<float>(y + x*src.rows, z) = src.at<Vec3b>(y, x)[z];

	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);

	Mat new_image(src.size(), src.type());
	for (int y = 0; y < src.rows; y++)
	for (int x = 0; x < src.cols; x++)
	{
		int cluster_idx = labels.at<int>(y + x*src.rows, 0);
		new_image.at<Vec3b>(y, x)[0] = centers.at<float>(cluster_idx, 0);
		new_image.at<Vec3b>(y, x)[1] = centers.at<float>(cluster_idx, 1);
		new_image.at<Vec3b>(y, x)[2] = centers.at<float>(cluster_idx, 2);
	}

	dst = new_image.clone();

	std::cout << "Kmean process completely!" << std::endl;
}

// Kmean tự code - Chưa cài
void Kmeans(Mat src, Mat &dst, int clusterCount)
{
	std::cout << "Kmean Processing. It can take some seconds, please wait!" << std::endl;

	Mat samples(src.rows * src.cols, 3, CV_32F);
	for (int y = 0; y < src.rows; y++)
	for (int x = 0; x < src.cols; x++)
	for (int z = 0; z < 3; z++)
		samples.at<float>(y + x*src.rows, z) = src.at<Vec3b>(y, x)[z];

	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);

	Mat new_image(src.size(), src.type());
	for (int y = 0; y < src.rows; y++)
	for (int x = 0; x < src.cols; x++)
	{
		int cluster_idx = labels.at<int>(y + x*src.rows, 0);
		new_image.at<Vec3b>(y, x)[0] = centers.at<float>(cluster_idx, 0);
		new_image.at<Vec3b>(y, x)[1] = centers.at<float>(cluster_idx, 1);
		new_image.at<Vec3b>(y, x)[2] = centers.at<float>(cluster_idx, 2);
	}

	dst = new_image.clone();

	std::cout << "Kmean process completely!" << std::endl;
}