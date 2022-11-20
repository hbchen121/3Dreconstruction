#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <map>

using namespace std;
using namespace cv;

// main.cpp
void myMain(const char* imgPath);
void matShow(Mat mat, const char* winName);
void matSave(Mat mat, const char* imgpath);
void getIntParam(Mat &intParam);
string toString(int n);

// undistortion.cpp
void unDistortion(Mat src, Mat &dst);

// HsvSplit.cpp
void hsvSplit(Mat src, Mat &dst, int flag);
void approxHsv(Mat src, Mat &dst);

// world coordinate To img coordinate(world2Img.cpp)
void coordMap(Mat img, Mat intParam, Mat extParam, Mat* matWorld, int xs, int length, int ys, int width, int zs, int height);
