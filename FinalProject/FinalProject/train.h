// Train.h files
// Class: CSS487
// Group Members:
//      Andrew Chou
//      Nguyen Vi Cao
//      Yuze Wang

#ifndef TRAIN_H_
#define TRAIN_H_
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <vector>
using namespace std;
using namespace cv;

class Train
{
public:
    // Default Constructor for the train class
    // Precondition: No image scanned, no object created
    // Postcondition: the parameter image is saved to private variable "trainingImage"
    Train(const Mat& img);

    // Preprocess method
    // Precondition: image remain the same, no preprocessing done
    // Post condition: image being processed with:
    //                  convert color to grayscale
    //                  blurring the grayscale image with Gaussian, Size(0,0)
    //                  find threshold of the blurred image
    //                  utilize the treshold from previous function call to define contour area
    void preprocess();

    // extract function
    // Precondition: no loop through contours item, no learning on character shape made
    // Postcondition: loop through contour item one by one, showing character to be pressed by user in ROI and resize window, addBound on the character (or contour) being pressed on original input image
    void extract();

    // addBound function
    // Precondition: no rect shape shown even if user pressed key
    // Postcondition: whenever key is pressed, rect shown on the corresponding character representing the character is learned or looped through
    Mat addBound(vector<vector<Point>> ptCon, Mat number, Mat mImgThresh, int mI);

    // store function
    // Precondition: data extracted not stored anywhere
    // Postcondition: save the data extracted from user pressing to .xml extension file storage to be used later on.
    //                  the classification.xml is saving the character that user pressed in decimal code ASCII
    //                  the images.xml is saving all components found on images and save it.
    void store();

    // throwError function
    // Precondition: no exception made, no error searching made
    // Postcondition: make sure the file is able to be opened, if failed, notify users.
    void throwError();

    // oneCallMethod()
    // Precondition: no batch methods called
    // Postcondition: call all required method for train to process, extract and store()
    void oneCallMethod();

private:
    // Mat image object for input image scanned, grayscale, blur, thresh and its copy
    Mat trainingImage, grayScale, gaussian, thresh, threshCopy;
    Mat ClassifyInts;      //these are our training images, note we will have to perform some conversions before writing to file later
                                    //due to the data types that the KNN object KNearest requires, we have to declare a single Mat,
                                    //then append to it as though it's a vector, also we will have to perform some conversions before writing to file later
    Mat trainingFlattenImageFloat;
    vector<vector<Point>> contours;
    vector<Vec4i> contourCorner;
    // all valid characters to be learnt
    vector<int> validChar = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
};
#endif
