#pragma once
#include "network.h"

class agent
{
private:
	network chromosome;
	
public:
	agent(int inputs, int outputs, int genes);

	float fitness(void);

};

