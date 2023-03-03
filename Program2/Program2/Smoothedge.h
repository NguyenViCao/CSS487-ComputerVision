// -------------------------------------Smoothedge.h--------------------------------------------------------------
// Class: CSS487A
// Project Name: Program 1
// by Andrew Chou
// ---------------------------------------------------------------------------------------------------------------
// Purpose: .h file for Smoothedge.cpp file. This works as a contract that show 
//			all functions of the class.
//			This class consists of 3 main function:
//				Smooth(): for convolution with appropriate kernel for iterations times
//				linearFiltering(): a generalized convolution method to be used by Smooth and EdgeDetect
//				edgeDetect(): to find the maximum gmag pixel and set the intensity to 255.5 as edge.
// ---------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include "Image.h"


using namespace std;

class Smoothedge {

public:

	// Smoothedge Constructor
	// Post Condition: save the input image variable
	//				and initiate kernel such as gaussianX, gaussianY, gradientX, gradientY
	Smoothedge(const Image& input);

	// convertFloatPix function
	// Pre condition: all value within each pixel is int type.
	// Post condition: convert all those value to float value instead and return output back to method
	Image convertFloatPix(const Image& input) const;

	// convertToByte function
	// Pre condition: all value remain in float type.
	// Post condition: cast the float value of the pixel to static_cast<byte> type and set it back to output
	Image convertToByte(const Image& input) const;

	// GaussianX getter function
	// Pre condition: no access to gaussianX kernel
	// Post condition: return the gaussianX kernel (image type)
	Image getGaussianX();

	// GaussianY getter function
	// Pre condition: no access to gaussianY kernel
	// Post condition: return the gaussianY kernel (image type)
	Image getGaussianY();

	// GradientX getter function
	// Pre condition: no access to gradientX kernel
	// Post condition: return the gradientX kernel (image type)
	Image getGradientX();

	// GradientY getter function
	// Pre condition: no access to gradientY kernel
	// Post condition: return the gradientY kernel (image type)
	Image getGradientY();

	// kernel Setter function
	// Pre condition: all kernel are left with default value except total row and col
	// Post condition: adjust the float value of each pixel of the kernels.
	void setFilterKernel();

	// linearFiltering function
	// Pre condition: no filter/convolution being made, kernel not being computed with image
	// Post codition: convolute the image pixel accordingly based on the kernel being provided
	//					kernel dimension are being generalized in this function.
	Image linearFiltering(const Image& input, const Image& kernel, vector<int> kOrigin) const;

	// Smoothing function
	// Pre condition: no call on linearFiltering to perform computation with gaussian's X and Y kernel
	// Post condition: Performed calculation for kernel center point and passed into linearFiltering
	//					Compute the value for each pixel accordingly with the kernel provided.
	//					Performed the filtering one at a time (perform on kernel X for iteration times then on Y)
	Image smoothing(const Image& input, int iteration) const;

	// edgeDetect function
	// Pre condition: no linearFiltering call on smoothed image to perform computation with gradient's X and Y kernel
	// Post condition: performed calculation to find gx, gy and gmag 
	//					using that to confirm that gmag is bigger than 10.0, > than pixel at same direction and opposite direction
	//					If condition met, set the pixel to be edge by change the intensity to 255.0
	Image edgeDetect(const Image& input) const;

private:
	// image member
	Image gif;
	// the kernels
	Image gaussianX;
	Image gaussianY;
	Image gradientX;
	Image gradientY;
};