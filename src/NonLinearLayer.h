#ifndef __FUDANDNN_NONLINEARLAYER_H
#define __FUDANDNN_NONLINEARLAYER_H
#include "utils.h"
#include "Matrix.h"
#include "NetworkFunctions.h"
#include "AbstractLayer.h"

class NonLinearLayer : public AbstractLayer
{
public:
	NonLinearLayer();
	virtual string getNetworkName() { return nullptr; };
	void readSelf(string);
	void writeSelf(string);
};
#endif