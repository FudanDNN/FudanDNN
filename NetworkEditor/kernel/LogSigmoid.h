#ifndef __FUDANDNN_LOGSIGMOID_H
#define __FUDANDNN_LOGSIGMOID_H
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

};


#endif