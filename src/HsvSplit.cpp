#include "main.h"

static Mat matSrc;
static Mat matHsv;
static Mat matBgr;
static Mat matDst;
// h
static int hmin = 0;
static int hmin_max = 360;
static int hmax = 180;
static int hmax_max = 180;

// s
static int smin = 0;
static int smin_max = 255;
static int smax = 255;
static int smax_max = 255;

// value
static int vmin = 106;
static int vmin_max = 255;
static int vmax = 255;
static int vmax_max = 255;

void callBack(int, void*);

void hsvSplit(Mat src, Mat &dst, int flag){
	matSrc = src.clone();
	//matShow(src, "Hsv src img");
	matSrc.convertTo(matBgr, CV_32FC3, 1.0 / 255, 0);
	cvtColor(matSrc, matHsv, CV_BGR2HSV);
	char dstName[] = "dst.jpg";
	//namedWindow(dstName, WINDOW_GUI_EXPANDED);
	
	if(flag){
		// adjust H, S, V
		// adjust H	
		createTrackbar("hmin", dstName, &hmin, hmin_max, callBack);
		createTrackbar("hmax", dstName, &hmax, hmax_max, callBack);
		// adjust S
		createTrackbar("smin", dstName, &smin, smin_max, callBack);
		createTrackbar("smax", dstName, &smax, smax_max, callBack);
		// adjust Value
		createTrackbar("vmin", dstName, &vmin, vmin_max, callBack);
		createTrackbar("vmax", dstName, &vmax, vmax_max, callBack);
	}
	else{
		hmin = 78, hmax = 99;
		smin = 43, smax = 255;
		vmin = 46, vmax = 255;
	}
	callBack(0, 0);
	//waitKey(0);
	cout << "HsvSplit" << endl;
	dst = matDst;
	return ;
}

void callBack(int, void*){
	matDst = Mat::zeros(matSrc.size(), matSrc.type());
	Mat mask;
	inRange(matHsv, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), mask);
	for(int r = 0; r < matDst.rows; r++){
		for(int c = 0; c < matDst.cols; c++){
			if(mask.at<uchar>(r, c) == 255){
				matDst.at<Vec3b>(r, c) = matSrc.at<Vec3b>(r, c);
			}
		}
	}
	//matShow(matDst, "dst.jpg");
	//imshow("dst.jpg", matDst);
	//matDst.convertTo(matDst, CV_8UC3, 255.0, 0); //convert matDst to Binary
	imwrite("HSV_Split.jpg", matDst);
	
}

void approxHsv(Mat src, Mat &dst){
	Mat img = src.clone();
	Mat res = Mat::zeros(src.size(), CV_8UC1);
	cvtColor(img, img, COLOR_BGR2GRAY);
	threshold(img, img, 20, 255, THRESH_BINARY);
	//matShow(img, "bin");
	vector< vector<Point> > contours, newContours;
	vector<Point> newContour;
	vector<Vec4i> hierarcy;
	findContours(img, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i<contours.size(); i++){
		if(contours[i].size() < 300) continue;
		for(int j = 0; j < contours[i].size(); j++){
			newContour.push_back(contours[i][j]);
		}
	}
	newContours.push_back(newContour);
	vector< vector<Point> >contours_poly(contours.size());
    approxPolyDP(Mat(newContours[0]), contours_poly[0], 3, true);
    drawContours(res, contours_poly, 0, Scalar(255, 255, 255), -1);  
    //matShow(res, "approx img");
    dst = res.clone();
    return ;
}
