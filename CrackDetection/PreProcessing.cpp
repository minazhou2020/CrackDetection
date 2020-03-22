#include "PreProcessing.h"

void pre_processing::preprocess(Mat &img) const
{
	for (auto y = 0; y + pre_cell <= img.rows; y += pre_cell)
	{
		for (auto x = 0; x + pre_cell <= img.cols; x += pre_cell)
		{
			auto temp=img(cv::Rect(x, y, pre_cell, pre_cell));
			//find average mean, min, max
			const int mean = cv::mean(temp)[0];
			double min, max;
			cv::minMaxLoc(temp, &min, &max);
			//find high_theshold value which regarded as exposed pixel
			const auto threshold_high = mean + (max - mean)*threshold_rate_;
			//find low_theshold value which regarded as crack pixel
			const auto threshold_low = mean - (mean - min)*threshold_rate_;
			//compute the new average in the range of (low_threshold, high_threshold)
			cv::Mat mask;
			threshold(temp, mask, threshold_high, 255, 4);
			threshold(mask, mask, threshold_low, 255, 3);
			const int new_mean= cv::mean(temp, mask)[0];
			//find the factor to convert image average value to a constant
			const float factor = intensity_ / new_mean;
			//change the image intensity
			temp = temp * factor;
		}
	}
}

vector<Mat> pre_processing::split_image(const Mat& img)
{
	vector<cv::Mat> cell_vector;
	//create cell_vector

	for (auto y = 0; y<= static_cast<int>(img.rows - CELL_HEIGHT); y += CELL_HEIGHT)
	{
		for (auto x = 0; x<= static_cast<int>(img.cols - CELL_WIDTH); x += CELL_WIDTH)
		{
			cell_vector.push_back(img(cv::Rect(x, y, CELL_WIDTH, CELL_HEIGHT)));
		}
	}
	return cell_vector;
}



