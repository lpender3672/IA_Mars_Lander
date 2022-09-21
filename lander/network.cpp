#include "network.h"
#include "layer.h"
#include <vector>


network::network(std::vector<int> network_shape) {
	for (int i = 1; i < network_shape.size(); i++) {
		layers.push_back(layer(network_shape[i-1], network_shape[i]));
	}
}