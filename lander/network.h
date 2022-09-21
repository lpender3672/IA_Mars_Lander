#pragma once
#include <vector>
#include "layer.h"


class network
{
private:
	
	std::vector<layer> layers;

public:
	
	network(std::vector<int> network_shape);
};

