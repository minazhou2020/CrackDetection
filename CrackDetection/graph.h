/*
 * @file   graph.h
 * @author Yuxiao Zhou
 */

#ifndef CRACK_DETECTION_GRAPH_H_
#define CRACK_DETECTION_GRAPH_H_

#include <utility> 
#include <vector>
#include <algorithm> 
#include <iostream>
#include "disjoint_sets.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

namespace CrackDetection {
	class graph
	{
		typedef pair<int, int> i_pair;
		int v_, e_;
		vector<pair<float, i_pair>> edges_;
	public:
		graph(int v, int e);
		void add_edge(int u, int v, float w);
		int kruskal_MST(vector<Point2f> seeds, Mat image);
		~graph();
	};
}

#endif

