#ifndef __FUDANDNN_NONLINEARFACTORY_H
#define __FUDANDNN_NONLINEARFACTORY_H
#include "utils.h"
#include "NonLinearLayer.h"
#include "ReLU.h"

class NonLinearFactory{

public:
	shared_ptr<NonLinearLayer> createNonLinear(int type, int unit);
	enum NONLINEAR
	{
		SIGMOID,
		RELU,
		HARDSHRINK,
		HARHTANH,
		LOGSSIGMOID,
		PRELU,
		SOFTSHRINK,
	};
};

#endif