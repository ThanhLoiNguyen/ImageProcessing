#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "ImgProcFuncs.h"
#include "Menu.h"

#include <iostream>
#include <conio.h>
using namespace std;
void showImageResult(Mat &imageResLib, Mat &imageResCoding);


int main()
{
	int task = 0, f = 0;
	Mat imageGray, imageColor, imageResLib, imageResCoding; //Ảnh gốc, ảnh chạy hàm Thư viện và hàm Tự code
	//Tên của file ảnh
	char *imageName = "apple.jpg";
	imageGray = imread(imageName, CV_LOAD_IMAGE_GRAYSCALE);
	imageColor = imread(imageName, CV_LOAD_IMAGE_COLOR);
	Mat imageBinary, empty;
	GrayscaletoBinary(imageGray, imageBinary);
	if (!imageGray.data||!imageColor.data)
	{
		std::cout << "No data!";
		return -1;
	}
	do
	{
		showMenu();
		cin >> task;
		switch (task)
		{
		case 0:
			return 0;
		case 1: //FFT thuận
			FFT(imageGray, imageResCoding);
			FFTLib(imageResCoding, imageResLib);
			showImageResult(imageResLib, imageResCoding);
			break;
		case 2: //FFT nghịch
			inverseFFT(imageGray, imageResCoding);
			inverseFFTLib(imageGray, imageResLib);
			showImageResult(imageResLib, empty);
			break;
		case 3: //Lọc tần thấp
		{ 
			int f;
			do {
				f = showMenuFunc3();
				if (f == 0) break;
				switch (f)
				{
				case 1:
					IdealLowpassFilter(imageGray, imageResCoding, 0.1);
					break;
				case 2:
					ButterworthLowpassFilter(imageGray, imageResCoding, 0.1);
					break;
				case 3:
					GaussianLowpassFilter(imageGray, imageResCoding, 0.1);
					break;
				}
				showImageResult(empty, imageResCoding);
				waitKey();
			} while (1);
			break;
		}
		case 4: //Lọc tần cao
		{
			int f;
			do {
				f = showMenuFunc4();
				if (f == 0) break;
				switch (f)
				{
				case 1:
					IdealHighpassFilter(imageGray, imageResCoding, 0.1);
					break;
				case 2:
					ButterworthHighpassFilter(imageGray, imageResCoding, 0.1);
					break;
				case 3:
					GaussianHighpassFilter(imageGray, imageResCoding, 0.1);
					break;
				}
				showImageResult(empty, imageResCoding);
				waitKey();
			} while (1);
			break;
		}
		case 5: //Rút trích đặt trưng (chung với case 6)
		case 6: //Rút đặt trưng mặt người
			extractFaceFeature();
			break;
		case 7: //RegionGrowing
			RegionGrowingSegmentation(imageColor, imageResCoding, 400, 400, 50);
			showImageResult(empty, imageResCoding);
			break;
		case 8: //K-mean
			KMeansLib(imageColor, imageResLib);
			Kmeans(imageColor, imageResCoding);
			showImageResult(imageResLib, imageResCoding);
			break;
		case 9: //hình thái học trên ảnh nhị phân
		{
			int **kernel = newKernel(3);
			while (1)
			{
				int f = showMenuFunc9();
				if (f == 0) break;
				switch (f)
				{
				case 1: //Dãn
					BinaryDilation(imageBinary, imageResCoding, kernel, 3, 1, 1);
					BinaryDilationLib(imageBinary, imageResLib);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 2: //Co
					BinaryErosion(imageBinary, imageResCoding, kernel, 3, 1, 1);
					BinaryErosionLib(imageBinary, imageResLib);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 3: //Mở
					BinaryOpening(imageBinary, imageResCoding, kernel, 3, 1, 1);
					BinaryOpeningLib(imageBinary, imageResLib);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 4: //Đóng
					BinaryClosing(imageBinary, imageResCoding, kernel, 3, 1, 1);
					BinaryClosingLib(imageBinary, imageResLib);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 5: //BoundaryExtraction
					BinaryBoundaryExtraction(imageBinary, imageResCoding);
					showImageResult(empty, imageResCoding);
					break;
				case 6: //RegionFilling
				{
					Mat imageFill;
					imageFill = imread("heart.jpg", CV_LOAD_IMAGE_GRAYSCALE);
					GrayscaletoBinary(imageFill, imageFill);
					BinaryRegionFilling(imageFill, imageResCoding, 130, 130);
					imshow("Before", imageFill);
					imshow("After", imageResCoding);
					break;
				}
				case 7: //HitOrMiss
					repairKernel(kernel);
					BinaryHitOrMiss(imageBinary, imageResCoding, kernel, 3, 1, 1);
					showImageResult(empty, imageResCoding);
					break;
				case 8: //Thinning
					repairKernel(kernel);
					BinaryThinning(imageBinary, imageResCoding, kernel, 3, 1, 1);
					for (int i = 0; i < 3; i++)
						BinaryThinning(imageResCoding, imageResCoding, kernel, 3, 1, 1);
					showImageResult(empty, imageResCoding);
					break;
				case 9: //Thickening
					repairKernel(kernel);
					BinaryThickening(imageBinary, imageResCoding, kernel, 3, 1, 2);
					for (int i = 0; i < 3; i++)
						BinaryThickening(imageResCoding, imageResCoding, kernel, 3, 1, 1);
					showImageResult(empty, imageResCoding);
					break;
				}
				waitKey();
			}
			deleteKernel(kernel);
			break;
		}
		case 10:// Toán tử hình thái học ảnh xám
		{
			int **kernel = newKernel(3);
			for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 3; ++j)
					kernel[i][j] = 10;
			while (1)
			{
				int f = showMenuFunc10();
				if (f == 0) break;
				switch (f)
				{
				case 1: //Dãn nở độ sáng
					GrayScaleDilationLib(imageGray, imageResLib);
					GrayScaleDilation(imageGray, imageResCoding, kernel, 3);
					imshow("Original", imageGray);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 2: //Co độ sáng
					GrayScaleErosionLib(imageGray, imageResLib);
					GrayScaleErosion(imageGray, imageResCoding, kernel, 3);
					imshow("Original", imageGray);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 3: //Mở độ sáng
					GrayScaleOpeningLib(imageGray, imageResLib);
					GrayScaleOpening(imageGray, imageResCoding, kernel, 3);
					imshow("Original", imageGray);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 4: //Đóng độ sáng
					GrayScaleClosingLib(imageGray, imageResLib);
					GrayScaleClosing(imageGray, imageResCoding, kernel, 3);
					imshow("Original", imageGray);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 5: //Làm trơn
					GrayscaleSmoothingLib(imageGray, imageResLib);
					GrayscaleSmoothing(imageGray, imageResCoding, kernel, 3);
					imshow("Original", imageGray);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 6: //Toán tử Gradient
					GrayscaleMorphologyGradient(imageGray, imageResCoding, kernel, 3);
					GrayscaleMorphologyGradientLib(imageGray, imageResLib);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 7: //Toán tử đỉnh nón
					TopHatTransformation(imageGray, imageResCoding, kernel, 3);
					TopHatTransformationLib(imageGray, imageResLib);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 8: //Toán tử textual segmentation
					TexturalSegmentation(imageGray, imageResCoding, kernel, 3);
					TexturalSegmentationLib(imageGray, imageResLib);
					showImageResult(imageResLib, imageResCoding);
					break;
				case 9: //Granulometry
					Granulometry(imageGray, imageResCoding, 50, 100);
					showImageResult(imageGray, imageResCoding);
					break;
				}
				waitKey();
			}
			deleteKernel(kernel);
		}
		default:
			cout << "Chon sai chuc nang, vui long chon lai" << endl;
		}
		waitKey(0);
	} while (task);
	return 0;
}

void showImageResult(Mat &imageResLib, Mat &imageResCoding)
{
	if (!imageResLib.empty())
		imshow("Library", imageResLib);
	if (!imageResCoding.empty())
		imshow("Coding", imageResCoding);
}