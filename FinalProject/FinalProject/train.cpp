// Train.cpp files
// Class: CSS487
// Group Members:
//      Andrew Chou
//      Nguyen Vi Cao
//      Yuze Wang


#include "train.h"
#include <iostream>
#include <stdlib.h>

// Default Constructor for the train class
// Precondition: No image scanned, no object created
// Postcondition: the parameter image is saved to private variable "trainingImage"
Train::Train(const Mat& img)
{
	this->trainingImage = img;
}

// Preprocess method
// Precondition: image remain the same, no preprocessing done
// Post condition: image being processed with:
//                  convert color to grayscale
//                  blurring the grayscale image with Gaussian, Size(0,0)
//                  find threshold of the blurred image
//                  utilize the treshold from previous function call to define contour area
void Train::preprocess()
{
	// convert to grayscale
	cvtColor(this->trainingImage, this->grayScale, 6);
	// Gaussian Blurring
	GaussianBlur(this->grayScale, this->gaussian, Size(5, 5), 0);
	// finding threshold area for the blurred
	adaptiveThreshold(this->gaussian, this->thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 2);		//https://docs.opencv.org/4.x/d7/d4d/tutorial_py_thresholding.html
	// create a copy of threshold
	this->threshCopy = this->thresh.clone();
	// perform contour finding with threshold copy, because this function alter the original threshold
	findContours(this->threshCopy, this->contours, this->contourCorner, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);		//https://docs.opencv.org/3.4/d3/dc0/group__imgproc__shape.html#ga17ed9f5d79ae97bd4c7cf18403e1689a
}

// addBound function
// Precondition: no rect shape shown even if user pressed key
// Postcondition: whenever key is pressed, rect shown on the corresponding character representing the character is learned or looped through
Mat Train::addBound(vector<vector<Point>> ptCon, Mat number, Mat mImgThresh, int mI)
{
	//a rectangle object represent each focus point
	Rect bound = boundingRect(ptCon[mI]);			//https://docs.opencv.org/3.4/da/d0c/tutorial_bounding_rects_circles.html

	//get the color
	Scalar red(0, 0, 255);

	//draw the rectangle around the extracted character
	rectangle(this->trainingImage, bound, red, 3, LINE_4);	//https://www.geeksforgeeks.org/draw-an-rectangle-using-opencv-in-cpp/
	//roi: region of interest
	//use roi image to extract each character from the training image to a small image of each char
	Mat roi = this->thresh(bound);					//https://learnopencv.com/how-to-select-a-bounding-box-roi-in-opencv-cpp-python/
	//resize roi into a small image
	Mat matROIResized;
	resize(roi, matROIResized, Size(20, 30));

	//display three types of image
	imshow("roi image", roi);						//display the cutted image
	imshow("resize", matROIResized);				//display the small image of the roi
	imshow("training image", this->trainingImage);	//display the training image which will have the red rectangle represent the roi
	return matROIResized;
}


// extract function
// Precondition: no loop through contours item, no learning on character shape made
// Postcondition: loop through contour item one by one, showing character to be pressed by user in ROI and resize window, addBound on the character (or contour) being pressed on original input image
void Train::extract()
{
	for (int i = 0; i < this->contours.size(); i++) {
		//we want to cut each characters into a 10*10 pixels image
		//start extracting only if the contour area is large enough
		if (contourArea(this->contours[i]) > 100) {	//100=10*10 pixels	
			Mat roiSmall = addBound(this->contours, trainingImage, thresh, i);
			//match the input character from keyboard with character in training image
			int testChar = waitKey(0);
			if (testChar == 27)
				//if escape key was pressed then stop the program
				exit(0);
			else if (find(this->validChar.begin(), this->validChar.end(), testChar) != this->validChar.end()) {
				this->ClassifyInts.push_back(testChar);		//append classification char to integer list of chars ASCII
				//add the training image (converting the rersize roiSmall into ImageFloat)
				Mat ImageFloat;
				roiSmall.convertTo(ImageFloat, CV_32FC1);
				//flatten the ImageFloat
				Mat flattenImageFLoat = ImageFloat.reshape(1, 1);
				this->trainingFlattenImageFloat.push_back(flattenImageFLoat);
			}
		}
	}
	cout << "training completed" << endl;
}

// store function
// Precondition: data extracted not stored anywhere
// Postcondition: save the data extracted from user pressing to .xml extension file storage to be used later on.
//                  the classification.xml is saving the character that user pressed in decimal code ASCII
//                  the images.xml is saving all components found on images and save it.
void Train::store()
{
	//store the train data into .xml file
	FileStorage fsClassifications("classifications.xml", FileStorage::WRITE);

	//if the file isn't open then exit the program
	if (fsClassifications.isOpened() == false) {
		throwError();
	}
	//write classifications into classification section of classification file
	fsClassifications << "classifications" << this->ClassifyInts;
	fsClassifications.release();		//close

	//open the training image file
	FileStorage fsTrainingImage("images.xml", FileStorage::WRITE);
	//if the file isn't open then exit the program
	if (fsTrainingImage.isOpened() == false) {
		throwError();
	}
	//write training image into section of image file
	fsTrainingImage << "image" << this->trainingFlattenImageFloat;
	fsTrainingImage.release();		//close
}

// throwError function
// Precondition: no exception made, no error searching made
// Postcondition: make sure the file is able to be opened, if failed, notify users.
void Train::throwError()
{
	cout << "error, unable to open file, exiting program" << endl;
	exit(0);
}

// oneCallMethod()
// Precondition: no batch methods called
// Postcondition: call all required method for train to process, extract and store()
void Train::oneCallMethod() {
	preprocess();
	extract();
	store();
}
