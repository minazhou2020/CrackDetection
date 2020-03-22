/*
 * @file   graph.cpp
 * @brief  construct graph and find the MST from the constructed graph using Kruskal's algorithm
 * @author Yuxiao Zhou
 */

#include "graph.h"

 /*
  * constructor of graph
  */
namespace CrackDetection {
	graph::graph(const int v, const int e)
	{
		this->v_ = v;
		this->e_ = e;
	}

	/*
	 * add the edge to graph
	 */
	void graph::add_edge(int u, int v, const float w)
	{
		edges_.push_back({ w, {u, v} });
	}

	/*
	 * use Kruskal's algorithm to find the MST from the constructed graph.
	 * Kruskal's algorithm processes the edges in order of their weight values
	 * (smallest to largest), taking for the MST each edge that
	 * does not form a cycle with edges previously added, stopping after adding
	 * V-1 edges.
	 */
	int graph::kruskal_MST(vector<Point_<float>> seeds, Mat image)
	{
		// Initialize result
		auto mst_wt = 0;
		// Sort edges in increasing order on basis of cost 
		sort(edges_.begin(), edges_.end());
		// Create disjoint sets 
		const disjoint_sets ds(v_);
		for (auto& edge : edges_)
		{
			const auto u = edge.second.first;
			const auto v = edge.second.second;

			const auto set_u = ds.find(u);
			const auto set_v = ds.find(v);

			// Check if the selected edge is creating 
			// a cycle or not (Cycle is created if u 
			// and v belong to same set) 
			if (set_u != set_v)
			{
				// draw lines between connected nodes
				line(image, seeds[u], seeds[v], Scalar(255, 0, 0), 2, 8);
				// Update MST weight 
				mst_wt += edge.first;
				// Merge two sets 
				ds.merge(set_u, set_v);
			}
		}
		return mst_wt;
	}

	graph::~graph()
		= default;
}
