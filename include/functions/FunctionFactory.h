#ifndef __FUDANDNN_FUNCTIONFACTORY_H
#define __FUDANDNN_FUNCTIONFACTORY_H

#include "Sigmoid.h"
#include "HardTanH.h"
#include "ReLU.h"
#include "TanH.h"
#include "Function.h"

class FunctionFactory{
public:
	shared_ptr<Function> createFunction(int type, double s, double lb, double ub, double prec, double ic);
};
#endif;

