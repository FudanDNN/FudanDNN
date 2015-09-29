#include "NonLinearLayer.h"
#pragma once
using namespace std;
class NonLinearFactory{

public:
	shared_ptr<NonLinearLayer> createNonLinear(int type, int unit);
	static enum NONLINEAR
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
