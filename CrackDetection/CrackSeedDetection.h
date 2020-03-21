#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/utility.hpp>
#include "Graph.h"
#include "PreProcessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm> 
#include <iostream>
#include "opencv2/flann/miniflann.hpp"

using namespace cv;
using namespace std;

#define NEAREST_NEIGHBOR 5

class crack_seed_detection
{
public:
	static vector<Point2f> convert_to_seed(int height_count, int width_count, vector<Mat> cells, float threshold);
	static void find_nearest_neighbor(vector<Point2f>& seeds, graph& g);
private:
	static const int patter_size = 4;
	static const int neighbor_count = 4;
	static const int guide_count = 2;
	static float cell_contrast(int avg, int center, int guide);
	static int is_seed(const vector<Mat>& cells, const int i, const int j, const int center_mean, const int height_count, const float
	                   threshold);

};

