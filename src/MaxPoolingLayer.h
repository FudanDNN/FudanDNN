#ifndef __FUDANDNN_MAXPOOLINGLAYER_H
#define __FUDANDNN_MAXPOOLINGLAYER_H
#include "utils.h"
#include "Matrix.h"
#include "AbstractComponent.h"

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
	MaxPoolingLayer(size_t poolingSize, size_t stride, size_t visualRow, size_t visualColumn, size_t num);
	void compute() override;
	void calculate() override;
	void gradient() override;
	void writeSelf(string);
	void readSelf(string);
	string getComponentName() { return "MaxPooling"; };

};
#endif