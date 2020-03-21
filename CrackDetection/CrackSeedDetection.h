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

#define NEIGHBOR_NUMBER 4
#define DISTANCE_THRESHOLD 2500

class CrackSeedDetection
{
public:
	static vector<Point2f> convert_to_seed(const int height_count, const int width_count, vector<Mat>& cells, float threshold);
	static void find_nearest_neighbor(vector<Point2f>& seeds, Graph& g);
private:
	static int neighbor_mean_total(const int x_coors[], const int y_coors[], const vector<Mat> cells, const int i, const int j, const int height_count);
	static float cell_constrast(const int avg, const int center, const int guide);
	static int is_seed(const vector<Mat> cells, const int center_mean, const int i, const int j, const int height_count, const float threshold);

};
