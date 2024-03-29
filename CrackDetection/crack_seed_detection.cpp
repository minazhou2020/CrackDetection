/*
 * @file   crack_seed_detection.cpp
 * @brief  classified 'cell' as either a non-crack cell or a crack seed by
 *         analyzing brightness changes on neighboring cells.
 *         Four patterns were proposed adopted for crack seed verification.
 * @author Yuxiao Zhou
 */

#include "crack_seed_detection.h"

namespace CrackDetection {
	/*
	 *  classify a cell as either a non-crack cell or a crack seed by analyzing brightness changes on neighboring cells 
	 */
	bool crack_seed_detection::is_seed(const vector<Mat>& cells, const int i, const int j, const int center_mean,
		const int height_count, const float threshold) const
	{
		float max_contrast = 0;
		auto guide_count = 0;
		for (auto m = 0; m < patter_size_; m++)
		{
			float mean_neighbor_total = 0;
			for (auto n = 0; n < neighbor_count_; n++)
			{
				auto neighbor = cells[(i + neighbors_[m*patter_size_ + n].first)*height_count + j + neighbors_[m*patter_size_ + n].second];
				auto mean = cv::mean(neighbor);
				mean_neighbor_total += mean[0];
			}

			for (auto g = 0; g < guide_count_; g++)
			{
				const int guide = mean(cells[(i + guides_[guide_count].first)*height_count + guides_[guide_count++].second])[0];
				max_contrast = max(cell_contrast(mean_neighbor_total / neighbor_count_, center_mean, guide), max_contrast);
			}

		}
		return max_contrast > threshold;
	}

	/*
	 * calculate the contrast of a cell
	 */
	float crack_seed_detection::cell_contrast(const int avg, const int center, const int guide) {
		return static_cast<float>(2 * avg - center - guide) / static_cast<float>(avg);
	}

	/*
	 *  convert cells to seed  
	 */
	vector<Point2f> crack_seed_detection::convert_to_seed(const int height_count, const int width_count, const vector<Mat>&
		cells, const float threshold) const
	{
		auto seeds = vector<Point2f>();
		for (auto i = 2; i < height_count - 2; i++) {
			for (auto j = 2; j < width_count - 2; j++) {
				const auto index = i * width_count + j;
				auto center = cells[index];
				const int center_mean = cv::mean(center)[0];
				if (is_seed(cells, i, j, center_mean, width_count, threshold)) {
					auto p = Point_<float>(j * CELL_WIDTH, i * CELL_HEIGHT);
					seeds.push_back(p);
				}
			}
		}
		return seeds;
	}

	/*
	 * construct a graph:
	 * vertices: verified seed
	 * edges: edges are added between seeds and their five nearest neighbors
	 * weights of edge:euclidean distance between seeds and their neighbors
	 */
	void crack_seed_detection::construct_graph(vector<Point2f>& seeds, graph& g)
	{
		//Insert all 2D points to this vector
		const flann::KDTreeIndexParams index_params;
		vector<bool> bool_vect(seeds.size(), false);
		cv::Mat_<float> features(0, 2);
		for (auto&& point : seeds)
		{
			//Fill matrix
			cv::Mat row = (cv::Mat_<float>(1, 2) << point.x, point.y);
			features.push_back(row);
		}

		flann::Index kd_tree(features, index_params);
		//Insert the 2D point we need to find neighbors to the query
		vector<int> indices;
		vector<float> distances;
		//find the nearest neighbor
		const double threshold = 50 * 50;
		for (auto i = 0; i < seeds.size(); i++)
		{
			cv::Mat query = (cv::Mat_<float>(1, 2) << seeds[i].x, seeds[i].y);
			kd_tree.knnSearch(query, indices, distances, NEAREST_NEIGHBOR <= seeds.size() ? NEAREST_NEIGHBOR : seeds.size());
			if (distances.size() <= 1 || distances[1] > threshold)
				continue;
			for (auto j = 1; j < indices.size(); j++)
			{
				const auto search_index = i;
				const auto find_index = indices[j];
				if (search_index == find_index)
					continue;
				if (distances[i] <= threshold)
					g.add_edge(search_index, find_index, distances[j]);
			}
		}
	}
}

