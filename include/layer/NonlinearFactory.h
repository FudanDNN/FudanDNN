#ifndef __FUDANDNN_NONLINEARFACTORY_H
#define __FUDANDNN_NONLINEARFACTORY_H
#include "util/utils.h"
#include "NonLinearLayer.h"

class NonlinearFactory{

public:
	shared_ptr<NonlinearLayer> createNonlinear(int type, int unit);
};

#endif