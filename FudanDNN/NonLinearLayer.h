#pragma once
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