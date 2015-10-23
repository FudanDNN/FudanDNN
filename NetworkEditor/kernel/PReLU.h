#ifndef __FUDANDNN_PRELU_H
#define __FUDANDNN_PRELU_H
#include "utils.h"
#include "NonLinearLayer.h"

class PReLU : public NonLinearLayer
{

public:
	PReLU(size_t size);
	PReLU() {};
	void compute();
	void calculate();
	void gradient();
	string getNetworkName() override;
	size_t getSize();
};


#endif