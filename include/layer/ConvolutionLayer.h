#ifndef __FUDANDNN_CONVOLUTION_LAYER_H
#define __FUDANDNN_CONVOLUTIOON_LAYER_H

#include "layer/Layer.h"
#include "util/Matrix.h"

class ConvolutionLayer : public Layer
{
public:
	ConvolutionLayer(size_t rowSize, size_t columnSize, size_t krowSize, size_t kcolumnSize, size_t visualSize, size_t hiddenSize, size_t stride, size_t initScheme, shared_ptr<Solver> solver);
	~ConvolutionLayer();
	string getNetworkName() override;
	void compute() override;
	void calculate() override;
	void gradient() override;
	void writeSelf(string) override;
	void readSelf(string) override;
	size_t getSize();
	void init() override;
	void initialization(int initScheme);

private:
	void initGradient();
	bool initialized = false;
	size_t initScheme;
	size_t krowSize = 0;
	size_t kcolumnSize = 0;
	size_t stride;
	vector<vector<shared_ptr<Matrix>>> kernel;
	vector<vector<shared_ptr<Matrix>>> kernelGradient;
	vector<shared_ptr<Matrix>> bias;
	vector<shared_ptr<Matrix>> biasGradient;
	vector<vector<shared_ptr<Matrix>>> kernelMomentum;
	vector<shared_ptr<Matrix>> biasMomentum;


};

#endif
