#ifndef __FUDANDNN_RELU_H
#define __FUDANDNN_RELU_H
#include "utils.h"
#include "NonLinearLayer.h"

class ReLU : public NonLinearLayer
{

public:
	ReLU(size_t size);
	ReLU() {};
	string getNetworkName() override;
	void compute();
	void calculate();
	void gradient();
	size_t getSize();
};


#endif