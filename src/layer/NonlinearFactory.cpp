#include "layer/NonlinearFactory.h"
#include "layer/SigmoidLayer.h"
shared_ptr<NonlinearLayer> NonlinearFactory::createNonlinear(int type, int unit){
	shared_ptr<NonlinearLayer> layer;
	switch (type){
	case NONLINEAR::SIGMOID:
		layer = shared_ptr<SigmoidLayer>(new SigmoidLayer(unit));
		break;
	default:
		break;
	}
	return layer;
}