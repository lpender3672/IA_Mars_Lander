#include "layer.h"
#include <vector>


layer::layer(int input_nodes, int output_nodes) {
	
	this->input_nodes = input_nodes;
	this->output_nodes = output_nodes;
	
	// Initialize weights and biases
	for (int i = 0; i < output_nodes; i++) {
		weights.push_back(std::vector<double>());
		for (int j = 0; j < input_nodes; j++) {
			weights[i].push_back(0);
		}
		biases.push_back(0);
	}
	
	// Initialize weight and bias gradients
	for (int i = 0; i < output_nodes; i++) {
		weight_gradients.push_back(std::vector<double>());
		for (int j = 0; j < input_nodes; j++) {
			weight_gradients[i].push_back(0);
		}
		bias_gradients.push_back(0);
	}
	
	/*
	// Initialize weight and bias velocities
	for (int i = 0; i < output_nodes; i++) {
		weight_velocities.push_back(std::vector<double>());
		for (int j = 0; j < input_nodes; j++) {
			weight_velocities[i].push_back(0);
		}
		bias_velocities.push_back(0);
	}
	*/
}