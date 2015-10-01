#pragma once
#include "utils.h"
#include "NonLinearLayer.h"

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

