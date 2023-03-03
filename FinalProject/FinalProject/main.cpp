/* main.cpp
CSS487 Computer Vision
Professor: Clark Olson
Final Project: Text reconigtion
Author:
	Andrew Chou
	Nguyen Vi Cao
	Yuze Wang
 */

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "train.h"
#include "test.h"
#include <iostream>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;

// main 			create a Mat training object, then call the train class to learn the characters;
//					create a Mat testing object, then call the test class to compare the characters
//						on the testing image with the characters learned and output to the console
// precondition:	need to have characters image that are in different fonts; test image
//						for now, need to be in just one form for best output
//					openCV need to be installed
//					all of the characters input for the training process need to be correctly
//						input from the user
// postconditions:	after the training process, program will generate the two xml files classifications.xml
//						and image.xml in order for the testing process. Afterwards, testing
//						will be able to identify and output the characters on the test image
//						on the console.

int main(int argc, char* argv[])
{	
	Mat training = imread("training_chars.png");
	Mat testing = imread("test1.png");
	//Mat testing2 = imread("test2.png");
	//Mat testing3 = imread("test3.png");
	
	// perform training
	// With xml files given, this train step can be skipped. Simply commenting two lines below
	Train train_data(training);
	train_data.oneCallMethod();

	//done with training
	//testing step
	Test test_data(testing);
	test_data.oneCallMethod();

	return 0;
}