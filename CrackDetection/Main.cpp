#include "CrackSeedDetection.h"

vector<Mat> read_folder(const string& path) {
	vector<Mat> images;
	vector<cv::String> fn;
	glob(path+"/*.jpg", fn, false);
	//number of jpg files in images folder
	const auto count = fn.size(); 
	for (size_t i = 0; i < count; i++) {
		images.push_back(imread(fn[i]));
	}	
	return images;
}

int main(char** argv)
{
	const string fold_path(argv[1]);
	auto images = read_folder(fold_path);
	cout << images.size()<<endl;

#pragma omp parallel for num_threads(16)
	for (auto i = 0; i < images.size();i++) {
		cout << images[i].rows << endl;
		Mat src_gray;
		cvtColor(images[i], src_gray, cv::COLOR_BGR2GRAY);
		const auto image_vector = pre_processing::split_image(src_gray);
		imwrite(fold_path + "//pre_" + to_string(i) + ".png", src_gray);
		const auto height = src_gray.rows;
		const auto width = src_gray.cols;
		//split image into vector of cells
		auto seeds = crack_seed_detection::convert_to_seed(height / CELL_HEIGHT, width / CELL_WIDTH,
		                                                 image_vector, 0.15);
		cout << seeds.size() << endl;
		if (seeds.empty())
			continue;
		/*construct a graph
		vertices are acquired from the seeds, which are the potential candidates of crack pixel
		edges are only build between seeds and their five nearest neighbors*/
		graph a_graph = graph(seeds.size(), seeds.size() * NEIGHBOR_NUMBER);
		//add edge to the graph
		crack_seed_detection::find_nearest_neighbor(seeds, a_graph);
		//create a black image with original height and width
		Mat result(height, width, CV_8UC3, Scalar(0, 0, 0));
		//find the MST using kruskal algorithm and draw the path on the black image
		a_graph.kruskal_MST(seeds, result);
		//save the image to the folder
		imwrite(fold_path+"//"+ to_string(i)+".png", result);
	}
}