// -------------------------------------Smoothedge.cpp------------------------------------------------------------
// Class: CSS487A
// Project Name: Program 2
// by Andrew Chou
// ---------------------------------------------------------------------------------------------------------------
// Purpose: .cpp file for Smoothedge.h file. This provides the body of all
//			function header on the .h file.
//			This class consists of 3 main function:
//				Smooth(): for convolution with appropriate kernel for iterations times
//				linearFiltering(): a generalized convolution method to be used by Smooth and EdgeDetect
//				edgeDetect(): to find the maximum gmag pixel and set the intensity to 255.5 as edge.
// ---------------------------------------------------------------------------------------------------------------

#include "Smoothedge.h"

// Smoothedge Constructor
// Post Condition: save the input image variable
//				and initiate kernel such as gaussianX, gaussianY, gradientX, gradientY
Smoothedge::Smoothedge(const Image& input) {
	this->gif = input;
	Image gX(1, 3);
	Image gY(3, 1);
	gaussianX = gX;
	gradientX = gX;
	gaussianY = gY;
	gradientY = gY;
}
// convertFloatPix function
// Pre condition: all value within each pixel is int type.
// Post condition: convert all those value to float value instead and return output back to method
Image Smoothedge::convertFloatPix(const Image& input) const {
	Image output(input.getRows(), input.getCols());
	for (int row = 0; row < input.getRows(); row++) {
		for (int col = 0; col < input.getCols(); col++) {
			pixel color = input.getPixel(row, col);
			float grey = (color.green + color.red + color.blue) / 3;
			output.setFloat(row, col, grey);
		}
	}
	return output;
}

// convertToByte function
// Pre condition: all value remain in float type.
// Post condition: cast the float value of the pixel to static_cast<byte> type and set it back to output
Image Smoothedge::convertToByte(const Image& input) const {
	Image output(input.getRows(), input.getCols());
	for (int row = 0; row < output.getRows(); row++) {
		for (int col = 0; col < output.getCols(); col++) {
			byte grey = static_cast<byte>(input.getFloat(row, col));
			output.setGrey(row, col, grey);
		}
	}
	return output;
}

// kernel Setter function
// Pre condition: all kernel are left with default value except total row and col
// Post condition: adjust the float value of each pixel of the kernels.
void Smoothedge::setFilterKernel() {
	float x = 0.25;
	float y = 0.5;
	float z = 0.25;
	gaussianX.setFloat(0, 0, x);
	gaussianX.setFloat(0, 1, y);
	gaussianX.setFloat(0, 2, z);
	gaussianY.setFloat(0, 0, x);
	gaussianY.setFloat(1, 0, y);
	gaussianY.setFloat(2, 0, z);
	x = -1.0;
	y = 0;
	z = 1.0;
	gradientX.setFloat(0, 0, x);
	gradientX.setFloat(0, 1, y);
	gradientX.setFloat(0, 2, z);
	gradientY.setFloat(0, 0, x);
	gradientY.setFloat(1, 0, y);
	gradientY.setFloat(2, 0, z);
}

// GaussianX getter function
// Pre condition: no access to gaussianX kernel
// Post condition: return the gaussianX kernel (image type)
Image Smoothedge::getGaussianX() {
	return this->gaussianX;
}

// GaussianY getter function
// Pre condition: no access to gaussianY kernel
// Post condition: return the gaussianY kernel (image type)
Image Smoothedge::getGaussianY() {
	return this->gaussianY;
}

// GradientX getter function
// Pre condition: no access to gradientX kernel
// Post condition: return the gradientX kernel (image type)
Image Smoothedge::getGradientX() {
	return this->gradientX;
}

// GradientY getter function
// Pre condition: no access to gradientY kernel
// Post condition: return the gradientY kernel (image type)
Image Smoothedge::getGradientY() {
	return this->gradientY;
}

