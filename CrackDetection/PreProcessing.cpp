#include "PreProcessing.h"

void pre_processing::preprocess(Mat &img)
{
	for (auto y = 0; y + 16 <= img.rows; y += 16)
	{
		for (auto x = 0; x + 16 <= img.cols; x += 16)
		{
			auto temp=img(cv::Rect(x, y, 16, 16));
			//find average mean, min, max
			const int mean = cv::mean(temp)[0];
			double min, max;
			cv::minMaxLoc(temp, &min, &max);
			//find high_theshold value which regarded as exposed pixel
			const auto threshold_high = mean + (max - mean)*THRESHOLD_RATE;
			//find low_theshold value which regarded as crack pixel
			const auto threshold_low = mean - (mean - min)*THRESHOLD_RATE;
			//compute the new average in the range of (low_threshold, high_threshold)
			auto count = 0;
			float total = 0;
			for (auto m = 0; m < 16; ++m) {
				for (auto n = 0; n < 16; ++n) {
					const auto pixel = temp.at<uchar>(m, n);
					if (pixel <= threshold_high and pixel >= threshold_low) {
						total += pixel;
						++count;
					}
				}
			}
			//get the new avg
			const float avg = total / count;
			//find the factor to convert image average value to a constant
			const float factor = INTENSITY / avg;
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



