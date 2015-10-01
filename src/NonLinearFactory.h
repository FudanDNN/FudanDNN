#pragma once
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
