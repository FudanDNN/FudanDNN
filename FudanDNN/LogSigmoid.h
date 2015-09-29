#include "NonLinearLayer.h"

#pragma once
class LogSigmoid : public NonLinearLayer
{

public:
	LogSigmoid(size_t size);
	LogSigmoid() {};
	string getNetworkName() override;
	void compute();
	void calculate();
	void gradient();
	size_t getSize();
	void writeSelf(string) {};
	void readSelf(string) {};

};

