#pragma once
#include "utils.h"
#include "AbstractLayer.h"
#include "NonLinearLayer.h"

class HardShrink : public NonLinearLayer
{

public:
	HardShrink(size_t size);
	HardShrink() {};
	string getNetworkName() override;
	void compute();
	void calculate();
	void gradient();
	size_t getSize();
	void writeSelf(string) {};
	void readSelf(string) {};

};

