/*
 * @file   main.cpp
 * @brief  entry point of the CrackDetection.exe
 * @author Yuxiao Zhou
 */

#include "crack_seed_detection.h"

 /*
  * read files from a directory and stored as a vector of Matrix
  */
vector<Mat> read_folder(const string& path) {
	vector<Mat> images;
	vector<cv::String> fn;
	glob(path + "/*.jpg", fn, false);
	//number of jpg files in images folder
	const auto count = fn.size();
	for (size_t i = 0; i < count; i++) {
		images.push_back(imread(fn[i]));
	}
	return images;
}

/*
 *entry point of the CrackDetection.exe
 */
int main(int argc, char** argv)
{
	const string path(argv[1]);
	auto images = read_folder(path);

#pragma omp parallel for num_threads(16)
	for (auto i = 0; i < images.size(); i++) {
		Mat src_gray;
		cvtColor(images[i], src_gray, cv::COLOR_BGR2GRAY);
		CrackDetection::pre_processing p;
		p.preprocess(src_gray);
		const auto image_vector = p.split_image(src_gray);
		const auto height = src_gray.rows;
		const auto width = src_gray.cols;
		//split image into vector of cells
		CrackDetection::crack_seed_detection csd;
		auto seeds = csd.convert_to_seed(height / CELL_HEIGHT, width / CELL_WIDTH,
			image_vector, 0.15);
		CrackDetection::graph a_graph = CrackDetection::graph(seeds.size(), seeds.size() * NEAREST_NEIGHBOR);
		//add edge to the graph
		CrackDetection::crack_seed_detection::construct_graph(seeds, a_graph);
		//create a black image with original height and width
		Mat result(height, width, CV_8UC3, Scalar(0, 0, 0));
		//find the MST using kruskal algorithm and draw the path on the black image
		a_graph.kruskal_MST(seeds, result);
		//save the image to the folder
		imwrite(path + "//" + to_string(i) + ".png", result);
	}
}
