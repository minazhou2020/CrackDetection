#pragma once
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

#define CELL_HEIGHT 8
#define CELL_WIDTH 8

class pre_processing
{
	static const float threshold_rate = 0.6;
	static const int intensity = 150;

public:
	static void preprocess(Mat &img);
	static vector<Mat> split_image(const Mat& img);
};