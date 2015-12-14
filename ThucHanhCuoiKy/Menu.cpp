#include <iostream>
#include <stdlib.h>
using namespace  std;

void showMenu()
{
	system("cls");
	cout << "-------------------MENU---------------------" << endl
		<< "Nhom tac vu 5 - Phep bien doi Fourier:" << endl
		<< "\t1. Phep bien doi Fourier thuan FFT" << endl
		<< "\t2. Phep bien doi Fourier nghich FFT" << endl
		<< "\t3. Loc thong tan so thap bang phep bien doi Fourier va DL Convolution" << endl
		<< "\t4. Loc thong tan so cao bang phep bien doi Fourier va DL Convolution" << endl << endl
		<< "Nhom tac vu 6 - Phep bien doi Karhunen Loeve:" << endl
		<< "\t5. Phan tich thanh phan chinh" << endl
		<< "\t6. Rut trich dat trung cho tap mat nguoi dua vao phan tich thanh phan chinh" << endl << endl
		<< "Nhom tac vu 7 - Phan doan anh:" << endl
		<< "\t7. Region growing" << endl
		<< "\t8. K-means" << endl << endl
		<< "Nhom tac vu 8 - Toan tu hinh thai hoc:" << endl
		<< "\t9. Toan tu hinh thai hoc tren anh nhi phan" << endl
		<< "\t10. Toan tu hinh thai hoc tren anh xam" << endl << endl
		<< "Vui long cho tac vu - go 0 de thoat" << endl;
}

//int showMenuFunc4()
//{
//	system("cls");
//	int f;
//	cout << "-----CHUYEN DOI RGB -> HSV VA NGUOC LAI-----" << endl
//		<< "\t1. RGB->HSV" << endl
//		<< "\t2. HSV->RGB" << endl;
//	cin >> f;
//	return f;
//}
//
//int showMenuFunc5()
//{
//	system("cls");
//	int f;
//	cout << "-----CHUYEN DOI RGB -> LUV, Lab, YcbCr-----" << endl
//		<< "\t1. RGB->LUV" << endl
//		<< "\t2. LUV->RGB" << endl
//		<< "\t3. RGB->Lab" << endl
//		<< "\t4. Lab->RGB" << endl
//		<< "\t5. RGB->YcbCr" << endl
//		<< "\t6. YcbCr->RGB" << endl;
//	cin >> f;
//	return f;
//}
//
//int showMenuFunc6()
//{
//	system("cls");
//	int f;
//	cout << "-----HIEU CHINH MAU SAC MO HINH TUYEN TINH-----" << endl
//		<< "\t1. Dieu chinh do sang" << endl
//		<< "\t2. Dieu chinh do tuong phan" << endl
//		<< "\t3. Dieu chinh do sang va tuong phan" << endl;
//	cin >> f;
//	return f;
//}
//
//int showMenuFunc7()
//{
//	system("cls");
//	int f;
//	cout << "-----HIEU CHINH MAU SAC MO HINH PHI TUYEN-----" << endl
//		<< "\t1. Dieu chinh mau sac theo ham logarit" << endl
//		<< "\t2. Dieu chinh mau sac theo ham mu" << endl;
//	cin >> f;
//	return f;
//}
//
//int showMenuFunc11()
//{
//	system("cls");
//	int f;
//	cout << "-----CAC TOAN TU LAM TRON ANH-----" << endl
//		<< "\t1. Toan tu trung binh" << endl
//		<< "\t2. Toan tu Gauss" << endl
//		<< "\t3. Toan tu trung vi" << endl;
//	cin >> f;
//	return f;
//}
//
//int showMenuFunc12()
//{
//	system("cls");
//	int f;
//	cout << "-----CAC TOAN TU PHAT HIEN BIEN CANH-----" << endl
//		<< "\t1. Toan tu LAPLACE" << endl
//		<< "\t2. Toan tu SOBEL" << endl;
//	cin >> f;
//	return f;
//}