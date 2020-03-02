#include "DisjointSets.h"

/*helper data structure class for running the MST algorithm*/

DisjointSets::DisjointSets(int n)
{
	// Allocate memory 
	this->n = n;
	parent = new int[n + 1];
	rnk = new int[n + 1];
	// Initially, all vertices are in different sets and have rank 0. 
	for (int i = 0; i <= n; i++)
	{
		rnk[i] = 0;
		//every element is parent of itself 
		parent[i] = i;
	}
}

// Find the parent of a node 'u' 
int DisjointSets::find(int u)
{
	/* Find the parent of the nodes in the path
	   from u--> parent[u] point to parent[u] */
	if (u != parent[u])
		parent[u] = find(parent[u]);
	return parent[u];
}

void DisjointSets::merge(int x, int y)
{
	x = find(x), y = find(y);
	if (rnk[x] > rnk[y])
		parent[y] = x;
	else // If rnk[x] <= rnk[y] 
		parent[x] = y;

	if (rnk[x] == rnk[y])
		rnk[y]++;
}

DisjointSets::~DisjointSets()
{
}
