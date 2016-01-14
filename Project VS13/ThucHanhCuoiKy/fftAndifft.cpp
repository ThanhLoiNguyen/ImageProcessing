//Do có xung đột namespace nên hàm này phải để riêng
#include <complex>
#include <iostream>
#include <valarray>

const double PI = 3.141592653589793238460;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
// Cooley–Tukey FFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive
void fft(CArray& x)
{
	const size_t N = x.size();
	if (N <= 1) return;

	// divide
	CArray even = x[std::slice(0, N / 2, 2)];
	CArray  odd = x[std::slice(1, N / 2, 2)];

	// conquer
	fft(even);
	fft(odd);

	// combine
	for (size_t k = 0; k < N / 2; ++k)
	{
		Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k + N / 2] = even[k] - t;
	}
}

void ifft(CArray& x)
{
	// conjugate the complex numbers
	x = x.apply(std::conj);

	// forward fft
	fft(x);

	// conjugate the complex numbers again
	x = x.apply(std::conj);

	// scale the numbers
	x /= x.size();
}

void MatToCArray(cv::Mat src, CArray &arr)
{
	arr.resize(src.rows*src.cols);
	int n = 0;
	for (int i = 0; i < src.rows; ++i)
		for (int j = 0; j < src.cols; ++j)
		{
			arr[n] = src.at<uchar>(i, j);
			n++;
		}
}

void CArrayToMat(CArray arr, cv::Mat &dst, int row, int col)
{
	if (row != -1)
	{
		for (int i = 0; i < dst.cols; ++i)
		{
			int k = *arr[i]._Val;
			if (k > 255) k = 255;
			if (k < 0) k = 0;
			dst.at<uchar>(row, i) = k;
		}
			
	}
	else if (col != -1)
	{
		for (int i = 0; i < dst.rows; ++i)
		{
			int k = *arr[i]._Val;
			if (k > 255) k = 255;
			if (k < 0) k = 0;
			dst.at<uchar>(i, col) = k;
		}
	}
}

void FFT(cv::Mat &src, cv::Mat &dst)
{
	cv::Mat res = src.clone();
	CArray a;
	for (int i = 0; i < src.rows; ++i)
	{
		MatToCArray(src.row(i), a);
		fft(a);
		CArrayToMat(a, res, i, -1);
	}

	for (int i = 0; i < src.cols; ++i)
	{
		MatToCArray(src.col(i), a);
		fft(a);
		CArrayToMat(a, res, -1, i);
	}
	dst = res.clone();
}

void inverseFFT(cv::Mat &src, cv::Mat &dst)
{
	cv::Mat res = src.clone();
	CArray a;
	for (int i = 0; i < src.rows; ++i)
	{
		MatToCArray(src.row(i), a);
		ifft(a);
		CArrayToMat(a, res, i, -1);
	}

	for (int i = 0; i < src.cols; ++i)
	{
		MatToCArray(src.col(i), a);
		ifft(a);
		CArrayToMat(a, res, -1, i);
	}
	dst = res.clone();
}