#include "PreProcessing.h"

/*This class is to 
1. standardize image background in order to enhance the accuracy of crack detection.
2. split image into grid of cells (8*8 pixel)
*/

void PreProcessing::preprocess(Mat &img)
{
	for (int y = 0; y + CELL_HEIGHT  <= img.rows; y += CELL_HEIGHT)
	{
		for (int x = 0; x + CELL_WIDTH  <= img.cols; x += CELL_WIDTH )
		{
			cv::Mat temp=img(cv::Rect(x, y, CELL_WIDTH, CELL_HEIGHT ));
			//find average mean, min, max
			int mean = cv::mean(temp)[0];
			double min, max;
			cv::minMaxLoc(temp, &min, &max);
			//find high_theshold value which regarded as exposed pixel
			double threshold_high = mean + (max - mean)*THRESHOLD_RATE;
			//find low_theshold value which regarded as crack pixel
			double threshold_low = mean - (mean - min)*THRESHOLD_RATE;
			//compute the new average in the range of (low_threshold, high_threshold)
			int count = 0;
			float total = 0;
			for (int m = 0; m < CELL_HEIGHT ; ++m) {
				for (int n = 0; n < CELL_WIDTH; ++n) {
					uchar pixel = temp.at<uchar>(m, n);
					if (pixel <= threshold_high && pixel >= threshold_low) {
						total += pixel;
						++count;
					}
				}
			}
			//get the new avg
			float avg = total / count;
			//find the factor to convert image average value to a constant
			float factor = INTENSITY / avg;
			//change the image intensity
			temp = temp * factor;
		}
	}
}

vector<Mat> PreProcessing::split_image(const Mat img)
{
	int height_count = img.rows / CELL_HEIGHT;
	int width_count = img.cols / CELL_WIDTH;
	vector<cv::Mat> cell_vector;
	//create cell_vector

	for (int y = 0; y<= (int)(img.rows-CELL_HEIGHT); y += CELL_HEIGHT)
	{
		for (int x = 0; x<= (int)(img.cols-CELL_WIDTH); x += CELL_WIDTH)
		{
			cell_vector.push_back(img(cv::Rect(x, y, CELL_WIDTH, CELL_HEIGHT)));
		}
	}
	return cell_vector;
}
