#pragma once
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

#define THRESHOLD_RATE 0.6
#define INTENSITY 150
#define CELL_HEIGHT 8
#define CELL_WIDTH 8

class pre_processing
{
public:
	static void preprocess(Mat &img);
	static vector<Mat> split_image(const Mat& img);
};