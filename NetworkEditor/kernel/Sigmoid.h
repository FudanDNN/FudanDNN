#ifndef __FUDANDNN_SIGMOID_H
#define __FUDANDNN_SIGMOID_H
#include "utils.h"
#include "NonLinearLayer.h"

class Sigmoid : public NonLinearLayer
{

public:
	Sigmoid(size_t size);
	Sigmoid() {};
	string getNetworkName() override;
	void compute() override;
	void calculate() override;
	void gradient() override;
	size_t getSize();

};


#endif