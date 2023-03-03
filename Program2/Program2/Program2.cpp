// CSS 487; Professor: Clark Olson
// Program2:		implement linear filtering and edge detection and observe the differences in edge detection for different levels of smoothing.
// Preconditions:	the function should take two images as parameters (the kernel should be represented as a small image). 
//					It should also take as inputs the coordinates of the “origin” of the kernel
//					The inputs and outputs to your convolution routine should use floating-point pixel values to prevent the unnecessary loss of information. 
//					You will need to convert the input image to floating point at the start and convert back to grey-scale bytes before writing the image to disk. 
//					All other operations should be performed in floating point.
// Postconditions:	output two images “smooth.gif” is the image after smoothing; “edges.gif” is the result of the edge detection 
//					Use writeGreyImage to output them to disk (after converting the smoothed image from floating point back to byte)
// Author:			Nguyen Vi Cao

#include "Image.h"
#include "transform.h"
#include <iostream>
using namespace std;

// main method
// Preconditions:	test2.gif, gmag.gif and runtest2.bat are in the program2 folder. 
// Postconditions:	Creates two images smooth.gif and edge.gif that have already gone 
//					through the process of linear filtering, iterative smoothing, and edge detection
int main(int argc, char* argv[])
{
	int iterations;
	sscanf_s(argv[1], "%d", &iterations);
	
	//iterations = 2;
	Image input("test2.gif");

	//convert input image to float
	Transform convert(input);
	convert.setKernel();
	Image output = convert.convertFloat(input);

	//call smooth func
	Image smooth(output);
	output = convert.smoothing(smooth, iterations);
	smooth = convert.smoothing(smooth, iterations);
	smooth = convert.backToByte(smooth);
	smooth.writeGreyImage("smooth.gif");

	//call edge func
	output = convert.edgeDetect(output);
	output = convert.backToByte(output);
	output.writeFloatImage("edge.gif");
	return 0;
}