// CSS 487; Professor: Clark Olson
// Program3:		create a histogram color. Replace every pixel in the foreground image that is close to the most common color with 
//					the corresponding pixel from the background image 
// Preconditions:	OpenCV is successfully installed.
//					OpenCVDebug.prop file is under the project folder.
//					background.jpg and foreground.jpg is under the project folder
// Postconditions:	create overlay.jpg that go over the color histogram process that replace the blue background of the foreground.jpg with the background.jpg
//					create output.jpg that Modify the background image by flipping it horizontally, converting it to greyscale, blurring it, and detecting edges 
//					(each of these is a call to an OpenCV method – flip, cvtColor, GaussianBlur, Canny).
//					create myoutput.jpg that implement three additional operations to apply to the (original) image to create an interesting output
// Author:			Nguyen Vi Cao

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "colorhistogram.h"
#include <iostream>
using namespace cv;
using namespace std;

// main method
// precondition:	read in foreground.jpg and background.jpg, already has the OpenCVDebug.prop file is under the project folder in order to compile correctly.
// Postconditions:	create overlay.jpg that go over the color histogram process that replace the blue background of the foreground.jpg with the background.jpg
//					create output.jpg that Modify the background image by flipping it horizontally, converting it to greyscale, blurring it, and detecting edges 
//					(each of these is a call to an OpenCV method – flip, cvtColor, GaussianBlur, Canny).
//					create myoutput.jpg that implement three additional operations to apply to the (original) image to create an interesting output
int main(int argc, char* argv[])
{	
		Mat fore = imread("foreground.jpg");
		Mat back = imread("background.jpg");

		//use OpenCV methods on output object
		//write output to disk
		ColorHistogram backg(back);
		Mat output = backg.useOpenCVMethods(back);
		imwrite("output.jpg", output);

		//explore OpenCV methods; call 3 more operations apply it to myoutput object
		//write myoutput to disk
		Mat myoutput = backg.exploreOpenCVMethods(back);
		imwrite("myoutput.jpg", myoutput);

		//color histogram
		int size = 4;
		ColorHistogram obj(fore, back);
		Mat hist = obj.createHistogram(fore, size);
		obj.findMostCommon(hist, size);
		Mat overlayed = obj.overlay(fore, back, size);
		namedWindow("overlayed", WINDOW_NORMAL);
		resizeWindow("overlayed", overlayed.cols / 8, overlayed.rows / 8);
		imshow("Overlayed Image ", overlayed);
		imwrite("overlay.jpg", overlayed);

		return 0;
}