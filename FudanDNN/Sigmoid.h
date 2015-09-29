#include "NonLinearLayer.h"

#pragma once
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
	void writeSelf(string) {};
	void readSelf(string) {};
};

