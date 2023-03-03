#include "transform.h"
#include <cmath>

// CSS 487; Professor:	Clark Olson
// transform.cpp:		implementation file of all the functions that are used for program 2, including:
//		convertFloat:	to convert every pixels of the input images to floating point for smoothering and edge detecting later on
//		linearFilt:		perform Linear Filtering with the input image
//		bilinInter:		pass in an image and get the correct color for that pixel
//		smoothing:		to smoothen the image passed in
//		edgeDetection:	detect the edges of an image
// Author:				Nguyen Vi Cao

// default constructor
// Preconditions:	passed in an image (test2.gif already located in the file folder)
Transform::Transform(const Image& input)
{
	this->input_ = input;
	Image Gx(1, 3);
	Image Gy(3, 1);
	gaussianX_ = Gx;
	gradientX_ = Gx;
	gaussianY_ = Gy;
	gradientY_ = Gy;
}

// setter to set the kernel
// Preconditions:	kernel are provided by professor in the prog2 description
void Transform::setKernel()
{
	//declaration of kernel for iterative smoothing
	float x = 0.25;
	float y = 0.5;
	float z = 0.25;
	gaussianX_.setFloat(0, 0, x);
	gaussianX_.setFloat(0, 1, y);
	gaussianX_.setFloat(0, 2, z);

	gaussianY_.setFloat(0, 0, x);
	gaussianY_.setFloat(1, 0, y);
	gaussianY_.setFloat(2, 0, z);

	//delcaration of kernel for edge detection
	x = -1.0;
	y = 0;
	z = 1.0;
	gradientX_.setFloat(0, 0, x);
	gradientX_.setFloat(0, 1, y);
	gradientX_.setFloat(0, 2, z);

	gradientY_.setFloat(0, 0, x);
	gradientY_.setFloat(1, 0, y);
	gradientY_.setFloat(2, 0, z);
}

Image Transform::getGaussianX()
{
	return this->gaussianX_;
}

// convertFloat function	to convert every pixels of the input images to floating point for smoothering and edge detecting later on
// Preconditions:			passed in an image input (hardcode in main function)
// Postconditions:			return an image with every pixel that will now hold the float value instead of pixel like original
Image Transform::convertFloat(const Image& input)const
{
	Image output(input.getRows(), input.getCols());
	for (int row = 0; row < input.getRows(); row++) {
		for (int col = 0; col < input.getCols(); col++) {
			if (0 < row && row <= input.getRows() && 0 < col && col < input.getCols()) {
				output.setFloat(row, col, output.getFloat(row, col));
				pixel color = input.getPixel(row, col);
				float grey = (2 * color.green + color.red + color.blue) / 4;
				output.setFloat(row, col, grey);
			}
		}
	}
	return output;
}

// backToByte function		to convert every pixels of the input images back to byte after getting smoothed and edge detected
// Preconditions:			passed in an image input
// Postconditions:			return an image with every pixel that will now hold converted back to byte
Image Transform::backToByte(const Image& input)const
{
	Image output(input.getRows(), input.getCols());
	for (int r = 0; r < input.getRows(); r++) {
		for (int c = 0; c < input.getCols(); c++) {
			output.setGrey(r, c, static_cast<byte>(input.getFloat(r, c)));
		}
	}
	return output;
}

// linearFilt function		perform Linear Filtering with the input image
// Preconditions:			passed in two input images, one is the test2.gif and the other is the kernel.
// Postconditions:			return a colored image that has been transformed 
Image Transform::linearFilt(const Image& input, const Image& kernel, vector<int> ori) const
{
	Image output(input);
	for (int r = 0; r < output.getRows(); r++) {
		for (int c = 0; c < output.getCols(); c++) {
			float sum = 0;
			for (int u = 0; u < kernel.getRows(); u++) {
				for (int v = 0; v < kernel.getCols(); v++) {
					//setting condition to adjust rows and col for kernel
					int adjU = 0;
					int	adjV = 0;
					while (r + u - ori[0] + adjU < 0){
						adjU++;
					}
					while (r + u - ori[0] + adjU > output.getRows() - 1) {
						adjU--;
					}
					while (c + v - ori[1] + adjV < 0) {
						adjV++;
					}
					while (c + v - ori[1] + adjV > output.getCols() - 1) {
						adjV--;
					}
					//adding up the right pixel at the right point in the kernel
					sum += kernel.getFloat(u, v) * input.getFloat(r + u - ori[0] + adjU, c + v - ori[1] + adjV);
				}
			}
			output.setFloat(r, c, sum);
		}
	}
	return output;
}

// smoothing function	to smoothen the image passed in
// Preconditions:		passed image is located in the correct folder and in the right format
// Postconditions:		return a smoothen image by the right number of iterations in the batch file
Image Transform::smoothing(const Image& input, int iterations)const
{
	Image output(input);
	//center: row at 0, col at 1
	vector<int> x;
	x.push_back(this->gaussianX_.getRows() / 2);
	x.push_back(this->gaussianX_.getCols() / 2);
	//center: row at 1, col at 0
	vector<int> y;
	y.push_back(this->gaussianY_.getRows() / 2);
	y.push_back(this->gaussianY_.getCols() / 2);
	for (int i = 0; i < iterations; i++) {
		output = linearFilt(output, gaussianX_, x);
	}
	for (int i = 0; i < iterations; i++) {
		output = linearFilt(output, gaussianY_, y);
	}
	return output;
}

// edgeDetect function	detect the edges of an image
// Preconditions:		passed image is located in the correct folder and in the right format
// Postconditions:		return an image with edge detection by the right number of iterations in the batch file.
Image Transform::edgeDetect(const Image& input)const
{
	Image output(input);
	//center: row at 0, col at 1
	vector<int> x;
	x.push_back(this->gradientX_.getRows() / 2);
	x.push_back(this->gradientX_.getCols() / 2);
	//center: row at 1, col at 0
	vector<int> y;
	y.push_back(this->gradientY_.getRows() / 2);
	y.push_back(this->gradientY_.getCols() / 2);
	Image gx = linearFilt(output, gradientX_, x);
	Image gy = linearFilt(output, gradientY_, y);
	Image gmag(input);
	gmag = linearFilt(output, gradientX_, x);
	gmag = linearFilt(output, gradientY_, y);
	for (int r = 0; r < output.getRows(); r++) {
		for (int c = 0; c < output.getCols(); c++) {
			//if each pixel
			//1.gradient magnitude > 10.0
			//2.bilinear interpolation
			//3.non-maximous surpression
			//4.is greater than the two nearby pixels p and r
			if (gmag.getFloat(r, c) >= 10.0 && gmag.getFloat(r, c) > gx.getFloat(r, c) && gmag.getFloat(r, c) > gy.getFloat(r, c))
				//=>mark as an edge pixel in the output
				gmag.setFloat(r, c, 255.0);
		}
	}
	//convert back to byte before write to disk
	//backToByte(output);
	return gmag;
}

