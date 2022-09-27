#include "network.h"
#include "layer.h"
#include <vector>


void network::set_shape(std::vector<int> _network_shape) {
	network_shape = _network_shape;
	for (int i = 1; i < _network_shape.size(); i++) {
		layers.push_back(layer(_network_shape[i-1], _network_shape[i]));
	}
}