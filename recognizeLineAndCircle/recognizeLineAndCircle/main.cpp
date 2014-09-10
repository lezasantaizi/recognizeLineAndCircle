#include "lineDetect.h"

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
	namedWindow("hello1");
	imshow("hello1",dst);
	//cvtColor(dst, cdst, CV_GRAY2BGR);
	HoughLineDetect lineDetect(dst,100,0.5) ;
	lineDetect.detectLine();

	namedWindow("hello");
	imshow("hello",dst);
	
	waitKey(0);
	return 0;
}