/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */
#include "pch.h"
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/face.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace cv::face;
using namespace std;
static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
	std::ifstream file;
	/*if (!file) {
		string error_message = "No valid input file was given, please check the given filename: ";
		//CV_Error(Error::StsBadArg, error_message);
		cout << error_message << filename<<"\n";
	}*/
	cout << "filename: " << filename << "\n";
	file.open(filename, ios::in);//delete .c_str() after filename
	if (!file.is_open())
		cout << "File cannot open: " << filename << "\n";
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
		else
			cout << "Error! Path directory to login photo filename is empty.\n";
	}
}
bool faceRecn1(string& fnCsv, string& faceLogFn)
{
	// These vectors hold the images and corresponding labels.
	vector<Mat> images;
	vector<int> labels;
	bool facePass = false;
	cout << "csv filename: " << fnCsv << "\n";
	// Read in the data. This can fail if no valid
	// input filename is given.
	try {
		read_csv(fnCsv, images, labels);
	}
	catch (const cv::Exception& e) {
		cerr << "Error opening file \"" << fnCsv << "\". Reason: " << e.msg << endl;
		// nothing more we can do
		exit(1);
	}
	// Quit if there are not enough images for this demo.
	if (images.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(Error::StsError, error_message);
	}
	// The following lines simply get the last images from
	// your dataset and remove it from the vector. This is
	// done, so that the training data (which we learn the
	// cv::LBPHFaceRecognizer on) and the test data we test
	// the model with, do not overlap.
	Mat faceLog = imread(faceLogFn, 0);
	int testLabel = labels[0];
	int predictLabel = -1;
	double predictConfd = 0.0;
	// The following lines create an LBPH model for
	// face recognition and train it with the images and
	// labels read from the given CSV file.
	try {
		Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
		
		model->setThreshold(65.0); //Default threshold value is 123.0
		
		model->train(images, labels);
		// The following line predicts the label of a given
		// test image:
		model->predict(faceLog, predictLabel, predictConfd);
		
		printf("Predicted class = %d / Actual class = %d", predictLabel, testLabel);
		cout << "Confidence value: " << predictConfd << "\n" << std::flush;
		
		if (predictConfd <= 40.0) {
			facePass = true;
			cout << "Your face login is accepted and successful with predicted confidence level: " << predictConfd << "\n";
		}
		else {
			cout << "Your face login is not accepted and failed with predicted confidence level: " << predictConfd << "\n";
		}
	} 
	catch (const cv::Exception& e) {
		cerr << "Exception during prediction: " << e.msg << endl;
	}
	
	return facePass;
}
