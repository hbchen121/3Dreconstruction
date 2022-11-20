#include "main.h"

void unDistortion(Mat src, Mat &dst){
	//matShow(src, "origianl");
	Mat cameraMat; // = Mat::eye(3, 3, CV_32FC1); // internal parameter
	getIntParam(cameraMat);
	/*
	cameraMat.at<float>(0, 0) = 3128.10395376414; 	// fx
	cameraMat.at<float>(0, 1) = -17.3869888347918; 	// alpha_c*fx
	cameraMat.at<float>(0, 2) = 2015.27614417167; 	// cx
	cameraMat.at<float>(1, 1) = 3122.30855680858; 	// fy
	cameraMat.at<float>(1, 2) = 1171.48985161511; 	// cy
	*/

	Mat distCoeffs = Mat::zeros(5, 1, CV_32FC1); //undistorti 
	distCoeffs.at<float>(0, 0) = 0.409086688899048; 	// k1	
	distCoeffs.at<float>(1, 0) = -2.12468009353960; 	// k2
	distCoeffs.at<float>(2, 0) = 0.00355086180926543; 	// p1	
	distCoeffs.at<float>(3, 0) = 0.00118259447796026; 	// p1
	distCoeffs.at<float>(4, 0) = 3.34621634794969; // k3
	
	Mat view, rview, map1, map2;
	Size imageSize = src.size();
	initUndistortRectifyMap(cameraMat, distCoeffs, Mat(),
		getOptimalNewCameraMatrix(cameraMat, distCoeffs, imageSize, 1, imageSize, 0),
		imageSize, CV_32FC1, map1, map2);
	remap(src, dst, map1, map2, INTER_LINEAR);
	//matShow(dst, "calibration");
	cout << "undistortion Done" << endl;
	return ;
}
