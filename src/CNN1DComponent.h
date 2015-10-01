#pragma once
#include "utils.h"
#include "Matrix.h"
#include "AbstractComponent.h"
#include <vector>

using namespace std;

class CNN1DComponent : public AbstractComponent {
protected:

	// size of kernel
	size_t kernelSize;

	// step of kernel 
	size_t stride;
	size_t hiddenNum;

	// # of feature maps 
	size_t featureMapNum;
	double regularizationRate;
	double kernelLearningRate;
	double biasLearningRate;

	// conv kernel 
	vector<shared_ptr<AbstractMatrix>> convKernels;

	// gradient of conv kernel 
	vector<shared_ptr<AbstractMatrix>> convKernelsGradient;

	// bias
	vector<shared_ptr<AbstractMatrix>> bias;

	// gradient of bias
	vector<shared_ptr<AbstractMatrix>> biasGradient;

	// hidden value
	vector<shared_ptr<AbstractMatrix>> hiddenValue;

	// gradient of feature map
	vector<shared_ptr<AbstractMatrix>> hiddenvGradient;
	vector<shared_ptr<AbstractMatrix>> visualValue;
	vector<shared_ptr<AbstractMatrix>> visualGradient;

	void initialization(size_t initScheme);

	enum initializationScheme
	{
		RANDOM,			//[-1,1)
		RANDOM_SQRT,	//[-1/sqrt(visual), 1/sqrt(visual))
		RANDOM_NORM1	//[-1/visual, 1/visual)
	};

public:
	CNN1DComponent();
	CNN1DComponent(size_t kernelSize, size_t stride, size_t num, size_t featureMap_Num,
		size_t visualRow, size_t scheme);
	~CNN1DComponent();
	void gradient();
	void compute();
	void calculate();
	void update();
	void regularize();
	double getRegularizationRate();
	void setRegularizationRate(double regularizationRate);
	double getKernelLearningRate();
	void setKernelLearningRate(double kernelLearningRate);
	double getBiasLearningRate();
	void setBiasLearningRate(double biasLearningRate);

};

