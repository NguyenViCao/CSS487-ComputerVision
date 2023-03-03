#include "transform.h"
#include "Image.h"
#include <cmath>

//default constructor
Transform::Transform(Image input)
{
	this->input_ = input;
}

//----------------------------------------------------end of default constructor--------------------------------------------------------

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
Image Transform::transform(const double& sx, const double& sy, const double& tx, const double& ty, const double& theta, const double& k, const Image& input)
{
	Image output(input.getRows(), input.getCols());

	//convert theta to radian, multiply value by pi/180 to get radian
	double pi = 3.14159265359;

	// assign c(center)
	float cx = (float)input.getCols() / 2;
	float cy = (float)input.getRows() / 2;

	// Loop over all pixels and copy them into output image.
	// The output will be transformed accordingly to the six parameters that are passed in
	for (int row = 0; row < input.getRows(); row++) {
		for (int col = 0; col < input.getCols(); col++) {
			// assign p (point from the original image)

			// let a = q - t - c
			float ax = (float)col - tx - cx;
			float ay = (float)row - ty - cy;

			// calculating p1 = R^-1 * K^-1
			float px1 = (-k * cos(theta * (pi / 180))) + cos(theta * (pi / 180)) + sin(theta * (pi / 180));
			float py1 = (k * sin(theta * (pi / 180))) - sin(theta * (pi / 180)) + cos(theta * (pi / 180));

			// calculating p2 = p1 * S^-1
			// originally R^-1 * K^-1 * S^-1
			float px2 = (((-k * cos(theta * (pi / 180))) + sin(theta * (pi / 180))) / sy) + (cos(theta * (pi / 180)) / sx);
			float py2 = (((k * sin(theta * (pi / 180))) + cos(theta * (pi / 180))) / sy) - (sin(theta * (pi / 180)) / sx);

			// calculating p3 = p2 + p
			// originally R^-1 * K^-1 * S^-1 * (q - t - c)
			float px3 = ((cos(theta * (pi / 180)) / sx) * ax) + ((((-k * cos(theta * (pi / 180))) + sin(theta * (pi / 180))) / sy) * ay);
			float py3 = ((-sin(theta * (pi / 180)) / sx) * ax) + ((((k * sin(theta * (pi / 180))) + cos(theta * (pi / 180))) / sy) * ay);

			// update p3 += c
			// originally R^-1 * K^-1 * S^-1 * (q - t - c) + c
			px3 += cx;
			py3 += cy;

			if (0 < px3 && px3 <= input.getCols() && 0 < py3 && py3 <= input.getRows())
			{
				output.setPixel(row, col, input.getPixel(py3, px3));
			}
		}
	}
	return output;

}

//---------------------------------------------------end of transform function-------------------------------------------------------

// bilinear Interpolation function
// Preconditions:	pass in an image that has already gone through the linear transformation and get the correct color for that pixel
// Postconditions:	return a pixel that is colored and intensify averaged base on the neighbor pixels. 
pixel Transform::bilinInter(const Image& input, pair<double, double>& inp_pix)
{
	pixel outp_pix;
	int col = floor(inp_pix.first);
	int row = floor(inp_pix.second);

	double a = inp_pix.second - row;
	double b = inp_pix.first - col;

	if (col + 1 < input.getCols() && row + 1 < input.getRows() && col - 1 > 0 && row - 1 > 0)
	{
		outp_pix.red = (1 - a) * (1 - b) * input.getPixel(row, col).red + a * (1 - b) * input.getPixel(row + 1, col).red
			+ (1 - a) * b * input.getPixel(row, col + 1).red + a * b * input.getPixel(row + 1, col + 1).red;
		outp_pix.green = (1 - a) * (1 - b) * input.getPixel(row, col).green + a * (1 - b) * input.getPixel(row + 1, col).green
			+ (1 - a) * b * input.getPixel(row, col + 1).green + a * b * input.getPixel(row + 1, col + 1).green;
		outp_pix.blue = (1 - a) * (1 - b) * input.getPixel(row, col).blue + a * (1 - b) * input.getPixel(row + 1, col).blue
			+ (1 - a) * b * input.getPixel(row, col + 1).blue + a * b * input.getPixel(row + 1, col + 1).blue;
	}
	else
	{
		outp_pix.red = 0;
		outp_pix.green = 0;
		outp_pix.blue = 0;
	}
	return outp_pix;
}
//--------------------------------------------end of bilinear interpolstion function-------------------------------------------------