#ifndef TEST_H_
#define TEST_H_
#include "datacontours.h"
#include "train.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <vector>
using namespace std;
using namespace cv;

/*
	CSS487 Computer Vision
	Professor: Clark Olson
	Final Project: Text Recognition
	Author:
		Andrew Chou
		Nguyen Vi Cao
		Yuze Wang

	Header file for the testing process of the Text Recognigtion project 
*/

class Test
{
public:
	// default constructor
	// Preconditions:		create 1 image object for the Test class
	Test(const Mat& img);

	// readImage function
	// Preconditions:	read the image.xml file produced after running the training process 
	// Postconditions:	assign the value to the flattenImageFloat
	void readImage();

	// readClassifications function
	// Preconditions:	read the Classifications.xml file produced after running the training process 
	// Postconditions:	assign the value to the classificationVal
	void readClassifications();

	// kNNCreateTrain function
	// Preconditions:	install the openCV library in order to use KNN; https://docs.opencv.org/4.x/dd/de1/classcv_1_1ml_1_1KNearest.html
	// Postconditions:	instantiate K Nearest Neighbor object
	void kNNCreateTrain();

	// getKnnObj function
	// Preconditions:	KNN object must be created before hence 
	// Postconditions:	a pointer points to the KNN object
	Ptr<ml::KNearest> getKnnObj();

	// checkValid function
	// Preconditions:	call the function inside DataContour class to check if every contours are valid or not	
	// Postconditions:	add contour with data to the list validContour
	void checkValid();

	// assignData function
	// Preconditions:	DataContour class must and checkValid must operate correctly in order for this function get retrieve value to assign	
	// Postconditions:	add contour with data object to list of all contours (contours that already contain data at this point)
	void assignData();

	// sortlr function
	// Preconditions:	have a list of valid contours with data that are ready to be sort	
	// Postconditions:	sorts the contours from left to right
	void sortlr();

	// preprocess function
	// Preconditions:	have already read in the test image by the class to work with 
	// Postconditions:	apply ColorCVT, Gaussian blur, adativeThreshold, and findContour on the test image
	void preprocess();

	// recognize function
	// Preconditions:	test image must be preprocessed and convert to array of contours	
	// Postconditions:	draw a green rectangle on each of the characters identified on the test image
	//					compare the characters identified on the test image with the characters that are already learned from the training process
	//					append the result to the res tring
	void recognize();

	// display function
	// Preconditions:	res string must have data after the recognize function	
	// Postconditions:	output the string to the console
	void display();

	// oneCallMethod function
	// Preconditions:	all of the functions above need to work properly	
	// Postconditions:	call all of the needed functions
	void oneCallMethod();

	// throwError function
	// Preconditions:	N/A 
	// Postconditions:	throw an error message if there are any files that can't be read
	void throwError();

private:
	Mat testImage, grayScale, gaussian, thresh, threshCopy, ROIResized;
	vector<vector<Point>> contours;
	vector<Vec4i> contourCorner;
	vector<DataContours> allContourWithData;
	vector<DataContours> validContour;
	Mat classificationVal;								//read data from the classification numbers
	Mat flattenImageFloat;								//read data from the image numbers
	string res;											//a string that will hold our final result
	Ptr<ml::KNearest> kNNObj;
};
#endif
