#pragma once
class DisjointSets
{
	int *parent, *rnk;
	int n;
public:
	DisjointSets(int n);
	int find(int u);
	void merge(int x, int y);
	~DisjointSets();
};

