#include "agent.h"
#include "network.h"
#include  <vector>

agent::agent(int inputs, int outputs, int genes) {
	std::vector<int> temp = { inputs, genes, outputs };
	chromosome.set_shape(temp);
}


float agent::fitness() {
	
	// use chromosome model to evaluate spacecraft agents fitness
	// integrate through

	// fitness = ~ time of flight - 1000 * crashed?

	return 1.f;
}