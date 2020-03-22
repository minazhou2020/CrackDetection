/*
 * @file   CrackSeedDetection.h
 * @author Yuxiao Zhou
 */
#ifndef CRACK_DETECTION_CRACK_SEED_DETECTION_H_
#define CRACK_DETECTION_CRACK_SEED_DETECTION_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/utility.hpp>
#include "pre_processing.h"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm> 
#include <iostream>
#include "opencv2/flann/miniflann.hpp"
#include "graph.h"

using namespace cv;
using namespace std;

#define NEAREST_NEIGHBOR 5

namespace CrackDetection {
	class crack_seed_detection
	{
	public:
		vector<Point2f> convert_to_seed(int height_count, int width_count, const vector<Mat>& cells, float threshold) const;
		static void construct_graph(vector<Point2f>& seeds, graph& g);
	private:
		const int patter_size_ = 4;
		const int neighbor_count_ = 4;
		const int guide_count_ = 2;
		const vector<pair<int, int>> neighbors_ = {
			{0, -2}, {0, -1}, {0, 1}, {0, 2}, {-2, 0}, {-1, 0}, {1, 0}, {2, 0}, {-2, -2}, {-1, -1}, {1, 1}, {2, 2},
			{-2, 2}, {-1, 1}, {1, -1}, {2, -2}
		};
		const vector<pair<int, int>> guides_ = { {-1,0}, {1,0}, {0,1}, {0,-1},{-1,1}, {1,-1}, {-1,-1}, {1,1}};
		static float cell_contrast(int avg, int center, int guide);
		bool is_seed(const vector<Mat>& cells, int i, int j, int center_mean, int height_count, float threshold) const;
	};
}

#endif
