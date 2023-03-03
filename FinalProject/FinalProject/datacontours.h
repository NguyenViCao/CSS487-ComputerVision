#ifndef DATACONTOURS_H_
#define DATACONTOURS_H_
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
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

	Header file for the DataContour class needed for the testing process.
*/

class DataContours
{
public:
	// default constructor
	DataContours();

	//determine whether if that contour is valid
	//preconditions:	have the area already been setted
	//postconditions:	return false if the area is smaller then 10x10 pixels,
	//						and return ture if area satisfy the target size
	bool validContours();		//check for the area of the contours

	//arrange contours in the order of from left to right
	//preconditions:	have bounds of a contours
	//postconditions:	the bounds of a contours are sorted from left to right
	//						to be able to used for the contours
	static bool sortEachBound(const DataContours left, const DataContours right);			//sort the contours from left to right

	//return the corner of each contours
	//precondition:		no access to the private variables, have a value
	//						that can be turned into corner
	//postcondition:	turn the data so that it can be used as corner
	vector<Point> getCorners()const;
	
	//used for define contours' corner
	//precondition:		have the object to apply corners on, and 
	//						have data for a corner
	//postcondition:	turn the data into the corner of contour
	void setCorners(vector<Point> corn);

	//return the bound of each contours
	//precondition:		no access to the private variables, have a value 
	//						that can be turned into bound
	//postcondition:	turn the data so that it can be used as bound
	Rect getBound()const;

	//used for define contours' bound
	//precondition:		have the object to apply corners on, and 
	//						have data for a bound
	//postcondition:	turn the data into the bound of a contour
	void setBound(Rect bou);

	//return the size of each contours
	//precondition:		no access to the private variables, have a value 
	//						that can be turned into contours' area
	//postcondition:	turn the data so that it can be used as area
	//						for the contour
	float getArea()const;

	//used for define contours' size
	//precondition:		have the object to apply corners on, and 
	//						have data for a contour's area 
	//postcondition:	turn the data into the area of contour
	void setArea(float res);

private:
	vector<Point> corners;		//corners from the contours
	Rect bound;					//Rectangle object from openCV that bounds the character
	float area = 0.0;			//area of the contours
};
#endif