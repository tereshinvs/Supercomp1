// METIS file generator

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

typedef std::pair<int, int> E;

void small(std::fstream&& out) {
	std::cout << "Starting generating small graph..." << std::endl;

	int num_nodes = 5;

	std::vector<E> edge_array;
	edge_array.push_back(E(0, 2));
	edge_array.push_back(E(1, 3));
	edge_array.push_back(E(1, 4));
	edge_array.push_back(E(2, 1));
	edge_array.push_back(E(2, 3));
	edge_array.push_back(E(3, 4));
	edge_array.push_back(E(4, 0));
	edge_array.push_back(E(4, 1));

	std::vector<float> weights;
	weights.push_back(1.0);
	weights.push_back(1.0);
	weights.push_back(2.0);
	weights.push_back(7.0);
	weights.push_back(3.0);
	weights.push_back(1.0);
	weights.push_back(1.0);
	weights.push_back(1.0);

	std::vector<std::vector<std::pair<int, float>>> adj_list(num_nodes);

	for (int i = 0; i < edge_array.size(); ++i) {
		adj_list[edge_array[i].first].push_back(std::make_pair(edge_array[i].second, weights[i]));
		adj_list[edge_array[i].second].push_back(std::make_pair(edge_array[i].first, weights[i]));
	}

	out << num_nodes << " " << adj_list.size()*2 << " " << 1 << std::endl;

	for (int i = 0; i < adj_list.size(); ++i) {
		for (int j = 0; j < adj_list[i].size(); ++j)
			out << adj_list[i][j].first + 1 << " " << adj_list[i][j].second + 1 << "  ";
		out << std::endl;
	}

	std::cout << "Finish generating small graph" << std::endl;
}

void small_part(std::fstream&& out, int proc_num) {
	int num_nodes = 5;
	for (int i = 0; i < num_nodes; ++i)
		out << i % proc_num << std::endl;
}

void big(std::fstream&& out) {
	std::cout << "Starting generating big graph..." << std::endl;

	int num_nodes = 1000;

	std::vector<std::vector<float>> mat(num_nodes, std::vector<float>(num_nodes));

	std::srand(std::time(nullptr));
	for (int i = 0; i < num_nodes; ++i)
		for (int j = i + 1; j < num_nodes; ++j) {
			float weight = std::rand() / float(RAND_MAX);
			mat[i][j] = mat[j][i] = weight;
		}

	out << num_nodes << " " << num_nodes * (num_nodes - 1) << " " << 1 << std::endl;

	for (int i = 0; i < num_nodes; ++i) {
		for (int j = 0; j < num_nodes; ++j)
			if (i != j)
				out << j + 1 << " " << mat[i][j] << "  ";
		out << std::endl;
	}

	std::cout << "Finish generating big graph" << std::endl;
}

void big_part(std::fstream&& out, int proc_num) {
	int num_nodes = 5000;
	for (int i = 0; i < num_nodes; ++i)
		out << i % proc_num << std::endl;
}

int main(int argc, char* argv[]) {
	small(std::fstream("small.graph", std::ios_base::out));
	big(std::fstream("big.graph", std::ios_base::out));

	small_part(std::fstream("small.part", std::ios_base::out), std::stoi(argv[1]));
	big_part(std::fstream("big.part", std::ios_base::out), std::stoi(argv[1]));
}
