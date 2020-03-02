#include "CrackSeedDetection.h"

int trans_n_x[4] = { 0, 0, 0, 0 };
int trans_n_y[4] = { -2, -1, 1, 2 };
int longi_n_x[4] = { -2, -1, 1, 2 };
int longi_n_y[4] = { 0, 0, 0, 0 };
int backward_x[4] = { -2, -1, 1, 2 };
int backward_y[4] = { -2, -1, 1, 2 };
int forward_x[4] = { -2, -1, 1, 2 };
int forward_y[4] = { 2, 1, -1, -2 };

int CrackSeedDetection::neighbor_mean_total(int x_coors[], int y_coors[], vector<Mat> cells, int i, int j, int height_count) {
	int mean_neighbor_total = 0;
	for (int m = 0; m < 4; m++) {
		Mat n = cells[(i + x_coors[m])*height_count + j+ y_coors[m]];
		Scalar mean = cv::mean(n);
		mean_neighbor_total += mean[0];
	}
	return mean_neighbor_total/4;
}

float CrackSeedDetection::cell_constrast(int avg, int center, int guide) {
	return (float)(2 * avg - center - guide) / (float)avg;
}

int CrackSeedDetection::is_seed(vector<Mat> cells, int center_mean, int i, int j, int width_count, float threshold) {
	float max_contrast = 0;
	int g1 = mean(cells[(i - 1)*width_count + j])[0];
	int g2 = mean(cells[(i + 1)*width_count + j])[0];
	int trans_mean_total = neighbor_mean_total(trans_n_x, trans_n_y, cells, i, j, width_count);
	max_contrast = max(cell_constrast(trans_mean_total, center_mean, g1), max_contrast);
	max_contrast = max(cell_constrast(trans_mean_total, center_mean, g2),max_contrast);
	int g3 = mean(cells[i*width_count + j + 1])[0];
	int g4 = mean(cells[i*width_count + j - 1])[0];
	int longi_mean_total = neighbor_mean_total(longi_n_x, longi_n_y, cells, i, j, width_count);
	max_contrast = max(cell_constrast(longi_mean_total, center_mean, g3),max_contrast);
	max_contrast = max(cell_constrast(longi_mean_total, center_mean, g4), max_contrast);
	int g5 = mean(cells[(i - 1)*width_count + j + 1])[0];
	int g6 = mean(cells[(i + 1)*width_count + j - 1])[0];
	int back_mean_total = neighbor_mean_total(backward_x, backward_y, cells, i, j, width_count);
	max_contrast = max(cell_constrast(back_mean_total, center_mean, g5), max_contrast);
	max_contrast = max(cell_constrast(back_mean_total, center_mean, g6), max_contrast);
	int g7 = mean(cells[(i - 1)*width_count + j - 1])[0];
	int g8 = mean(cells[(i + 1)*width_count + j + 1])[0];
	int forward_mean_total = neighbor_mean_total(forward_x, forward_y, cells, i, j, width_count);
	max_contrast = max(cell_constrast(back_mean_total, center_mean, g7), max_contrast);
	max_contrast=max(cell_constrast(back_mean_total, center_mean, g8), max_contrast);
	return max_contrast > threshold;
}



vector<Point2f> CrackSeedDetection::convert_to_seed(int height_count, int width_count, vector<Mat> cells, float threshold)
{
	vector<Point2f> seeds = vector<Point2f>();
	for (int i = 2; i < height_count - 2; i++) {
		for (int j = 2; j < width_count - 2; j++) {
			int index = i * width_count + j;
			Mat center = cells[index];
			int center_mean = cv::mean(center)[0];
			if (is_seed(cells, center_mean, i, j, width_count, threshold)) {
				Point2f p = Point2f(j*CELL_WIDTH, i*CELL_HEIGHT);
				seeds.push_back(p);
			}
		}
	}
	return seeds;
}

void CrackSeedDetection::find_nearest_neighbor(vector<Point2f>& seeds, Graph& g)
{
	//Insert all 2D points to this vector
	flann::KDTreeIndexParams indexParams;
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
	double threshold = 50 * 50;
	for (int i = 0; i < seeds.size(); i++)
	{
		cv::Mat query = (cv::Mat_<float>(1, 2) << seeds[i].x, seeds[i].y);
		kdtree.knnSearch(query, indices, dists, NEIGHBOR_NUMBER <= seeds.size() ? NEIGHBOR_NUMBER : seeds.size());
		if (dists.size() <= 1 || dists[1] > threshold)
			continue;
		for (int j = 1; j < indices.size(); j++)
		{
			int search_index = i;
			int find_index = indices[j];
			if (search_index == find_index)
			{
				continue;
			}
			if (dists[i]<=threshold)
				g.addEdge(search_index, find_index, dists[j]);
		}
	}

}

