#include "NonLinearLayer.h"

#pragma once
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
	void writeSelf(string) {};
	void readSelf(string) {};
};