// linearFiltering function
// Pre condition: no filter/convolution being made, kernel not being computed with image
// Post codition: convolute the image pixel accordingly based on the kernel being provided
//					kernel dimension are being generalized in this function.
Image Smoothedge::linearFiltering(const Image& input, const Image& kernel, vector<int> kOrigin) const {
	Image output(input);
	for (int row = 0; row < output.getRows(); row++) {
		for (int col = 0; col < output.getCols(); col++) {
			float sum = 0;
			for (int u = 0; u < kernel.getRows(); u++) {
				for (int v = 0; v < kernel.getCols(); v++) {
					// adjustment counter to make sure the pixel are valid when plugging in the formula
					int adjustU = 0;
					int adjustV = 0;

					// conditions for adjusting the "row + u - kOrigin[0]" and "col + v - kOrigin[1]"
					// if any of the value is either below zero or over total row or col
					// increment or decrement the adjustment counter respectively.
					while (row + u - kOrigin[0] + adjustU < 0) {
						adjustU++;
					}
					while (row + u - kOrigin[0] + adjustU > output.getRows() - 1) {
						adjustU--;
					}
					while (col + v - kOrigin[1] + adjustV < 0) {
						adjustV++;
					}
					while (col + v - kOrigin[1] + adjustV > output.getCols() - 1) {
						adjustV--;
					}
					// performing the formula: O(r, c) = sum of (k(u, v) * I(row - u, col - v))
					sum += kernel.getFloat(u, v) * 
						input.getFloat(row + u - kOrigin[0] + adjustU, col + v - kOrigin[1] + adjustV);
				}
			}
			output.setFloat(row, col, sum);
		}
	}
	return output;
}

// Smoothing function
// Pre condition: no call on linearFiltering to perform computation with gaussian's X and Y kernel
// Post condition: Performed calculation for kernel center point and passed into linearFiltering
//					Compute the value for each pixel accordingly with the kernel provided.
//					Performed the filtering one at a time (perform on kernel X for iteration times then on Y)
// Image * Gaussian 
Image Smoothedge::smoothing(const Image& input, int iteration) const {
	Image output(input);
	// row = 0 & col = 1 = center.
	vector<int> xCenter;
	xCenter.push_back(this->gaussianX.getRows() / 2);
	xCenter.push_back(this->gaussianX.getCols() / 2);
	// row = 1 & col = 0 = center.
	vector<int> yCenter;
	yCenter.push_back(this->gaussianY.getRows() / 2);
	yCenter.push_back(this->gaussianY.getCols() / 2);
	for (int i = 0; i < iteration; i++) {
		output = linearFiltering(output, gaussianX, xCenter);
	}
	for (int i = 0; i < iteration; i++) {
		output = linearFiltering(output, gaussianY, yCenter);
	}
	return output;
}

// edgeDetect function
// Pre condition: no linearFiltering call on smoothed image to perform computation with gradient's X and Y kernel
// Post condition: performed calculation to find gx, gy and gmag 
//					using that to confirm that gmag is bigger than 10.0, > than pixel at same direction and opposite direction
//					If condition met, set the pixel to be edge by change the intensity to 255.0
Image Smoothedge::edgeDetect(const Image& input) const {
	Image output(input);
	// getting center
	// row = 0 & col = 1 = center.
	vector<int> xCenter;
	xCenter.push_back(this->gradientX.getRows() / 2);
	xCenter.push_back(this->gradientX.getCols() / 2);
	// row = 1 & col = 0 = center.
	vector<int> yCenter;
	yCenter.push_back(this->gradientY.getRows() / 2);
	yCenter.push_back(this->gradientY.getCols() / 2);
	// convolution with gradient kernel once each pixel
	Image gx = linearFiltering(output, gradientX, xCenter);
	Image gy = linearFiltering(output, gradientY, yCenter);
	Image gmag(input);
	gmag = linearFiltering(output, gradientX, xCenter);
	gmag = linearFiltering(output, gradientY, yCenter);
	// loop through to calculate gradient magnitude (for edge detection)
	for (int row = 0; row < output.getRows(); row++) {
		for (int col = 0; col < output.getCols(); col++) {
			// edge if 3 criteria met
			// one criterion not met ==> not edge 
			if (gmag.getFloat(row, col) >= 10.0 &&
				gmag.getFloat(row, col) > gx.getFloat(row, col) &&
				gmag.getFloat(row, col) > gy.getFloat(row, col)) {
				gmag.setFloat(row, col, 255.0);
			}
		}
	}
	return gmag;
}