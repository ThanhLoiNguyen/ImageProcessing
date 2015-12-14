#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "ImgProcFuncs.h"
#include "Menu.h"
#include "HiepITLib.h"

#include <iostream>
using namespace std;
void showImageResult(Mat &imageResLib, Mat &imageResCoding);

int main()
{
	int task = 0, f = 0;
	Mat imageGray, imageColor, imageResLib, imageResCoding; //Ảnh gốc, ảnh chạy hàm Thư viện và hàm Tự code
	//Tên của file ảnh và video
	char *imageName = "apple.jpg";

	imageGray = imread(imageName, CV_LOAD_IMAGE_GRAYSCALE);
	imageColor = imread(imageName, CV_LOAD_IMAGE_COLOR);
	if (!imageGray.data||!imageColor.data)
	{
		std::cout << "No data!";
		return -1;
	}

	waitKey(0);
	//do
	//{
	//	showMenu();
	//	cin >> task;
	//	switch (task)
	//	{
	//	case 0:
	//		return 0;
	//	case 5:
	//		
	//		break;
	//	case 6:
	//		
	//		break;
	//	case 7:
	//		
	//		break;
	//	case 8:
	//		;
	//		break;
	//	/*case 5:
	//		f = showMenuFunc5();
	//		switch (f)
	//		{
	//		case 1:
	//			
	//			break;
	//		case 2:

	//			break;
	//		case 3:
	//		
	//			break;
	//		case 4:
	//			
	//			break;
	//		case 5:
	//			
	//			break;
	//		case 6:
	//			
	//			break;
	//		}
	//		showImageResult(imageResLib, imageResCoding);
	//		break;
	//	case 6:
	//		double dBrightness, dContrast;
	//		f = showMenuFunc6();
	//		switch (f)
	//		{
	//		case 1:
	//			
	//			break;
	//		case 2:
	//			
	//			break;
	//		case 3:
	//			
	//			break;
	//		}
	//		showImageResult(imageResLib, imageResCoding);
	//		
	//		break;
	//	case 7:
	//		f = showMenuFunc7();
	//		
	//		if (f == 1)
	//		{
	//			ch
	//		}
	//		else if (f == 2)
	//		{
	//			
	//		}
	//		
	//		break;
	//	case 8:
	//		
	//		
	//		break;
	//	case 9:
	//		
	//		
	//		break;
	//	case 10:
	//		;
	//		break;
	//	case 11:
	//		f = showMenuFunc11();

	//		showImageResult(imageResLib, imageResCoding);
	//		break;
	//	case 12:
	//		f = showMenuFunc12();
	//		if (f == 1)

	//		showImageResult(imageResLib, imageResCoding);
	//		break;*/
	//	default:
	//		cout << "Chon sai chuc nang, vui long chon lai" << endl;
	//	}

	//	waitKey(0);
	//} while (task);
	return 0;
}

void showImageResult(Mat &imageResLib, Mat &imageResCoding)
{
	imshow("Library", imageResLib);
	imshow("Coding", imageResCoding);
}