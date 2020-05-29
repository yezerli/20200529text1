#include<iostream>
#include<opencv.hpp>

using namespace cv;
using namespace std;

//HOG函数
float *HOG(Mat refMat) {
	int blockSize = 16;
	int nX = refMat.cols / blockSize;
	int nY = refMat.rows / blockSize;
	Mat gx, gy;
	Mat mag, angle;
	int cellnums = nX * nY;	//cell总数
	int bins = cellnums * 8;
	Sobel(refMat, gx, CV_32F, 1, 0, 1);
	Sobel(refMat, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	//定义存储数组
	float* ref_hist = new float[bins];
	memset(ref_hist, 0, sizeof(float) * bins);
	int binnumber = 0;
	for (int j = 0; j < nY; j++) {
		for (int i = 0; i < nX; i++) {
			for (int a = j * blockSize; a < (j + 1)*blockSize; a++) {
				for (int b = i * blockSize; b < (i + 1)*blockSize; b++) {
					int angle1 = angle.at<float>(a, b)/45;	//像素的角度值
					float mag1 = mag.at<float>(a, b);		//像素的幅度值
					ref_hist[angle1 + binnumber * 8] += mag1;				//在数组的角度位置增加幅度值
				}
			}
			binnumber++;//令循环开始下一个cell的操作
		}
	}
	return ref_hist;
}

int main() {
	Mat src = imread("E://Git//template.png");
	Mat img = imread("E://Git//img.png");

	float dis1 = 0;
	float dis = 0;
	Rect rect;
	float *hist;
	hist = HOG(src);
	float *hist0;
	int x = src.rows;
	int y = src.cols;
	int a = img.rows;
	int b = img.cols;

	for (int i = 0; i < b - y; i++)
	{
		for (int j = 0; j < a - x; j++)
		{
			Mat ROI = img(Rect(i, j, y, x));
			hist0 = HOG(ROI);
			for (int n = 0; n < 16; n++)
			{
				dis1 += (hist[n] - hist0[n]) * (hist[n] - hist0[n]);
			}
			dis1 = sqrt(dis1);

			if (i == 0 & j == 0)
			{dis = dis1;}
			if (dis1 < dis)
			{
				dis = dis1;
				rect = Rect(i, j, y, x);
			}
		}
	}
	rectangle(img, rect, CV_RGB(255, 0, 0), 1, 8, 0);
	imshow("result", img);
	waitKey(0);
	return 0;
}
