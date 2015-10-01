#pragma once
#include "utils.h"
#include "Matrix.h"
#include "AbstractComponent.h"
#include <vector>

using namespace std;

class MaxPoolingLayer : public AbstractComponent {

protected:
	// positon of max value 
	vector<shared_ptr<AbstractMatrix>> maxPostionI;
	vector<shared_ptr<AbstractMatrix>> maxPostionJ;

	// step of pooling
	size_t stride;

	// size of pooling window
	size_t poolingSize;

public:
	MaxPoolingLayer(size_t poolingSize, size_t stride, size_t visualRow, size_t visualColumn);
	void compute() override;
	void calculate() override;
	void gradient() override;

};