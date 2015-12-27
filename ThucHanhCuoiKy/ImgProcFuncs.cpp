#include "ImgProcFuncs.h"
#include <iostream>
#include <vector>
#define PI 3.14159265
#define FW_MAX_8U 255.0
using namespace cv;


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


//5.1 Phép biến đổi Fourier dùng FFT thuận, nghịch
void FFT(Mat src, Mat &dst)
{
	if (src.empty())
		return;

	Mat padded;
	int m = getOptimalDFTSize(src.rows);
	int n = getOptimalDFTSize(src.cols); // trên cạnh thêm giá trị 0
	copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));
	//Copy ảnh src vào ảnh giữa ảnh padded, ảnh padded lúc này có cạnh là 0, ở giữa là ảnh src
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	//Các ảnh nhỏ được chia ra theo biến đổi Fourier, toàn 0
	Mat complexI;
	merge(planes, 2, complexI);         // Merge ảnh ComplexI với dãy ảnh toàn 0 planes

	idft(complexI, complexI);            // hàm biến đổi Fourier

	// tính toán mức độ và chuyển sang thang logarit
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];

	magI += Scalar::all(1);                    // chuyển sang thang logarit
	log(magI, magI);

	// căt ảnh thành dạng hình chữ nhật
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

	// chuẩn bị chia ảnh làm 4 góc
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // góc tư Top-Left
	Mat q1(magI, Rect(cx, 0, cx, cy));  // góc tư Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // góc tư Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // gốc tư Bottom-Right

	Mat tmp;                           // swap góc Top-Left và góc Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap góc Top-Right và góc Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magI, magI, 0, 1, CV_MINMAX); // chuẩn hóa ảnh về dạng chuẩn

	dst = magI.clone();
}

void inverseFFT(Mat src, Mat &dst)
{
	if (src.empty())
		return;

	Mat padded;
	int m = getOptimalDFTSize(src.rows);
	int n = getOptimalDFTSize(src.cols);
	copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);

	dft(complexI, complexI);
	split(complexI, planes);

	Mat inverseTransform;
	dft(complexI, inverseTransform, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
	normalize(inverseTransform, inverseTransform, 0, 1, CV_MINMAX);

	dst = inverseTransform.clone();
}
//--------------------------------------------------------------------------------


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

// 7.1 RegionGrowing
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
				&& pixelDistance(src.at<Vec3b>(seedx, seedy), src.at<Vec3b>(adjx, adjy)) < threshold)
			{
				res.at<Vec3b>(adjx, adjy) = Vec3b(255, 255, 255) - src.at<Vec3b>(seedx, seedy);
				visited.at<uchar>(adjx, adjy) = 1;
				q.push(pii(adjx, adjy));
			}
		}
	}

	dst = res.clone();
}

// 8.1 toán tử hình thái trên ảnh nhị phân
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
//----------------------------------------------------------------------------------//


//------- Toán tử hình thái học trên ảnh xám ----------------------------

void GrayScaleDilation(Mat &src, Mat &dst, int **kernel, int sz)
{
	Mat res = src.clone();

	for (int x = 0; x < src.rows - sz; ++x)
		for (int y = 0; y < src.cols - sz; ++y)
		{
			int max = 0;
			for (int i = 0; i < sz; ++i)
				for (int j = 0; j < sz; ++j)
					if (kernel[i][j] != -1)
					{
						int tmp = src.at<uchar>(x + i, y + j) + kernel[i][j];
						if (max < tmp)
							max = tmp;
					}
			if (max > 255)
				max = 255;
			res.at<uchar>(x , y) = max;
		}

	dst = res.clone();
}

void GrayScaleDilationLib(Mat &src, Mat &dst)
{
	dilate(src, dst, Mat());
}

void GrayScaleErosion(Mat &src, Mat &dst, int **kernel, int sz)
{
	Mat res = src.clone();

	for (int x = 0; x < src.rows - sz; ++x)
		for (int y = 0; y < src.cols - sz; ++y)
		{
			int min = 255;
			for (int i = 0; i < sz; ++i)
				for (int j = 0; j < sz; ++j)
					if (kernel[i][j] != -1)
					{
						int tmp = src.at<uchar>(x + i, y + j) - kernel[i][j];
						if (min > tmp)
							min = tmp;
					}
			if (min < 0)
				min = 0;
			res.at<uchar>(x, y) = min;
		}

	dst = res.clone();
}
void GrayScaleErosionLib(Mat &src, Mat &dst)
{
	erode(src, dst, Mat());
}

void GrayScaleOpening(Mat &src, Mat &dst, int **kernel, int sz)
{
	GrayScaleErosion(src, dst, kernel, sz);
	GrayScaleDilation(dst, dst, kernel, sz);
}

void GrayScaleOpeningLib(Mat &src, Mat &dst)
{
	morphologyEx(src, dst, MORPH_OPEN, Mat());
}

void GrayScaleClosing(Mat &src, Mat &dst, int **kernel, int sz)
{
	GrayScaleDilation(src, dst, kernel, sz);
	GrayScaleErosion(dst, dst, kernel, sz);
}

void GrayScaleClosingLib(Mat &src, Mat &dst)
{
	morphologyEx(src, dst, MORPH_CLOSE, Mat());
}

void GrayscaleSmoothing(Mat &src, Mat &dst, int **kernel, int sz)
{
	GrayScaleOpening(src, dst, kernel, sz);
	GrayScaleClosing(src, dst, kernel, sz);
}
void GrayscaleSmoothingLib(Mat &src, Mat &dst)
{
	GrayScaleOpeningLib(src, dst);
	GrayScaleClosingLib(src, dst);
}

void GrayscaleMorphologyGradient(Mat &src, Mat &dst, int** kernel, int sz)
{
	Mat dst1, dst2;
	GrayScaleDilation(src, dst1, kernel, sz);
	GrayScaleErosion(src, dst2, kernel, sz);
	dst = dst1 - dst2;
	normalize(dst, dst, 0, 255, NORM_MINMAX);
}

void GrayscaleMorphologyGradientLib(Mat &src, Mat &dst)
{
	morphologyEx(src, dst, MORPH_GRADIENT, Mat());
}

void TopHatTransformation(Mat &src, Mat &dst, int **kernel, int sz)
{
	Mat dst1;
	GrayScaleOpening(src, dst1, kernel, sz);
	dst = src - dst1;
	normalize(dst, dst, 0, 255, NORM_MINMAX);
}
void TopHatTransformationLib(Mat &src, Mat &dst)
{
	morphologyEx(src, dst, MORPH_TOPHAT, Mat());
}

void TexturalSegmentation(Mat &src, Mat &dst, int **kernel, int sz)
{
	GrayScaleClosing(src, dst, kernel, sz);
	GrayScaleOpening(dst, dst, kernel, sz);
}
void TexturalSegmentationLib(Mat &src, Mat &dst)
{
	GrayScaleClosingLib(src, dst);
	GrayScaleOpeningLib(dst, dst);
}