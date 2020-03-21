#include "DisjointSets.h"

disjoint_sets::disjoint_sets(const int n)
{
	// Allocate memory 
	this->n_ = n;
	parent_ = new int[n + 1];
	rnk_ = new int[n + 1];
	// Initially, all vertices are in 
	// different sets and have rank 0. 
	for (auto i = 0; i <= n; i++)
	{
		rnk_[i] = 0;
		//every element is parent of itself 
		parent_[i] = i;
	}
}

// Find the parent of a node 'u' 
	// Path Compression 
int disjoint_sets::find(const int u) const
{
	/* Make the parent of the nodes in the path
	   from u--> parent[u] point to parent[u] */
	if (u != parent_[u])
		parent_[u] = find(parent_[u]);
	return parent_[u];
}

void disjoint_sets::merge(int x, int y) const
{
	x = find(x), y = find(y);
	/* Make tree with smaller height
	   a subtree of the other tree  */
	if (rnk_[x] > rnk_[y])
		parent_[y] = x;
	else // If rnk[x] <= rnk[y] 
		parent_[x] = y;

	if (rnk_[x] == rnk_[y])
		rnk_[y]++;
}

disjoint_sets::~disjoint_sets()
= default;
