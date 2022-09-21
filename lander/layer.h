#pragma once
#include <vector>


class layer
{
private:
	int input_nodes;
	int output_nodes;
	
	std::vector<std::vector<double>> weights;
	std::vector<double> biases;

	std::vector<std::vector<double>> weight_gradients;
	std::vector<double> bias_gradients;
	
public:
	layer(int input_nodes, int output_nodes);
	
};

