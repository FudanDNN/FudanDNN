#pragma once
#include "utils.h"
#include "Matrix.h"
#include "AbstractComponent.h"

class CNN2DComponent : public AbstractComponent {
protected:

	// size of kernel
	size_t kernelSize;

	// step of kernel
	size_t stride;
	size_t hiddenNum;

	// # of feature maps
	size_t featureMapNum;
	double regularizationRate = 0;
	double kernelLearningRate = 0.1;
	double biasLearningRate = 0.1;

	// conv kernel 
	vector<shared_ptr<AbstractMatrix>> convKernels;

	// gradient of kernel 
	vector<shared_ptr<AbstractMatrix>> convKernelsGradient;

	// bias
	vector<shared_ptr<AbstractMatrix>> bias;

	// gradient of bias
	vector<shared_ptr<AbstractMatrix>> biasGradient;

	void initialization(size_t initScheme);

	enum initializationScheme
	{
		RANDOM,			//[-1,1)
		RANDOM_SQRT,	//[-1/sqrt(visual), 1/sqrt(visual))
		RANDOM_NORM1	//[-1/visual, 1/visual)
	};

public:
	CNN2DComponent();
	CNN2DComponent(size_t kernelSize, size_t stride, size_t featureMap_Num,
		size_t num, size_t visualRow, size_t visualColumn, size_t scheme);
	~CNN2DComponent() ;
	void gradient();
	void compute();
	void calculate();
	void update() override;
	void regularize() override;
	double getRegularizationRate();
	void setRegularizationRate(double regularizationRate);
	double getKernelLearningRate();
	void setKernelLearningRate(double kernelLearningRate);
	double getBiasLearningRate();
	void setBiasLearningRate(double biasLearningRate);

};

