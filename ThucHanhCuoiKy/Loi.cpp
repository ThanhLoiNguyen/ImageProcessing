#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
using namespace cv;

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