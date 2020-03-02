#include "Graph.h"

Graph::Graph(int V, int E)
{
	this->V = V;
	this->E = E;
}

void Graph::addEdge(int u, int v, float w)
{
	edges.push_back({ w, {u, v} });
}

int Graph::kruskalMST(vector<Point2f> seeds, Mat image)
{
	int mst_wt = 0; // Initialize result 
	// Sort edges in increasing order on basis of cost 
	sort(edges.begin(), edges.end());
	// Create disjoint sets 
	DisjointSets ds(V);
	// Iterate through all sorted edges 
	vector< pair<float, iPair> >::iterator it;
	for (it = edges.begin(); it != edges.end(); it++)
	{
		int u = it->second.first;
		int v = it->second.second;

		int set_u = ds.find(u);
		int set_v = ds.find(v);

		// Check if the selected edge is creating 
		// a cycle or not 
		if (set_u != set_v)
		{
			// draw lines between connected nodes
			line(image, seeds[u], seeds[v], Scalar(255, 0, 0), 2, 8);
			// Update MST weight 
			mst_wt += it->first;
			// Merge two sets 
			ds.merge(set_u, set_v);
		}
	}
	return mst_wt;
}

Graph::~Graph()
{
}
