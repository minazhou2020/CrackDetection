#pragma once
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

#define CELL_HEIGHT 8
#define CELL_WIDTH 8

class pre_processing
{
	const float threshold_rate_ = 0.6;
	const int intensity_ = 150;
	const int pre_cell = 16;
public:
	void preprocess(Mat &img) const;
	static vector<Mat> split_image(const Mat& img);
};