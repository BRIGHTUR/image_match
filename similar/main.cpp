#include<iostream>
#include<opencv2/opencv.hpp>
#include<math.h>
using namespace std;
using namespace cv;
int main()
{
	Mat refer = imread("refer.jpg",0);
	Mat test1 = imread("test1.jpg", 0);
	Mat test2 = imread("test2.jpg", 0);
	int count = 0;
	int temp = 0;
	int res1 = 0;
	int res2 = 0;
	refer.convertTo(refer, CV_32F, 1 / 255.0);
	test1.convertTo(test1, CV_32F, 1 / 255.0);
	test2.convertTo(test2, CV_32F, 1 / 255.0);
	int cellsize = 16;
	int nx = refer.cols / cellsize;
	int ny = refer.rows / cellsize;
	int bins = nx*ny * 8;
	float his[720] = {0};
	float his1[720] = { 0 };
	float his2[720] = { 0 };
	//参考图像处理
	Mat gx, gy;
	Mat mag, angle;
	Mat cell=Mat(Size(16,16),CV_32F);
	Mat cell1 = Mat(Size(16, 16), CV_32F);
	Sobel(refer, gx, CV_32F, 1, 0, 1);
	Sobel(refer, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	for (int j = 0; j < ny; j++) {
		for (int i = 0; i < nx; i++) {
			cell = mag(Rect(16 * i, 16 * j, 16, 16)).clone();
			cell1 = angle(Rect(16 * i, 16 * j, 16, 16)).clone();
			for (int m = 0; m < 16; m++) {
				for (int n = 0; n < 16; n++) {
					temp = cell1.at<float>(n, m) / 45;
					his[count * 8 + temp] = his[count * 8 + temp] + cell.at<float>(n, m);
				}
			}
			count = count + 1;
		}
	}
	//第一幅图像处理
	count = 0;
	temp = 0;
	Mat gx1, gy1;
	Mat mag1, angle1;
	Mat cell2 = Mat(Size(16, 16), CV_32F);
	Mat cell3 = Mat(Size(16, 16), CV_32F);
	Sobel(test1, gx1, CV_32F, 1, 0, 1);
	Sobel(test1, gy1, CV_32F, 0, 1, 1);
	cartToPolar(gx1, gy1, mag1, angle1, true);
	for (int j = 0; j < ny; j++) {
		for (int i = 0; i < nx; i++) {
			cell2 = mag1(Rect(16 * i, 16 * j, 16, 16)).clone();
			cell3 = angle1(Rect(16 * i, 16 * j, 16, 16)).clone();
			for (int m = 0; m < 16; m++) {
				for (int n = 0; n < 16; n++) {
					temp = cell3.at<float>(n, m) / 45;
					his1[count * 8 + temp] = his1[count * 8 + temp] + cell2.at<float>(n, m);
				}
			}
			count = count + 1;
		}
	}
	//第三幅图像处理
	count = 0;
	temp = 0;
	Mat gx2, gy2;
	Mat mag2, angle2;
	Mat cell4 = Mat(Size(16, 16), CV_32F);
	Mat cell5 = Mat(Size(16, 16), CV_32F);
	Sobel(test2, gx2, CV_32F, 1, 0, 1);
	Sobel(test2, gy2, CV_32F, 0, 1, 1);
	cartToPolar(gx2, gy2, mag2, angle2, true);
	for (int j = 0; j < ny; j++) {
		for (int i = 0; i < nx; i++) {
			cell4 = mag2(Rect(16 * i, 16 * j, 16, 16)).clone();
			cell5 = angle2(Rect(16 * i, 16 * j, 16, 16)).clone();
			for (int m = 0; m < 16; m++) {
				for (int n = 0; n < 16; n++) {
					temp = cell5.at<float>(n, m) / 45;
					his2[count * 8 + temp] = his2[count * 8 + temp] + cell4.at<float>(n, m);
				}
			}
			count = count + 1;
		}
	}
	//对比
	for (int i = 0; i < 720; i++) {
		res1 = res1 + (his[i] - his1[i])*(his[i] - his1[i]);
		res2 = res2 + (his[i] - his2[i])*(his[i] - his2[i]);
	}
	res1 = sqrt(res1);
	res2 = sqrt(res2);
	cout << res1 << endl;
	cout << res2 << endl;
	if (res1 < res2) {
		cout << "图片一与参考图片匹配度较高" << endl;
	}
	else {
		cout << "图片二与参考图片匹配度较高" << endl;
	}
	waitKey(0);
	system("pause");
}