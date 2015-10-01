#pragma once
#include "utils.h"
#include "NonLinearLayer.h"

class SoftShrink : public NonLinearLayer
{

public:
	SoftShrink(size_t size);
	SoftShrink() {};
	string getNetworkName() override;
	void compute();
	void calculate();
	void gradient();
	size_t getSize();

};

