#include "test.h"
#include <iostream>
#include <stdlib.h>

/*
	CSS487 Computer Vision
	Professor: Clark Olson
	Final Project: Text Recognition
	Author:
		Andrew Chou
		Nguyen Vi Cao
		Yuze Wang

	Test.cpp file includes every functions needed for this step
*/

// default constructor
// Preconditions:		create 1 image object for the Test class
Test::Test(const Mat& img) {
	this->testImage = img;
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);		// https://blog.csdn.net/benobug/article/details/117653268
}

// readImage function
// Preconditions:	read the image.xml file produced after running the training process 
// Postconditions:	assign the value to the flattenImageFloat
void Test::readImage()
{
	//open the original (training) image file
	FileStorage fsOriginalImage("images.xml", FileStorage::READ);
	//if the file isn't open then exit the program
	if (fsOriginalImage.isOpened() == false) {
		throwError();
	}
	//read original image section into Mat training image
	fsOriginalImage["image"] >> this->flattenImageFloat;
	fsOriginalImage.release();		//close
}

// readClassifications function
// Preconditions:	read the Classifications.xml file produced after running the training process 
// Postconditions:	assign the value to the classificationVal
void Test::readClassifications()
{
	FileStorage fsClassifications("Classifications.xml", FileStorage::READ);
	//if the file isn't open then exit the program
	if (fsClassifications.isOpened() == false) {
		throwError();
	}
	//read classifications section into mat classification
	fsClassifications["classifications"] >> this->classificationVal;
	fsClassifications.release();		//close
}

// kNNCreateTrain function
// Preconditions:	install the openCV library in order to use KNN; https://docs.opencv.org/4.x/dd/de1/classcv_1_1ml_1_1KNearest.html
// Postconditions:	instantiate K Nearest Neighbor object
void Test::kNNCreateTrain()
{
	this->kNNObj = ml::KNearest::create();
	this->kNNObj->train(this->flattenImageFloat, ml::ROW_SAMPLE, classificationVal);
}

// getKnnObj function
// Preconditions:	KNN object must be created before hence 
// Postconditions:	a pointer points to the KNN object
Ptr<ml::KNearest> Test::getKnnObj() {
	return this->kNNObj;
}

// checkValid function
// Preconditions:	call the function inside DataContour class to check if every contours are valid or not	
// Postconditions:	add contour with data to the list validContour
void Test::checkValid()
{
	for (int i = 0; i < allContourWithData.size(); i++) {						// iterate through every contours
		if (allContourWithData[i].validContours()) {
			validContour.push_back(allContourWithData[i]);						// check if they valid, if so append to valid contour list
		}
	}
}

// assignData function
// Preconditions:	DataContour class must and checkValid must operate correctly in order for this function get retrieve value to assign	
// Postconditions:	add contour with data object to list of all contours (contours that already contain data at this point)
void Test::assignData()
{
	for (int i = 0; i < contours.size(); i++) {									// for each contour
		DataContours conWithData;                                               // instantiate a contour with data object
		conWithData.setCorners(contours[i]);									// assign contour to contour with data
		conWithData.setBound(boundingRect(conWithData.getCorners()));			// get the bounding rect
		conWithData.setArea((float)contourArea(conWithData.getCorners()));		// calculate the contour area
		allContourWithData.push_back(conWithData);                              // add contour with data object to list of all contours (contours that already contain data at this point)
	}
}

// sortlr function
// Preconditions:	have a list of valid contours with data that are ready to be sort
// Postconditions:	sorts the contours from left to right
void Test::sortlr()
{
	//sort contours from left to right
	sort(validContour.begin(), validContour.end(), DataContours::sortEachBound);
}

// preprocess function
// Preconditions:	have already read in the test image by the class to work with 
// Postconditions:	apply ColorCVT, Gaussian blur, adativeThreshold, and findContour on the test image
void Test::preprocess()
{
	cvtColor(this->testImage, this->grayScale, 6);
	GaussianBlur(this->grayScale, this->gaussian, Size(5, 5), 0);
	adaptiveThreshold(this->gaussian, this->thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 2);		//https://docs.opencv.org/4.x/d7/d4d/tutorial_py_thresholding.html
	this->threshCopy = this->thresh.clone();
	findContours(this->threshCopy, this->contours, this->contourCorner, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);		//https://docs.opencv.org/3.4/d3/dc0/group__imgproc__shape.html#ga17ed9f5d79ae97bd4c7cf18403e1689a
}

// recognize function
// Preconditions:	test image must be preprocessed and convert to array of contours	
// Postconditions:	draw a green rectangle on each of the characters identified on the test image
//					compare the characters identified on the test image with the characters that are already learned from the training process
//					append the result to the res tring
void Test::recognize()
{
	kNNCreateTrain(); // create KNN and train it here!
	//go through each contour
	for (int i = 0; i < validContour.size(); i++) {
		//get a green color for the rectangle instead of red
		Scalar green(0, 255, 0);
		//a rectangle object represent each focus point
		Rect bounding = validContour[i].getBound();	//https://docs.opencv.org/3.4/da/d0c/tutorial_bounding_rects_circles.html
		//draw a rectangle around the extracted character
		rectangle(this->testImage, bounding, green, 3, LINE_4);					//https://www.geeksforgeeks.org/draw-an-rectangle-using-opencv-in-cpp/
		Mat roi = this->thresh(bounding);          //get ROI image of bounding rect

		//resize roi into a small image
		resize(roi, this->ROIResized, Size(20, 30));

		Mat ROIFloat;
		this->ROIResized.convertTo(ROIFloat, CV_32FC1);             //convert Mat to float in order for calling find_nearest

		Mat ROIFlattenedFloat = ROIFloat.reshape(1, 1);
		Mat currentChar(0, 0, CV_32F);

		this->kNNObj->findNearest(ROIFlattenedFloat, 1, currentChar);

		float currChar = (float)currentChar.at<float>(0, 0);
		this->res = this->res + char(int(currChar));							//append current char to full string
	}
}

// display function
// Preconditions:	res string must have data after the recognize function	
// Postconditions:	output the string to the console
void Test::display()
{
	cout << "\n\n" << "recognized characters are = " << this->res << "\n\n";       //display the result string
	cv::imshow("Testing Image", this->testImage);				//display input image with green boxes drawn around found digits
	cv::waitKey(0);												// wait for user key press
}

// oneCallMethod function
// Preconditions:	all of the functions above need to work properly	
// Postconditions:	call all of the needed functions
void Test::oneCallMethod()
{
	readImage();
	readClassifications();
	preprocess();
	assignData();
	checkValid();
	sortlr();
	recognize();
	display();
}

// throwError function
// Preconditions:	N/A 
// Postconditions:	throw an error message if there are any files that can't be read
void Test::throwError()
{
	cout << "error, unable to open training image file, exiting program" << endl;
	exit(0);
}