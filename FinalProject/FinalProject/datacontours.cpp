//the DataContours for defining contours on the input images
//setter functions are for converting a data to a part of the contour
//getter functions are for changing the data when have no access to the private variables
#include "datacontours.h"
#include <iostream>
#include <stdlib.h>


DataContours::DataContours() {}

//determine whether if that contour is valid
//preconditions: have the area already been setted
//postconditions: return false if the area is smaller then 10x10 pixels,
//	and return ture if area satisfy the target size
bool DataContours::validContours()
{
	if (area < 100)			//we have decided that the cutted roi should be in 10x10 pixels area 
		return false;
	return true;
}

//arrange contours in the order of from left to right
//preconditions: have bounds of a contours
//postconditions: the bounds of a contours are sorted from left to right
//	to be able to used for the contours
bool DataContours::sortEachBound(const DataContours left, const DataContours right)
{
	return (left.bound.x < right.bound.x);
}

//return the corner of each contours
//precondition: no access to the private variables, have a value
//	that can be turned into corner
//postcondition: turn the data so that it can be used as corner
vector<Point> DataContours::getCorners()const
{
	return this->corners;
}

//used for define contours' corner
//precondition: have the object to apply corners on, and 
//	have data for a corner
//postcondition: turn the data into the corner of contour
void DataContours::setCorners(vector<Point> corn)
{
	this->corners = corn;
}

//return the bound of each contours
//precondition: no access to the private variables, have a value 
//	that can be turned into bound
//postcondition: turn the data so that it can be used as bound
Rect DataContours::getBound()const
{
	return this->bound;
}

//used for define contours' bound
//precondition: have the object to apply corners on, and 
//	have data for a bound
//postcondition: turn the data into the bound of a contour
void DataContours::setBound(Rect bou)
{
	this->bound = bou;
}

////return the size of each contours
//precondition: no access to the private variables, have a value 
//	that can be turned into contours' area
//postcondition: turn the data so that it can be used as area
//	for the contour
float DataContours::getArea()const
{
	return this->area;
}

//used for define contours' size
//precondition: have the object to apply corners on, and 
//	have data for a contour's area 
//postcondition: turn the data into the area of contour
void DataContours::setArea(float res)
{
	this->area = res;
}
