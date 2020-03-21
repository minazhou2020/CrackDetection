#pragma once
class disjoint_sets
{
	int *parent_;
	int *rnk_;
	int n_;
public:
	explicit disjoint_sets(int n);
	int find(int u) const;
	void merge(int x, int y) const;
	~disjoint_sets();
};


