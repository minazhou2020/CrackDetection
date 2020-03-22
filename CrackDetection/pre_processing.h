/*
 * @file   pre_processing.h
 * @author Yuxiao Zhou
 */

#ifndef CRACK_DETECTION_PREPROCESSING_H_
#define CRACK_DETECTION_PREPROCESSING_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#define CELL_HEIGHT 8
#define CELL_WIDTH 8

namespace CrackDetection {
	class pre_processing
	{
	public:
		void preprocess(Mat &img) const;
		static vector<Mat> split_image(const Mat& img);
	private:
		const float threshold_rate_ = 0.6;
		const int intensity_ = 150;
		const int pre_cell = 16;
	};
}

#endif