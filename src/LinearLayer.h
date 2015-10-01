#pragma once
#include "utils.h"
#include "Matrix.h"
#include "AbstractNetworkLayer.h"

class LinearLayer : public AbstractNetworkLayer
{
public:
	
	LinearLayer(size_t visualUnit, size_t hiddenUnit, size_t initScheme);
	LinearLayer() {};
	string getNetworkName() override;
	void compute() override;
	void calculate()override;
	void gradient()override;

	void writeSelf(string) override;
	void readSelf(string) override;
	
};