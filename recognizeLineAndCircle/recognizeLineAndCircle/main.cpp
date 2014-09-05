#include <iostream>
#include "../3rdparty/include/opencv.hpp"


using namespace std;
using namespace cv;

#pragma comment(lib, "opencv_highgui249d.lib")
#pragma comment(lib, "opencv_imgproc249d.lib")
#pragma comment(lib, "opencv_core249d.lib")
#pragma comment(lib, "opencv_highgui249.lib")
#pragma comment(lib, "opencv_imgproc249.lib")
#pragma comment(lib, "opencv_core249.lib")
int main()
{
	Mat temp = imread("hello.jpg");
	namedWindow("hello");
	imshow("hello",temp);
	waitKey(0);
	return 0;
}