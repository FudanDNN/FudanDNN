#ifndef __FUDANDNN_FUNCTIONS_H
#define __FUDANDNN_FUNCTIONS_H

#include "util/utils.h"

class Function {
public:
	virtual double getValue(double);
	virtual double getDerivate(double);
	virtual Function getInstance();
};

#endif