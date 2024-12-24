/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */
#include "pch.h"
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "pwdNmLib.h"
#include "Random.h"

// Use the cv and std namespaces
using namespace cv;
using namespace std;
//using namespace fs;
namespace fs = filesystem;
string message{};
bool faceRecn1(string& fn_csv, string& faceLogFn);
//int client(const char* servIpAddr, const char* message);

// A simple program to capture a photo from a camera using OpenCV
string coutMessHdlr() {
	
	return message;
}
bool logFace(const char* userName)
{
	string uName(userName);
	// Define the base directory and subdirectory 
	fs::path baseDir = fs::current_path();
	fs::path subDir1 = "public/images/loginPhoto";
	fs::path subDir2 = "public/images/csv";
	fs::path fullPath1 = baseDir / subDir1;
	fs::path fullPath2 = baseDir / subDir2;
	// Create the directory 
	if (!fs::exists(fullPath1)) { 
	    fs::create_directory(fullPath1); 
		std::cout << "Directory created: " << fullPath1 << std::endl; 
	}
	if (!fs::exists(fullPath2)) {
		 fs::create_directory(fullPath2);
		 std::cout << "Directory created: " << fullPath1 << std::endl;
	}
	fs::path filePath1 = fullPath1 / (uName + ".png");
	fs::path filePath2 = fullPath2 / (uName + ".txt");
	string filePathStr1 = filePath1.string();
	string filePathStr2 = filePath2.string();
	string faceLogFn = filePathStr1; //"1FirstPass/public/images/loginPhoto/" + uName + ".png";
	string csvFn = filePathStr2; //"1FirstPass/public/images/csv/" + uName + ".txt";
	//To validate face login
	if (!faceRecn1(csvFn, faceLogFn))
	{
		message="Face login failed.\nPlease try again!\n";
		//coutMessHdlr();
		return false;
	}
	else
	{
		message="Face login successful.\n";
		//coutMessHdlr();
		return true;
	}
}
string genPwd(int nc, bool cap, bool num, bool sChar)
{
	int capAscii{}, numAscii{}, sCharAscii{};
	vector<char> rd{};
	string pwd{};
	if (cap)
	{
		nc--;
		capAscii = Random::get(65, 90);
		rd.push_back(static_cast<char>(capAscii));
	}
	if (num)
	{
		nc--;
		numAscii = Random::get(48, 57);
		rd.push_back(static_cast<char>(numAscii));
	}
	if (sChar)
	{
		nc--;
		sCharAscii = Random::get(32, 47);
		rd.push_back(static_cast<char>(sCharAscii));
	}
	for (int i = 0; i < nc; ++i)
	{
		rd.push_back(static_cast<char>(Random::get(32, 126)));
	}
	for (int i = 0; i < rd.size(); ++i)
	{
		pwd = pwd + rd[i];
	}
	//cout << "Password: " << pwd << '\n';
	return pwd;
}
/*void regis(string userName, string email)
{
	using namespace cv;
	string message{};
	// Create a VideoCapture object and open the camera device (0 for the default camera)
	VideoCapture cap(0);
	// Check if the camera is opened successfully
	if (!cap.isOpened())
	{
		// Print an error message and return -1
		message= "ERROR: Could not open camera";
		coutMessHdlr(message);
		return;
	}
	string fileType{};
	bool unameDup = true;
	int j = 0;
	coutMessHdlr("You have 5 attempts to register!\n");
	while (unameDup)
	{
		j++;
		string csvFilename = "C:/Users/k_pic/source/repos/pwdNmLib/DataFiles/csv/" + userName + ".txt";
		//To check if userName exists?
		std::ifstream file(csvFilename, ios::in);
		if (file.is_open())
		{
			coutMessHdlr("Your user name is duplicate not valid. Please change to new name.\n");
			if (j == 5)
				return;
		}
		else
		{
			coutMessHdlr("Your user name is valid.\n");
			unameDup = false;
		}
	}
	fileType = ".jpg";
	// Create a Mat object to store the captured frame
	Mat frame;
	// Create a named window to display the captured frame
	cv::namedWindow("Webcam", WINDOW_AUTOSIZE);
	cv::namedWindow("Photo_show", WINDOW_AUTOSIZE);
	coutMessHdlr("Please take 3 photos.\n");
	int n = 0;
	static int label = Random::get(0, 100);
	// Use a loop to capture a frame from the camera and show it on the window
	while (n < 3)
	{
		// Capture a frame from the camera
		cap >> frame;
		// Check if the frame is empty
		if (frame.empty())
		{
			cout <<"ERROR: Frame is empty\n";
			break;
		}
		//Make a copy of the frame
		Mat frame_copy = frame.clone();
		// Get the frame size
		int frame_width = frame.cols;
		int frame_height = frame.rows;
		// Define the rectangle size
		int rect_width = 260;
		int rect_height = 350;
		// Calculate the top-left and bottom-right points of the rectangle
		Point pt1((frame_width - rect_width) / 2, (frame_height - rect_height) / 2); // Centered on the frame
		Point pt2(pt1.x + rect_width, pt1.y + rect_height);
		// Define the color and thickness of the rectangle
		Scalar color = Scalar(255, 0, 0); // Blue
		int thickness = 2;
		// Draw the rectangle on the frame
		cv::rectangle(frame, pt1, pt2, color, thickness);
		// Show the frame on the window
		cv::imshow("Webcam", frame);
		// Wait for a key press
		int key = waitKey(10);
		// If the key is 's', save the frame as a .jpg file using the imwrite function
		if (key == 's')
		{
			// Specify the file name and the compression quality
			//file name path should have user name as folder that store photos of that user
			string num = "0" + to_string(n);
			string csvFilename = "C:/Users/k_pic/source/repos/pwdNmLib/DataFiles/csv/" + userName + ".txt";
			string filename = "C:/Users/k_pic/source/repos/pwdNmLib/DataFiles/facePhotos/" + userName + "Photo" + num + fileType;
			std::ofstream outf{ filename };
			if (!outf)
			{
				cerr << "Could not open file: " << filename << "\n";
			}
			vector<int> compression_params;
			compression_params.push_back(IMWRITE_JPEG_QUALITY);
			compression_params.push_back(95);
			cv::imshow("Photo_show", frame_copy);
			// Save the frame as a .jpg file
			bool success = imwrite(filename, frame_copy, compression_params);
			// Check if the file is saved successfully
			if (success)
			{
				// Print a success message
				cout << "Photo no: " << n << " saved as " << filename << endl;
				ofstream csvf;
				csvf.open(csvFilename, ios::app);
				if (csvf.is_open())
				{
					csvf << filename << "; " << label << "\n";
					cout << "The file " << filename << "has been updated.\n";
				}
				else
				{
					cerr << "Could not open file: " << csvFilename << "\n";
				}
				++n;
			}
			else
			{
				// Print an error message
				cout << "ERROR: Could not save photo" << endl;
			}
		}
		// If the key is ESC, break the loop and exit the program
		if (key == 27)
		{
			break;
		}
	}
	// Release the VideoCapture object and destroy the window
	cap.release();
	cv::destroyAllWindows();
	return;
}*/

