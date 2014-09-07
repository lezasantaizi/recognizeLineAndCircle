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

	int max = MAX(width, height);  
	int max_length = (int)(sqrt(2.0) * max);  
	int **hough_2d;//[500][max] = {0};
	for (int i =0;i<hough_interval;i++)
	{

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
		cout <<endl;
	}
	namedWindow("hello");
	imshow("hello",dst);
	
	waitKey(0);
	return 0;
}