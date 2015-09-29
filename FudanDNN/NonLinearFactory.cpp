#include "NonLinearFactory.h"
#include "Sigmoid.h"
shared_ptr<NonLinearLayer> NonLinearFactory::createNonLinear(int type,int unit){
	shared_ptr<NonLinearLayer> layer;
	switch (type){
	case SIGMOID:
		layer = shared_ptr<Sigmoid>(new Sigmoid(unit));
		break;
	default:
		layer = shared_ptr<Sigmoid>(new Sigmoid(unit));
		break;
	}
	return layer;
}