#include <iostream>
#include "../3rdparty/include/opencv.hpp"
#include<math.h>

using namespace std;
using namespace cv;

#pragma comment(lib, "opencv_highgui249d.lib")
#pragma comment(lib, "opencv_imgproc249d.lib")
#pragma comment(lib, "opencv_core249d.lib")
#pragma comment(lib, "opencv_highgui249.lib")
#pragma comment(lib, "opencv_imgproc249.lib")
#pragma comment(lib, "opencv_core249.lib")
int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "hello.jpg";
	Mat src = imread(filename,0);
	if(src.empty())
	{
		cout << "can not open " << filename << endl;
		return -1;
	}
	Mat dst, cdst;
	Canny(src, dst, 50, 200, 3);

	//cvtColor(dst, cdst, CV_GRAY2BGR);

	int width = dst.cols;
	int height = dst.rows;
	int centerX = width / 2;  
	int centerY = height / 2;  
	int hough_space = 500;
	double hough_interval = 3.1415926/hough_space;  
	double threshold = 0.8;

	int max = MAX(width, height);  
	int max_length = (int)(sqrt(2.0) * max);  
	int **hough_2d;//[500][max] = {0};
	hough_2d = new int *[hough_space]; 
	for (int i =0;i<hough_space;i++)
	{
		hough_2d[i] = new int[2*max_length];
	}

	for (int i = 0;i < hough_space;i++)
	{
		for (int j = 0;j<2*max_length;j++)
		{
			hough_2d[i][j] = 0;
		}
	}
	//hough_1d = new int[2 * hough_space * max_length];  

	for (int row = 0 ;row < dst.rows; row++)
	{
		for (int col = 0; col < dst.cols; col++)
		{
			int p = dst.at<uchar>(row,col) & 0xff;  
			if(p == 0) continue; // which means background color  
			for(int cell=0; cell < hough_space; cell++ ) 
			{  
				max = (int)((col - centerX) * cos(cell * hough_interval) 
					+ (row - centerY) * sin(cell * hough_interval));  
				max += max_length;   
				if (max < 0 || (max >= 2 * max_length)) 
				{
					continue;  
				}  
				hough_2d[cell][max] +=1;  
			}  
		}
	}

	// find the max hough value
	int max_hough = 0;
	for(int i=0; i<hough_space; i++) {
		for(int j=0; j<2*max_length; j++) {
			if(hough_2d[i][j] > max_hough) {
				max_hough = hough_2d[i][j];
			}
		}
	}
	// transfer back to image pixels space from hough parameter space
	int hough_threshold = (int)(threshold * max_hough);

	for(int row = 0; row < hough_space; row++) {
		for(int col = 0; col < 2*max_length; col++) {
			if(hough_2d[row][col] < hough_threshold) // discard it
				continue;
			int hough_value = hough_2d[row][col];
			bool isLine = true;
			for(int i=-1; i<2; i++) {
				for(int j=-1; j<2; j++) {
					if(i != 0 || j != 0) {
						int yf = row + i;
						int xf = col + j;
						if(xf < 0) continue;
						if(xf < 2*max_length) {
							if (yf < 0) {
								yf += hough_space;
							}
							if (yf >= hough_space) {
								yf -= hough_space;
							}
							if(hough_2d[yf][xf] <= hough_value) {
								continue;
							}
							isLine = false;
							break;
						}
					}
				}
			}
			if(!isLine) continue;

			// transform back to pixel data now...
			double dy = sin(row * hough_interval);
			double dx = cos(row * hough_interval);
			if ((row <= hough_space / 4) || (row >= 3 * hough_space / 4)) {
				for (int subrow = 0; subrow < height; ++subrow) {
					int subcol = (int)((col - max_length - ((subrow - centerY) * dy)) / dx) + centerX;
					if ((subcol < width) && (subcol >= 0)) {
						image_2d[subrow][subcol] = -16776961;
					}
				}
			} else {
				for (int subcol = 0; subcol < width; ++subcol) {
					int subrow = (int)((col - max_length - ((subcol - centerX) * dx)) / dy) + centerY;
					if ((subrow < height) && (subrow >= 0)) {
						image_2d[subrow][subcol] = -16776961;
					}
				}
			}
		}
	}
	namedWindow("hello");
	imshow("hello",dst);
	
	waitKey(0);
	return 0;
}