/*
 * @file   disjoint_sets.h
 * @author Yuxiao Zhou
 */

#ifndef CRACK_DETECTION_DISJOINTSET_H_
#define CRACK_DETECTION_DISJOINTSET_H_

namespace CrackDetection {
	class disjoint_sets
	{
	public:
		explicit disjoint_sets(int n);
		int find(int u) const;
		void merge(int x, int y) const;
		~disjoint_sets();
	private:
		int *parent_;
		int *rnk_;
		int n_;
	};
}

#endif

