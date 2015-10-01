#pragma once
#include "utils.h"
#include "NonLinearLayer.h"

using namespace std;
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
