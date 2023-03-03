// CSS 487; Professor: Clark Olson
// Program1:		a program that takes these six parameters (sx, sy, tx, ty, theta, and k) as input from the batch file (in that order) 
//					and transforms an image according to the parameters
// Preconditions:	6 inputs passed in are correctly formatted. Assume that the input image is named “test1.gif” and is located in the same directory as your code.
// Postconditions:	The output image should have the same dimensions as the input image. Any point in the output image that doesn’t have a corresponding 
//					point in the input image should be black. Write the output to “output.gif” and place it in the same directory as your code.
// Author:			Nguyen Vi Cao

#include "Image.h"
#include "transform.h"

// main method
// Preconditions:	test1.gif exists and is a correctly formatted GIF image. 6 inputs passed in are correctly formatted.
// Postconditions:	Creates an image output.gif that takes these six parameters (sx, sy, tx, ty, theta, and k) as inputs from the batch file (in that order) 
//					and transforms an image according to the parameters
//					S two scale factors Sx, Sy
//					t translation tx, ty
//					R rotate by theta (angle)
//					K shear factor 
int main(int argc, char** argv)
{
	double x_scale;
	double y_scale;
	double x_trans;
	double y_trans;
	double theta_rotate;
	double k_shear;
	sscanf_s(argv[1], "%lf", &x_scale);
	sscanf_s(argv[2], "%lf", &y_scale);
	sscanf_s(argv[3], "%lf", &x_trans);
	sscanf_s(argv[4], "%lf", &y_trans);
	sscanf_s(argv[5], "%lf", &theta_rotate);
	sscanf_s(argv[6], "%lf", &k_shear);
	
	Image input("test1.gif");
	Transform convert(input);
	Image output = convert.transform(x_scale, y_scale, x_trans, y_trans, theta_rotate, k_shear, input);

	output.writeImage("output.gif");
	return 0;
}