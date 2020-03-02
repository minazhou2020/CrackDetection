#include <utility> 
#include <vector>
#include <algorithm> 
#include <iostream>
#include "DisjointSets.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
  
using namespace std;
using namespace cv;

class Graph
{
	typedef pair<int, int> iPair;
	int V, E;
	vector<pair<float, iPair>> edges;
public:
	Graph(int V, int E);
	void addEdge(int u, int v, float w);
	int kruskalMST(vector<Point2f> seeds, Mat image);
	~Graph();
};

