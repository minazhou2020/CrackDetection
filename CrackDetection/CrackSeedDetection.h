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

class crack_seed_detection
{
public:
	static vector<Point2f> convert_to_seed(int height_count, int width_count, vector<Mat> cells, float threshold);
	static void find_nearest_neighbor(vector<Point2f>& seeds, graph& g);
private:
	static int neighbor_mean_total(const int x_coors[], int y_coors[], vector<Mat> cells, int i, int j, int height_count);
	static float cell_contrast(int avg, int center, int guide);
	static int is_seed(vector<Mat> cells, int center_mean, int i, int j, int height_count, float threshold);

};

