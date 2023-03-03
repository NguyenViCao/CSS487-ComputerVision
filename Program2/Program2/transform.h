#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include <vector>
#include "Image.h"

// author: Nguyen Vi Cao
// CSS 487, prof Clark Olson
// header file for Transform class that contains convertFLoat, linear filtering, bilinear interpolation, smoothering, and edge detection
class Transform
{
public:

	// default constructor
	// Preconditions:	passed in an image (test2.gif already located in the file folder)
	Transform(const Image& input);

	// setter to set the kernel
	// Preconditions:	kernel are provided by professor in the prog2 description
	void setKernel();

	// getter for debugging
	Image getGaussianX();

	// convertFloat function	to convert every pixels of the input images to floating point for smoothering and edge detecting later on
	// Preconditions:			passed in an image input (hardcode in main function)
	// Postconditions:			return an image with every pixel that will now hold the float value instead of pixel like original
	Image convertFloat(const Image& input)const;

	// backToByte function		to convert every pixels of the input images back to byte after getting smoothed and edge detected
	// Preconditions:			passed in an image input
	// Postconditions:			return an image with every pixel that will now hold converted back to byte
	Image backToByte(const Image& input)const;

	// linearFilt function		perform Linear Filtering with the input image
	// Preconditions:			passed in two input images, one is the test2.gif and the other is the kernel.
	// Postconditions:			return a colored image that has been transformed 
	Image linearFilt(const Image& input, const Image& kernel, vector<int> ori)const;

	// smoothing function	to smoothen the image passed in
	// Preconditions:		passed image is located in the correct folder and in the right format
	// Postconditions:		return a smoothen image by the right number of iterations in the batch file
	Image smoothing(const Image& input, int iterations)const;

	// edgeDetect function	detect the edges of an image
	// Preconditions:		passed image is located in the correct folder and in the right format
	// Postconditions:		return an image with edge detection by the right number of iterations in the batch file.
	Image edgeDetect(const Image& input)const;

private:
	Image input_;
	Image gaussianX_;
	Image gaussianY_;
	Image gradientX_;
	Image gradientY_;
};
#endif