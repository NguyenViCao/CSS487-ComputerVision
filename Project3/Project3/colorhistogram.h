#ifndef _COLORHISTOGRAM_H_
#define _COLORHISTOGRAM_H_
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;

// author: Nguyen Vi Cao
// CSS 487, prof Clark Olson
// header file for the Color Histogram that finds the most common color in the foreground image.

class ColorHistogram
{
public:
	// default constructor
	// Preconditions:	create 1 image object
	ColorHistogram(const Mat& back);

	// default constructor
	// Preconditions:	create 2 image objects
	ColorHistogram(const Mat& fore, const Mat& back);

	// createHistogram function
	// Preconditions:	read in the foreground image and create a 3d color histogram based on that foreground image
	// Postconditions:	void function, it only construct the 3d color histogram, increment the bins, replace the color 
	//					in each bin by looping through the 3d color histogram calculate the average
	Mat createHistogram(Mat& fore, int size);

	// findMostCount function
	// Precondition:	passed in the color histogram constructed from the foreground image
	// Postcondition:	set the value for red, green, blue under private
	void findMostCommon(const Mat& hist, int size);

	// overlay function
	// Preconditions:	read in 2 images, foreground and background to apply the overlay process
	// Postconditions:	return 1 image that replace the background of the foreground image with the background image.
	Mat overlay(const Mat& fore, Mat& back, int size);

	// useOpenCVMethods function
	// Preconditions:	read in the foreground image then apply the x-axis flip, covert to gray(cvtColor), Gaussian blur, and canny edge detection
	// Postconditions:	return an output image
	Mat useOpenCVMethods(const Mat& back);

	// convert function
	// Preconditions:	read in the foreground image then apply the y-axis flip, median blur, and Laplacian operator
	// Postconditions:	return myoutput image
	Mat exploreOpenCVMethods(const Mat& back);

private:
	Mat fore_;
	Mat back_;
	int size = 4;
	int avgb_, avgg_, avgr_;				//the avarage value of each color avgb_ = blue, avgg_ = green, avgr_ red
};
#endif