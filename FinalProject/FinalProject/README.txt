CSS487 COMPUTER VISION
Professor: Clark Olson
Final Project: Text Recognition Using K Nearest Neighbors
Group Member:
	Andrew Chou
	Nguyen Vi Cao
	Yuze Wang

The project here is separated into two parts:
* Training (train.h or train.cpp)
* Testing (test.h or test.cpp)

** The test.h is make call to datacontours.h functions **
So the program flow is: train.h --> test.h (test.h further makes use of datacontours.h)

How to run the program?
Option 1: do the entire flow
	* Simply build and run the program, it will first perform the training phase (might take a while to complete!)
		* The training involves interaction from users (to click the corresponding character shown on ROI and Resized Windows pop up)
		* NOTE: during the training phase, three windows pop up:
			* ROI and Resize Window: show the letter to be pressed by user for the program to learn
			* Original Image window: show the progress on how many characters shape are learnt.
		* During the training process, make sure to press capitalized character (by SHIFT + Corresponding Character)
			* To quit the program half way, Press ESC.
		* After all characters are learned (every character on Original Image window are boxed!), two xml files will be saved
			* images.xml
			* classifications.xml
			** Both xml files will be used further during the testing phase.
	* Once the training is done, testing is began right away.
		* This will generate the image windows (of testing image) to console
		* Recognized Hidden Character is shown on debug windows.

Option 2: perform the testing only
	* Simply use the xml files being included with submission and put them in the same directory as the class files
	* Comment out three line in main.cpp 
		* Mat training = imread...;
		* Train train_data(training);
		* train_data.oneCallMethod();
	* Then build and run the program as normal.

Training file choices:
* training_chars.png (default image: 5 different fonts)
* training_chars_small_test.png (optional image: only 6 characters here:: ABC123)
* training_chars2.png (4 different font styles)

Testing file choices:
* test1.png (6 characters: ABC123)
* test2.png (6 characters: DEF456)
* test3.png (6 characters: XYZ789)

Files to be included:
* train.h
* train.cpp
* test.h
* test.cpp
* datacontours.h
* datacontours.cpp
* main.cpp
* training_chars.png
* training_chars_small_test.png
* training_chars2.png
* test1.png
* test2.png
* test3.png
* images.xml
* classifications.xml