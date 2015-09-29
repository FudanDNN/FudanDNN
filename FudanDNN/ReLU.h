#include "NonLinearLayer.h"

#pragma once
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
	void writeSelf(string) {};
	void readSelf(string) {};
};

