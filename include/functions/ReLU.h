#ifndef __FUDANDNN_RELU_H
#define __FUDANDNN_RELU_H

#include "util/utils.h"

class ReLU
{
public:
	static double getValue(double);
	static double getDerivate(double);
	static shared_ptr<ReLU> getInstance();
	static void setParameters(double);

private:
	ReLU();
	static shared_ptr<ReLU> instance;
	static double incline;

};


#endif