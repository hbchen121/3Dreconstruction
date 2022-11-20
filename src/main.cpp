#include "main.h"

void getImg(Mat &matUndistort, Mat &matSplit, int i);
void getIntParam(Mat &intParam);

static vector<Mat> extParams;
void getExtParam(int num);
void getExtParam(Mat &extParam, int num);

void coordVote(Mat *matWorld,int height,int imgNum);
void world3dPointSave(Mat *matl, int h);

int main(int argc, char** argv){
	cout << "Done" << endl;
	int imgNum = 9;
	Mat src, matUndistort, hsv, approx;
	bool split = true;
	// split
	for(int i = 1; i <= imgNum; i++){
		if(split)	break;
		string path = "./bdb/" + toString(i) + ".jpg";
		cout << path << endl;
		//return 0;
		src = imread(path.c_str());
		if(!src.data){
			cout << path << " don't exist" << endl;
			continue ;
		}
		//matShow(src, "orginal image");
		//undistortion
		unDistortion(src, matUndistort);
		matSave(matUndistort, (toString(i) + "_undistort").c_str());
		//matShow(matUndistort, "undistortion image");
		// split
		hsvSplit(matUndistort, hsv, 0);
		//matShow(hsv, "hsv split");
		approxHsv(hsv, approx);

		//matShow(approx, "split region image");
		matSave(approx, toString(i).c_str());
		//break;
	}
	//return 0;

	getExtParam(imgNum);

	// build world coordinate
	int xs = -50, length = 250; 	// x [-50, 250]	-50~250
	int ys = -400, width = 300; 		// y [-250, -50] -50~-250
	int zs = -100, height = 100; 	// z[-100, 0] 50~-100


	Mat matWorld[height];
	for(int h = 0; h < height; h++){
		matWorld[h] = Mat::zeros(length, width, CV_8UC1);
	}


	Mat intParam, extParam;
	getIntParam(intParam);
	cout << "iniParam:\n" << intParam << endl;
	for(int i = 1; i <= imgNum; i++){
		Mat matSplit;
		getImg(matUndistort, matSplit, i);
		cvtColor(matSplit, matSplit, CV_BGR2GRAY);
		//cout << matSplit.channels() << endl;
		//return 0;
		// get internal/external parameter
		getExtParam(extParam, i);
		cout << "extParam:\n" << extParam << endl;
		// get world coordinate
		cout << "get world coordinate" << endl;
		coordMap(matSplit, intParam, extParam, matWorld, xs, length, ys, width, zs, height);
		cout << i << " done" << endl;
		//cout << matUndistort.rows << ' ' << matUndistort.cols << endl;
		//matShow(matUndistort, "1");
		//matShow(matSplit, "2");
		//if(i == 5)	return 0;
		//return 0;
	}
	//return 0;
	coordVote(matWorld, height, imgNum);
	//return 0;
	// save matWorld
	world3dPointSave(matWorld, height);
	for(int h = 0; h < height; h++){
		imwrite(("./3dSlice/" + toString(h) + ".jpg").c_str(), matWorld[h]);
	}
	//cout << "matWorld\n" <<  matWorld[0] << endl;

	return 0;
}

void coordVote(Mat *matWorld,int height,int imgNum){
	imgNum = 9; // because the orginal images are not good enough, so max value can't equal to imgNum(14), and we let it 9, which is the max value;
	int max = 0;
	for(int h = 0; h < height; h++){
		Mat mat = matWorld[h];
		for(int r = 0; r < mat.rows; r++){
			for(int c = 0; c < mat.cols; c++){
				max = max > mat.at<uchar>(Point(c, r)) ? max : mat.at<uchar>(Point(c, r));
				mat.at<uchar>(Point(c, r)) = (mat.at<uchar>(Point(c, r)) == imgNum) ? 255 : 0;
			}
		}
	}
	cout << "Vote done, Max:" << max << endl;
	return ;
}

void world3dPointSave(Mat* matl, int h){
	string path = "./params/3dPoint.txt";
	fstream outfile;
	outfile.open(path.c_str(), ios::out);
	if(!outfile){
		cout << "open " + path + " error" << endl;
		exit(1);
	}
	int i = 0;
	for(int z = 0; z < h; z++){
		Mat mat = matl[z];
		for(int r = 0; r < mat.rows; r++){
			for(int c = 0; c < mat.cols; c++){
				if(mat.at<uchar>(Point(c, r)) != 0){
					i++;
					outfile << r << ' ' << c << ' ' << z << endl;
				}
			}
		}
	}
	outfile.close();
	cout << "params 3d point saved, num:" << i << endl;
	return ;
}

void matShow(Mat mat, const char* winName){
	//namedWindow(winName, WINDOW_AUTOSIZE);
	namedWindow(winName, 0);
	cvResizeWindow(winName, 900, 600);
	imshow(winName, mat);
	waitKey(0);
	return ;
}

void matSave(Mat mat, const char* imgPath){
	string s = imgPath;
	string path = "./bdb/" + s + "_split.jpg";
	imwrite(path.c_str(), mat);
	cout << "save img " + path << endl;
	return ;
}

void getImg(Mat &matUndistort, Mat &matSplit, int i){
	string path = "./bdb/" + toString(i);
	//matUndistort = imread((path + "_undistort_split.jpg").c_str());
	matSplit = imread((path + "_split.jpg").c_str());
	return ;
}

void getIntParam(Mat &intParam){
	cout << "getIntParam test" << endl;
	// load Internal parameter only once
	intParam = Mat::eye(3, 3, CV_64FC1);
	intParam.at<double>(Point(0, 0)) = 3128.10395376757; 	// fx
	intParam.at<double>(Point(1, 0)) = -17.3869888372754; 	// alpha_c*fx
	intParam.at<double>(Point(2, 0)) = 2015.27614418319; 	// cx
	intParam.at<double>(Point(1, 1)) = 3122.30855680612; 	// fy
	intParam.at<double>(Point(2, 1)) = 1171.48985162251; 	// cy
	return ;
}

void getExtParam(int num){
	extParams.clear();
	fstream rfile, tfile;
	rfile.open("./params/RotationMatrices.txt", ios::in);
	tfile.open("./params/TranslationVector.txt", ios::in);
	if(!rfile || !tfile){
		cout << "open error" << endl;
		exit(1);
	}
	for(int i = 0; i < num; i++){
		Mat extParam = Mat::zeros(3, 4, CV_64FC1);
		rfile >> extParam.at<double>(Point(0,0)) >> extParam.at<double>(Point(0,1)) >> extParam.at<double>(Point(0,2));
		rfile >> extParam.at<double>(Point(1,0)) >> extParam.at<double>(Point(1,1)) >> extParam.at<double>(Point(1,2));
		rfile >> extParam.at<double>(Point(2,0)) >> extParam.at<double>(Point(2,1)) >> extParam.at<double>(Point(2,2));
		tfile >> extParam.at<double>(Point(3,0)) >> extParam.at<double>(Point(3,1)) >> extParam.at<double>(Point(3,2));
		//cout << extParam << endl;
		extParams.push_back(extParam);
	}
	rfile.close();
	tfile.close();
	cout << "getExtParam Done" << endl;
	return ;
}

void getExtParam(Mat &extParam, int num){
	cout << "getExtParam test" << endl;
	// load External parameter every image
	extParam = extParams[num-1];
	return ;
}

string toString(int n){
	stringstream ss;
	ss << n;
	string s;
	ss >> s;
	return s;
}

