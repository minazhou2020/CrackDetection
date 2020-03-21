#include "CrackSeedDetection.h"

/* Crack Seed Verification We employed the Grid Cell Analysis (GCA) method described in for seed detection, 
as this algorithm is capable of improving the detection speed by reducing the size of search space. 
A preprocessed image was divided into grid cells of 8×8 pixels, and the cells were classified as either a non-crack cell or a 
crack seed by analyzing brightness changes on neighboring cells. */

//coordinates of four neighbor cells in the eight proposed pattern 
const int trans_n_x[4] = { 0, 0, 0, 0 };
const int trans_n_y[4] = { -2, -1, 1, 2 };
const int longi_n_x[4] = { -2, -1, 1, 2 };
const int longi_n_y[4] = { 0, 0, 0, 0 };
const int backward_x[4] = { -2, -1, 1, 2 };
const int backward_y[4] = { -2, -1, 1, 2 };
const int forward_x[4] = { -2, -1, 1, 2 };
const int forward_y[4] = { 2, 1, -1, -2 };

// caculate the average intensity of four neighbor cells of each seed
int CrackSeedDetection::neighbor_mean_total(const int x_coors[], const int y_coors[], const vector<Mat> cells, const int i, const int j, const int width_count) {
	int mean_neighbor_total = 0;
	for (int m = 0; m < NEIGHBOR_NUMBER; m++) {
		Mat n = cells[(i + x_coors[m])*width_count + j+ y_coors[m]];
		Scalar mean = cv::mean(n);
		mean_neighbor_total += mean[0];
	}
	return mean_neighbor_total/NEIGHBOR_NUMBER;
}

// compute and assign the contrast to each cell
float CrackSeedDetection::cell_constrast(const int avg, const int center, const int guide) {
	return (float)(2 * avg - center - guide) / (float)avg;
}

// create a vector of seeds to maintain the verified seed cells
vector<Point2f> CrackSeedDetection::convert_to_seed(const int height_count, const int width_count, vector<Mat>& cells, const float threshold)
{
	vector<Point2f> seeds = vector<Point2f>();
	for (int i = NEIGHBOR_NUMBER /2; i < height_count - NEIGHBOR_NUMBER/2; i++) {
		for (int j = NEIGHBOR_NUMBER/2; j < width_count - NEIGHBOR_NUMBER /2; j++) {
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

// find the five nearest neighbor of the seed cells and add edge between the seed cells and their five neighbors
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
	for (int i = 0; i < seeds.size(); i++)
	{
		cv::Mat query = (cv::Mat_<float>(1, 2) << seeds[i].x, seeds[i].y);
		kdtree.knnSearch(query, indices, dists, NEIGHBOR_NUMBER <= seeds.size() ? NEIGHBOR_NUMBER : seeds.size());
		if (dists.size() <= 1 || dists[1] > THRESHOLD_RATE)
			continue;
		for (int j = 1; j < indices.size(); j++)
		{
			int search_index = i;
			int find_index = indices[j];
			if (search_index == find_index)
			{
				continue;
			}
			if (dists[i]<=DISTANCE_THRESHOLD)
				g.addEdge(search_index, find_index, dists[j]);
		}
	}

}

// verify if a cell is a seed cell
int CrackSeedDetection::is_seed(const vector<Mat> cells, const int center_mean, const int i, const int j, const int width_count, const float threshold) {
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