/*void menu(string userName, string url)
{
	vector<char> rd{};
	string pwd{};
	cout << "Please select: \n1. Generate new password for a website/app\n2. Login to a website/app\n3. Exit\n";
	int choice{};
	cin >> choice;
	static int nc{}, cap{}, num{}, spec{};
	char yn1{}, yn2{}, yn3{};
	switch (choice)
	{
	case 1:
		cout << "Please specify password requirements.\n1. No. of characters: ";
		cin >> nc;
		cout << "At least one capital letter required (Y or N): ";
		cin >> yn1;
		cout << "At least one number required (Y or N): ";
		cin >> yn2;
		cout << "At least one special character (Y or N): ";
		cin >> yn3;
		if (yn1 == 'Y' || yn1 == 'y')
		{
			nc--;
			cap = Random::get(65, 90);
			rd.push_back(static_cast<char>(cap));
		}
		if (yn2 == 'Y' || yn2 == 'y')
		{
			nc--;
			num = Random::get(48, 57);
			rd.push_back(static_cast<char>(num));
		}
		if (yn3 == 'Y' || yn3 == 'y')
		{
			nc--;
			spec = Random::get(32, 47);
			rd.push_back(static_cast<char>(spec));
		}
		for (int i = 0; i < nc; ++i)
		{
			rd.push_back(static_cast<char>(Random::get(32, 126)));
		}
		for (int i = 0; i < rd.size(); ++i)
		{
			pwd = pwd + rd[i];
		}
		cout << "Password: " << pwd << '\n';
		return;
	case 2:
		return;
	case 3:
		return;
	default:
		return;

	}
}*/

