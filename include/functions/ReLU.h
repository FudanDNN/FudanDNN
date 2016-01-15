#ifndef __FUDANDNN_RELU_H
#define __FUDANDNN_RELU_H

#include "util/utils.h"
#include "Function.h"

class ReLU : public Function
{
public:
	ReLU();
	ReLU(double);
	double getValue(double);
	double getDerivate(double);
	void setParameters(double);

private:
	double incline;

};


#endif