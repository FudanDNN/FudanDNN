#ifndef __FUDANDNN_MAXPOOLING_LAYER_H
#define __FUDANDNN_MAXPOOLING_LAYER_H

#include "layer/Layer.h"
#include "util/Matrix.h"

class MaxPoolingLayer : public Layer
{
public:
	MaxPoolingLayer(size_t rowSize, size_t columnSize, size_t krowSize, size_t kcolumnSize, size_t visualSize,
		size_t hiddenSize, size_t stride, size_t initScheme, double dropoutRate);
	~MaxPoolingLayer();
	string getNetworkName() override;
	void compute() override;
	void calculate() override;
	void gradient() override;

	void writeSelf(string) override;
	void readSelf(string) override;
	size_t getSize();
	void init() override;

private:
	void initGradient();
	bool initialized = false;
	size_t krowSize = 0;
	size_t kcolumnSize = 0;
	size_t stride;
	double dropoutRate;


};

#endif