#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include "Image.h"

// author: Nguyen Vi Cao
// CSS 487, prof Clark Olson
// header file for Transform class that contains linear transformation, and bilinear interpolation
class Transform
{
public:
	//default constructor
	Transform(Image input);

	// transform
	// Preconditions:	input image passed in is correctly allocated/formatted
	// Postconditions:	returns a new image that is transformed according to the six parameters that is read from the batch file (sx, sy, tx, ty, θ, and k)
	//					p = R^(-1) * K^(-1) * S^(-1) * (q - t - c) + c
	//					p is a point from the output image
	//					R rotate by theta (angle)
	//					K shear factor 
	//					S two scale factors Sx, Sy
	//					t translation tx, ty
	//					c cols/2, rows/2
	Image transform(const double& sx, const double& sy, const double& tx, const double& ty, const double& theta, const double& k, const Image& input);

	// bilinear Interpolation function
	// Preconditions:	pass in an image that has already gone through the linear transformation and get the correct color for that pixel
	// Postconditions:	return a colored image that has been transformed 
	pixel bilinInter(const Image& input, pair<double, double>& input_pix);

private:
	Image input_;
};
#endif