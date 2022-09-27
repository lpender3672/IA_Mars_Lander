#pragma once
#include <vector>
#include "layer.h"


class network
{
private:
	
	std::vector<layer> layers;
	std::vector<int> network_shape;

public:
	
	network();

	void set_shape(std::vector<int> _network_shape);
	
};

