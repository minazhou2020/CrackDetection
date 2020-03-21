#include "CrackSeedDetection.h"

int trans_n_x[4] = { 0, 0, 0, 0 };
int trans_n_y[4] = { -2, -1, 1, 2 };
int longi_n_x[4] = { -2, -1, 1, 2 };
int longi_n_y[4] = { 0, 0, 0, 0 };
int backward_x[4] = { -2, -1, 1, 2 };
int backward_y[4] = { -2, -1, 1, 2 };
int forward_x[4] = { -2, -1, 1, 2 };
int forward_y[4] = { 2, 1, -1, -2 };

int crack_seed_detection::neighbor_mean_total(const int x_coors[], int y_coors[], vector<Mat> cells, const int i, const int j, const int height_count) {
	auto mean_neighbor_total = 0;
	for (auto m = 0; m < 4; m++) {
		auto n = cells[(i + x_coors[m])*height_count + j+ y_coors[m]];
		auto mean = cv::mean(n);
		mean_neighbor_total += mean[0];
	}
	return mean_neighbor_total/4;
}

float crack_seed_detection::cell_contrast(const int avg, const int center, const int guide) {
	return static_cast<float>(2 * avg - center - guide) / static_cast<float>(avg);
}

int crack_seed_detection::is_seed(vector<Mat> cells, const int center_mean, const int i, const int j,
                                  const int height_count, const float threshold)
{
	float max_contrast = 0;
	const int g1 = mean(cells[(i - 1)*height_count + j])[0];
	const int g2 = mean(cells[(i + 1)*height_count + j])[0];
	const auto trans_mean_total = neighbor_mean_total(trans_n_x, trans_n_y, cells, i, j, height_count);
	max_contrast = max(cell_contrast(trans_mean_total, center_mean, g1), max_contrast);
	max_contrast = max(cell_contrast(trans_mean_total, center_mean, g2),max_contrast);
	const int g3 = mean(cells[i*height_count + j + 1])[0];
	const int g4 = mean(cells[i*height_count + j - 1])[0];
	const int longi_mean_total = neighbor_mean_total(longi_n_x, longi_n_y, cells, i, j, height_count);
	max_contrast = max(cell_contrast(longi_mean_total, center_mean, g3),max_contrast);
	max_contrast = max(cell_contrast(longi_mean_total, center_mean, g4), max_contrast);
	const int g5 = mean(cells[(i - 1)*height_count + j + 1])[0];
	const int g6 = mean(cells[(i + 1)*height_count + j - 1])[0];
	const auto back_mean_total = neighbor_mean_total(backward_x, backward_y, cells, i, j, height_count);
	max_contrast = max(cell_contrast(back_mean_total, center_mean, g5), max_contrast);
	max_contrast = max(cell_contrast(back_mean_total, center_mean, g6), max_contrast);
	const int g7 = mean(cells[(i - 1)*height_count + j - 1])[0];
	const int g8 = mean(cells[(i + 1)*height_count + j + 1])[0];
	auto forward_mean_total = neighbor_mean_total(forward_x, forward_y, cells, i, j, height_count);
	max_contrast = max(cell_contrast(back_mean_total, center_mean, g7), max_contrast);
	max_contrast=max(cell_contrast(back_mean_total, center_mean, g8), max_contrast);
	return max_contrast > threshold;
}



vector<Point2f> crack_seed_detection::convert_to_seed(int height_count, int width_count, vector<Mat> cells, float threshold)
{
	vector<Point2f> seeds = vector<Point2f>();
	for (auto i = 2; i < height_count - 2; i++) {
		for (auto j = 2; j < width_count - 2; j++) {
			const int index = i * width_count + j;
			auto center = cells[index];
			const int center_mean = cv::mean(center)[0];
			if (is_seed(cells, center_mean, i, j, width_count, threshold)) {
				Point_<float> p = Point_<float>(j * CELL_WIDTH, i * CELL_HEIGHT);
				seeds.push_back(p);
			}
		}
	}
	return seeds;
}

void crack_seed_detection::find_nearest_neighbor(vector<Point2f>& seeds, graph& g)
{
	//Insert all 2D points to this vector
	const flann::KDTreeIndexParams indexParams;
	vector<bool> bool_vect(seeds.size(), false);
	cv::Mat_<float> features(0, 2);
	for (auto&& point : seeds)
	{
		//Fill matrix
		cv::Mat row = (cv::Mat_<float>(1, 2) << point.x, point.y);
		features.push_back(row);
	}

	flann::Index kdtree(features, indexParams);
	//Insert the 2D point we need to find neighbors to the query
	vector<int> indices;
	vector<float> dists;
	//find the nearest neighbor
	const double threshold = 50 * 50;
	for (auto i = 0; i < seeds.size(); i++)
	{
		cv::Mat query = (cv::Mat_<float>(1, 2) << seeds[i].x, seeds[i].y);
		kdtree.knnSearch(query, indices, dists, NEIGHBOR_NUMBER <= seeds.size() ? NEIGHBOR_NUMBER : seeds.size());
		if (dists.size() <= 1 || dists[1] > threshold)
			continue;
		for (auto j = 1; j < indices.size(); j++)
		{
			const auto search_index = i;
			const auto find_index = indices[j];
			if (search_index == find_index)
			{
				continue;
			}
			if (dists[i]<=threshold)
				g.add_edge(search_index, find_index, dists[j]);
		}
	}

}

