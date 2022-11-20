#include "main.h"

bool inObject(Mat img, double u, double v);

void coordMap(Mat img, Mat intParam, Mat extParam, Mat* matWorld, int xs, int length, int ys, int width, int zs, int height){
	/*
		param(Mat(r*c, CV_8UC1), int, Mat(CV_32FC1), Mat(CV_32FC1), Mat[Mat(len*wid, CV_8UC3)], int, int, int)
	*/
	cout << "coordMap Done" << endl;
	//cout << "int:\n" << intParam << endl;
	//cout << "ext:\n" << extParam << endl;
	Mat param = intParam * extParam; // M33 * M34 = M34
	//cout << "param:\n" << param << endl;
	Mat coordWorld = Mat::zeros(length * width * height, 4, CV_64FC1); // Mn4
	int i = 0;
	for(int z = 0; z < height; z++){
		for(int x = 0; x < length; x++){
			for(int y = 0; y < width; y++){
				coordWorld.at<double>(Point(0, i)) = xs + x;
				coordWorld.at<double>(Point(1, i)) = ys + y;
				coordWorld.at<double>(Point(2, i)) = zs + z;
				coordWorld.at<double>(Point(3, i)) = 1;
				i++;
			}
		}
	}
	//cout << "param.size:" << param.size() << endl;
	//cout << "param.rows, param.cols:" << param.rows << ", " << param.cols << endl;
	//cout << "coordWorld.t().size:" << coordWorld.t().size() << endl;
	Mat coordImg = param * coordWorld.t(); // M34 * M4n = M3n
	//cout << "coordImg.size:" << coordImg.size() << endl;
	//cout << "coordImg:\n" << coordImg << endl;
	coordImg = coordImg.t(); // Mn3
	//cout << "coordImg.size:" << coordImg.size() << endl;
	i = 0;
	
	Mat tmp = Mat::zeros(img.size(), CV_8UC1);
	
	double maxu = 0, maxv = 0, minu = 0x7fffffff, minv = 0x7fffffff;
	for(int z = 0; z < height; z++){
		for(int x = 0; x < length; x++){
			for(int y = 0; y < width; y++){
				double u = coordImg.at<double>(Point(0, i));
				double v = coordImg.at<double>(Point(1, i));
				double t = coordImg.at<double>(Point(2, i));
				i++;
				u /= t;
				v /= t;
				maxu = maxu > u ? maxu : u;
				minu = minu < u ? minu : u;
				maxv = maxv > v ? maxv : v;
				minv = minv < v ? minv : v;
				//u = u - t + 1;
				//v = v - t + 1;
				//cout << u << ' ' << v <<  ' ' << t << endl;
				if(inObject(img, u, v)){
					//cout << '.';
					//tmp.at<uchar>((int)(u+0.5), (int)(v+0.5)) = 255;
					matWorld[height - z - 1].at<uchar>(Point(y, x)) += 1;
				}
			}
		}
	}
	cout << "maxu, minu:" << maxu << ", " << minu << endl;
	cout << "maxv, minv:" << maxv << ", " << minv << endl;
	cout << "src size" << img.size() << endl;
	//matSave(tmp, "100tmp");
	//matShow(tmp, "test");
	//matShow(img, "test");
	return ;
}

bool inObject(Mat img, double u, double v){
	//cout << '.';
	int i = v, j = u; // integer
	if(i < 0 || i >= img.rows || j < 0 || j >=img.cols){
		return false;
	}
	//matShow(img, "test2");
	if(img.at<uchar>(Point(j, i)) == 255){	
		return true;
	}
	return false;
}

