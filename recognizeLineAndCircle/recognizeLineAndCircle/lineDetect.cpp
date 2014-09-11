#include "lineDetect.h"

HoughLineDetect::HoughLineDetect(Mat image, int houghSpace, float thr)
{
	srcImage = image;
	width = image.cols;
	height = image.rows;
	centerX = width / 2;  
	centerY = height / 2;  
	hough_space = houghSpace;

	hough_interval = 2*3.1415926/hough_space;  
	//double threshold = 0.5;//调整阈值可以达到检测的线条数目
	threshold = thr;
	max = MAX(width, height);  
	max_length = (int)(sqrt(2.0) * max);  

	hough_2d = new int *[hough_space]; 
	for (int i =0;i<hough_space;i++)
	{
		hough_2d[i] = new int[2*max_length];//max and -max
		for (int j = 0;j<2*max_length;j++)
		{
			hough_2d[i][j] = 0;
		}
	}
	//for (int i = 0;i < hough_space;i++)
	//{

	//}
}
HoughLineDetect::~HoughLineDetect()
{
	//释放hough_2d
	for (int i = 0; i< hough_space; i++)
	{
		delete[] hough_2d[i];
	}
	delete[] hough_2d;
}
void HoughLineDetect::detectLine()
{
	for (int row = 0 ;row < srcImage.rows; row++)
	{
		for (int col = 0; col < srcImage.cols; col++)
		{
			int p = srcImage.at<uchar>(row,col) & 0xff;  
			if(p == 0) continue; // which means background color  
			for(int theta=0; theta < hough_space; theta++ ) 
			{  
				//max = (int)((col - centerX) * cos(cell * hough_interval) 
				//	+ (row - centerY) * sin(cell * hough_interval));  
				//max += max_length;   
				//为了让二维数组hough_2d的第二维可以从0变化到2*max_length,所以设置让col和row都
				//减去了width和height的1/2.
				int r = (int)((col - centerX) * cos(theta * hough_interval) 
					+ (row - centerY) * sin(theta * hough_interval));  
				r += max_length/2;   // start from zero, not (-max_length)  

				if (r < 0 || (r >= 2 * max_length)) 
				{
					continue;  
				}  
				hough_2d[theta][r] +=1;  
			}  
		}
	}

	// find the max hough value
	int max_hough = 0;
	for(int i=0; i<hough_space; i++) 
	{
		for(int j=0; j<2*max_length; j++) 
		{
			if(hough_2d[i][j] > max_hough) 
			{
				max_hough = hough_2d[i][j];
			}
		}
	}

	// transfer back to image pixels space from hough parameter space
	int hough_threshold = (int)(threshold * max_hough);

	for(int theta = 0; theta < hough_space; theta++) 
	{
		for(int r = 0; r < 2*max_length; r++) 
		{
			if(hough_2d[theta][r] < hough_threshold) // discard it
				continue;
			int hough_value = hough_2d[theta][r];
			bool isLine = true;
			for(int i=-1; i<2; i++) 
			{//为了补偿sqrt(2.0)带来的max_length向下取整导致的误差
				for(int j=-1; j<2; j++) 
				{
					if(i != 0 || j != 0) 
					{
						int yf = theta + i;
						int xf = r + j;
						if(xf < 0) continue;
						if(xf < 2*max_length) 
						{
							if (yf < 0) 
							{
								yf += hough_space;
							}
							if (yf >= hough_space) 
							{
								yf -= hough_space;
							}
							if(hough_2d[yf][xf] <= hough_value) 
							{
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
			double siny = sin(theta * hough_interval);
			double cosx = cos(theta * hough_interval);
			if ((theta <= hough_space / 4) || (theta >= 3 * hough_space / 4)) 
			{
				for (int subrow = 0; subrow < height; ++subrow) 
				{
					int subcol = (int)((r - max_length/2 - ((subrow - centerY) * siny)) / cosx) + centerX;
					if ((subcol < width) && (subcol >= 0)) 
					{
						srcImage.at<uchar>(subrow,subcol)= 255;
					}
				}
			} 
			else 
			{
				for (int subcol = 0; subcol < width; ++subcol) 
				{
					int subrow = (int)((r - max_length/2 - ((subcol - centerX) * cosx)) / siny) + centerY;
					if ((subrow < height) && (subrow >= 0)) 
					{
						srcImage.at<uchar>(subrow,subcol) = 255;
					}
				}
			}
		}
	}
}