/*void logFace(string userName, string filePath)
{
	using namespace cv;
	// Create a VideoCapture object and open the camera device (0 for the default camera)
	VideoCapture cap(0);

	// Check if the camera is opened successfully
	if (!cap.isOpened())
	{
		// Print an error message and return -1
		coutMessHdlr("ERROR: Could not open camera");
		return;
	}
	string csvFn{}, faceLogFn{}, weburl{};
	bool unameValid = false;
	int j = 0;
	coutMessHdlr("You have 3 attempts to log in!\n");
	while (!unameValid)
	{
		j++;
		string csvFilename = "C:/Users/k_pic/source/repos/pwdNmLib/DataFiles/csv/" + userName + ".txt";
		//To check if userName exists?
		std::ifstream file(csvFilename, ios::in);
		if (file.is_open())
		{
			unameValid = true;
			coutMessHdlr("Your user name exists and is valid.\n");
		}
		else
		{
			coutMessHdlr("Your user name is not correct or exists.\n");
			if (j == 3)
				return;
		}
	}

	//filePath = "Photo.jpg";
	// Create a Mat object to store the captured frame
	Mat frame1;
	int n = 0;
	// Create a named window to display the captured frame
	cv::namedWindow("Webcam", WINDOW_AUTOSIZE);
	cv::namedWindow("Photo_show", WINDOW_AUTOSIZE);
	cout << "You have 3 attempts to take self photos for face identification login.\n";
	while (true)
	{
		// Capture a frame from the camera
		cap >> frame1;

		// Check if the frame is empty
		if (frame1.empty())
		{
			// Print an error message and break the loop
			cout << "ERROR: Frame is empty" << endl;
			break;
		}
		//Make a copy of the frame
		Mat frame1_copy = frame1.clone();

		// Get the frame size
		int frame_width = frame1.cols;
		int frame_height = frame1.rows;

		// Define the rectangle size
		int rect_width = 260;
		int rect_height = 350;

		// Calculate the top-left and bottom-right points of the rectangle
		Point pt1((frame_width - rect_width) / 2, (frame_height - rect_height) / 2); // Centered on the frame
		Point pt2(pt1.x + rect_width, pt1.y + rect_height);

		// Define the color and thickness of the rectangle
		Scalar color = Scalar(255, 0, 0); // Blue
		int thickness = 2;

		// Draw the rectangle on the frame
		cv::rectangle(frame1, pt1, pt2, color, thickness);
		// Display text on frame
		string texts{ "Please align your face inside the rectangular." };
		Point org(10, 30);
		putText(frame1, texts, org, FONT_HERSHEY_PLAIN, 1.0, color, 1, LINE_8, false);
		// Show the frame on the window
		cv::imshow("Webcam", frame1);

		// Wait for a key press
		int key = waitKey(10);
		// If the key is 's', save the frame as a .jpg file using the imwrite function
		if (key == 's')
		{
			// Specify the file name and the compression quality
			//file name path should have user name as folder that store photos of that user
			csvFn = "C:/Users/k_pic/source/repos/pwdNmLib/DataFiles/csv/" + userName + ".txt";
			faceLogFn = "C:/Users/k_pic/source/repos/pwdNmLib/DataFiles/loginPhotos/" + userName + filePath;
			std::ofstream outf{ faceLogFn };
			if (!outf)
			{
				cerr << "Could not open file: " << faceLogFn << "\n";
			}
			vector<int> compression_params;
			compression_params.push_back(IMWRITE_JPEG_QUALITY);
			compression_params.push_back(95);
			cv::imshow("Photo_show", frame1_copy);
			// Save the frame as a .jpg file
			bool success = imwrite(faceLogFn, frame1_copy, compression_params);

			// Check if the file is saved successfully
			if (success)
			{
				// Print a success message
				cout << "Photo has been saved as " << faceLogFn << endl;
			}
			else
			{
				// Print an error message
				cout << "ERROR: Could not save photo." << endl << "Please try again.";
			}
			//To validate face login
			if (!faceRecn(csvFn, faceLogFn))
			{
				++n;
				cout << "Face login failed.\nPlease try again!\n";
				cout << "You have: " << (3 - n) << " attempts remaining.\n";
				if (n == 3)
					break;
			}
			else
			{
				cout << "Face login successful.\n";
				// Display text on frame
				string texts{ "Face login successful." };
				Point org(80, 80);
				putText(frame1_copy, texts, org, FONT_HERSHEY_PLAIN, 1.0, color, 1, LINE_8, false);
				cv::imshow("Photo_show", frame1_copy);
				//menu(userName, weburl);
				break;
			}
		}
		// If the key is ESC, break the loop and exit the program
		if (key == 27)
		{
			break;
		}
	}

	// Release the VideoCapture object and destroy the window
	cap.release();
	cv::destroyAllWindows();
	return;
}*/