/*
 * @file   disjoint_sets.cpp
 * @brief  helper class for MST
 * @author Yuxiao Zhou
 */

#include "disjoint_sets.h"

namespace CrackDetection {
	/*
	 * constructor of disjoint set
	 */
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

	/*
	 * Find the parent of a node 'u', path compression
	 */
	int disjoint_sets::find(const int u) const
	{
		if (u != parent_[u])
			parent_[u] = find(parent_[u]);
		return parent_[u];
	}

	/*
	 * merge into one set the set that contains element x and
	 * the set that contains element y (x and y are in different sets).
	 */
	void disjoint_sets::merge(int x, int y) const
	{
		x = find(x), y = find(y);
		if (rnk_[x] > rnk_[y])
			parent_[y] = x;
		else
			parent_[x] = y;

		if (rnk_[x] == rnk_[y])
			rnk_[y]++;
	}

	disjoint_sets::~disjoint_sets()
		= default;
}
