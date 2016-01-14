#ifndef __FUDANDNN_NONLINEARLAYER_H
#define __FUDANDNN_NONLINEARLAYER_H
#include "util/utils.h"
#include "util/Matrix.h"
#include "Layer.h"

class NonlinearLayer : public Layer
{
public:
	NonlinearLayer();
	virtual string getNetworkName() { return nullptr; };
	virtual void readSelf(string);
	virtual void writeSelf(string);
};
#endif