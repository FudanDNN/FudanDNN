#ifndef __FUDANDNN_NONLINEARFACTORY_H
#define __FUDANDNN_NONLINEARFACTORY_H
#include "utils.h"
#include "NonLinearLayer.h"

class NonLinearFactory{

public:
	shared_ptr<NonLinearLayer> createNonLinear(int type, int unit);
	enum NONLINEAR
	{
		SIGMOID,
		HARDSHRINK,
		HARHTANH,
		LOGSSIGMOID,
		PRELU,
		RELU,
		SOFTSHRINK,
	};
};

#endif