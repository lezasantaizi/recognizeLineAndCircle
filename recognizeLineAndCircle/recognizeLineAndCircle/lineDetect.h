#ifndef  LINE_DETECT	
#define LINE_DETECT
#include <iostream>
#include "../3rdparty/include/opencv.hpp"
#include<math.h>
using namespace std;
using namespace cv;

class HoughLineDetect
{
public:
	HoughLineDetect(Mat image, int houghSpace = 500, float thr = 0.5);
	~HoughLineDetect();
	void detectLine();

	private:
	Mat srcImage;
	int width;
	int height;
	int centerX;
	int centerY;
	int hough_space;
	float hough_interval;
	float threshold;
	int max;
	int max_length ;
	int **hough_2d;//[500][max] = {0};

};
#endif