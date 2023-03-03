#include "colorhistogram.h"
#include <opencv2/imgproc.hpp>

// CSS 487; Professor:	Clark Olson
// colorhistogram.cpp:	implementation file of the colorhistogram.h which include:
//	ColorHistogram:		default constructor that reads in two images foreground and background
//	createistogram:		construct the color historgram matrix for the foreground image
//	findMostCommon:		get the most common color from the color histogram
//	overlay:            apply color histogram on the two images
//	useOpenCV:			apply 4 openCV functions based on requirements
//	exploreOpenCV:		play around with 3 more openCV methods
// Author:				Nguyen Vi Cao

// default constructor
// Preconditions:	create 1 image objects
ColorHistogram::ColorHistogram(const Mat& back)
{
    this->back_ = back;
}

// default constructor
// Preconditions:	create 2 image objects
ColorHistogram::ColorHistogram(const Mat& fore, const Mat& back)
{
	this->fore_ = fore;
    this->back_ = back;
}

// createHistogram function
// Preconditions:	read in the foreground image and create a 3d color histogram based on that foreground image
// Postconditions:	void function, it only construct the 3d color histogram, increment the bins, replace the color 
//					in each bin by looping through the 3d color histogram calculate the average
Mat ColorHistogram::createHistogram(Mat& fore, int size)
{
	// create an array of the histogram dimensions
	// size is a constant - the # of buckets in each dimension
	int dims[] = { size, size, size };
	// create 3D histogram of integers initialized to zero	
	Mat hist(3, dims, CV_32S, Scalar::all(0));

	//histogram bucket
	int bucketSize = 256 / size;
	
	//create histogram - construct histogram based on the foreground image.
	for (int row = 0; row < fore.rows; row++) {
		for (int col = 0; col < fore.cols; col++) {
			int b = fore.at<Vec3b>(row, col)[0] / bucketSize;
			int g = fore.at<Vec3b>(row, col)[1] / bucketSize;
			int r = fore.at<Vec3b>(row, col)[2] / bucketSize;
			hist.at<int>(r, g, b)++;
		}
	}

	return hist;
}

// findMostCount function
// Precondition:	passed in the color histogram constructed from the foreground image
// Postcondition:	set the value for red, green, blue under private
void ColorHistogram::findMostCommon(const Mat& hist, int size)
{
	int count = INT_MIN;
	//Find the histogram bin with the most “votes” by looping over all three dimensions. 
	//If the bin with the most votes is [r, g, b], then the most common color is approximately
	//3 dimensional loop over the histogram
	for (int r = 0; r < size; r++) {
		for (int g = 0; g < size; g++) {
			for (int b = 0; b < size; b++) {
				if (hist.at<int>(r, g, b) > count) {
					count = hist.at<int>(r, g, b);
					// update private member
					this->avgr_ = r;
					this->avgg_ = g;
					this->avgb_ = b;
				}
			}
		}
	}
}

// overlay function
// Preconditions:	read in 2 images, foreground and background to apply the overlay process
// Postconditions:	return 1 image that replace the background of the foreground image with the background image.
Mat ColorHistogram::overlay(const Mat& fore,Mat& back, int size)
{
	Mat output = fore.clone();
	int bucketSize = 256 / size;
	//calculate common color:
	int cRed = this->avgr_ * bucketSize + bucketSize / 2;
	int cGreen = this->avgg_ * bucketSize + bucketSize / 2;
	int cBlue = this->avgb_ * bucketSize + bucketSize / 2;

	for (int row = 0; row < fore.rows; row++) {
		for (int col = 0; col < fore.cols; col++) {
			int b = output.at<Vec3b>(row, col)[0];
			int g = output.at<Vec3b>(row, col)[1];
			int r = output.at<Vec3b>(row, col)[2];

			// current pixel of this col must no more than a bucketSize away
			if (abs(r - cRed) <= bucketSize && abs(g - cGreen) <= bucketSize
				&& abs(b - cBlue) <= bucketSize) {
				output.at<Vec3b>(row, col)[0] = back.at<Vec3b>(row % back.rows, col % back.cols)[0];
				output.at<Vec3b>(row, col)[1] = back.at<Vec3b>(row % back.rows, col % back.cols)[1];
				output.at<Vec3b>(row, col)[2] = back.at<Vec3b>(row % back.rows, col % back.cols)[2];
			}
		}
	}
	return output;
}

Mat ColorHistogram::useOpenCVMethods(const Mat& back)
{
	//flip (y-axis, code 1)
	Mat output_flipy;
	flip(back, output_flipy, 1);
	//cvtColor
	Mat output_gray;
	cvtColor(output_flipy, output_gray, 6);
	//Gaussian blur
	Mat output_blur;
	GaussianBlur(output_gray, output_blur, Size(7, 7), 2.0, 2.0);
	//Canny
	Mat canny;
	Canny(output_blur, canny, 20, 60);					//lowthreshhold = 0; lowthreshhold*ratio = 0*3; kernelsize = 3
    return canny;
}

Mat ColorHistogram::exploreOpenCVMethods(const Mat& back)
{
	//flip (y-axis, code 1)
	Mat output_flipy;
	flip(back, output_flipy, 1);
	//median blur
	Mat output_medblur;
	medianBlur(output_flipy, output_medblur, 3);
	//Laplacian operator
	Mat blur, laplacian;
	GaussianBlur(output_medblur, blur, Size(3, 3), 0);
	Laplacian(blur, laplacian, CV_16S, 3, 1, 0, BORDER_DEFAULT);
    return laplacian;
}
