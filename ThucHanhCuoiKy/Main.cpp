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
	//Tên của file ảnh và video
	char *imageName = "apple.jpg";
	Mat empty;
	imageGray = imread(imageName, CV_LOAD_IMAGE_GRAYSCALE);
	imageColor = imread(imageName, CV_LOAD_IMAGE_COLOR);
	Mat imageBinary;
	GrayscaletoBinary(imageGray, imageBinary);
	if (!imageGray.data||!imageColor.data)
	{
		std::cout << "No data!";
		return -1;
	}

	waitKey(0);
	do
	{
		showMenu();
		cin >> task;
		switch (task)
		{
		case 0:
			return 0;
		case 1:
			FFT(imageGray, imageResCoding);
			showImageResult(empty, imageResCoding);
			break;
		case 2:
			inverseFFT(imageGray, imageResCoding);
			showImageResult(empty, imageResCoding);
			break;
		case 3:
			cout << "Developing!";
			getch();
			break;
		case 4:
			cout << "Developing!";
			getch();
			break;
		case 5:
			cout << "Developing!";
			getch();
			break;
		case 6:
			extractFaceFeature();
			break;
		case 7:
			RegionGrowingSegmentation(imageColor, imageResCoding, 400, 400, 50);
			showImageResult(empty, imageResCoding);
			break;
		case 8:
			KMeansLib(imageColor, imageResLib);
			Kmeans(imageColor, imageResCoding);
			showImageResult(imageResLib, imageResCoding);
			break;
		case 9:
		{
			int f = showMenuFunc9();
			int **kernel = newKernel();
			switch (f)
			{
			case 1:
				BinaryDilation(imageBinary, imageResCoding, kernel, 3, 1, 1);
				BinaryDilationLib(imageBinary, imageResLib);
				showImageResult(imageResLib, imageResCoding);
				break;
			case 2:
				BinaryErosion(imageBinary, imageResCoding, kernel, 3, 1, 1);
				BinaryErosionLib(imageBinary, imageResLib);
				showImageResult(imageResLib, imageResCoding);
				break;
			case 3:
				BinaryOpening(imageBinary, imageResCoding, kernel, 3, 1, 1);
				BinaryOpeningLib(imageBinary, imageResLib);
				showImageResult(imageResLib, imageResCoding);
				break;
			case 4:
				BinaryClosing(imageBinary, imageResCoding, kernel, 3, 1, 1);
				BinaryClosingLib(imageBinary, imageResLib);
				showImageResult(imageResLib, imageResCoding);
				break;
			case 5:
				BinaryBoundaryExtraction(imageBinary, imageResCoding);
				showImageResult(empty, imageResCoding);
				break;
			case 6:
			{
				Mat imageFill;
				imageFill = imread("heart.jpg", CV_LOAD_IMAGE_GRAYSCALE);
				GrayscaletoBinary(imageFill, imageFill);
				BinaryRegionFilling(imageFill, imageResCoding, 130, 130);
				imshow("Before", imageFill);
				imshow("After", imageResCoding);
				break;
			}
			case 7:
				repairKernel(kernel);
				BinaryHitOrMiss(imageBinary, imageResCoding, kernel, 3, 1, 1);
				showImageResult(empty, imageResCoding);
				break;
			case 8:
				repairKernel(kernel);
				BinaryThinning(imageBinary, imageResCoding, kernel, 3, 1, 1);
				for (int i = 0; i < 3; i++)
					BinaryThinning(imageResCoding, imageResCoding, kernel, 3, 1, 1);
				showImageResult(empty, imageResCoding);
				break;
			case 9:
				repairKernel(kernel);
				BinaryThickening(imageBinary, imageResCoding, kernel, 3, 1, 2);
				for (int i = 0; i < 3; i++)
					BinaryThickening(imageResCoding, imageResCoding, kernel, 3, 1, 1);
				showImageResult(empty, imageResCoding);
				break;
				
			}
			deleteKernel(kernel);
			break;
		}
		case 10:
			cout << "Developing!";
			getch();
			break;
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

int** newKernel()
{
	int **kernel = new int*[3];
	for (int i = 0; i < 3; i++)
	{
		kernel[i] = new int[3];
		for (int j = 0; j < 3; j++)
			kernel[i][j] = 1;
	}
	return kernel;
}

void repairKernel(int** kernel)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] = -1;
	kernel[1][1] = 1;
	kernel[1][2] = 0;
}

void deleteKernel(int **kernel)
{
	for (int i = 0; i < 3; i++)
		delete kernel[i];
	delete kernel;
}