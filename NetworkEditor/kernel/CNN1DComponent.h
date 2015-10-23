#ifndef __FUDANDNN_CNN1DCOMPONENT_H
#define __FUDANDNN_CNN1DCOMPONENT_H
#include "utils.h"
#include "Matrix.h"
#include "AbstractComponent.h"

class CNN1DComponent : public AbstractComponent {
protected:

	// size of kernel
	size_t kernelSize;

	// step of kernel 
	size_t stride;

	// # of feature maps 
	size_t featureMapNum;
	double regularizationRate;
	double kernelLearningRate;
	double biasLearningRate;
	double momentumRate = 0.9; 
	
	size_t scheme;

	// conv kernel 
	vector<vector<shared_ptr<AbstractMatrix>>> convKernels;

	// gradient of conv kernel 
	vector<vector<shared_ptr<AbstractMatrix>>> convKernelsGradient;

	// momentum of conv kernel
	vector<vector<shared_ptr<AbstractMatrix>>> convKernelMomentum;

	// bias
	vector<shared_ptr<AbstractMatrix>> bias;

	// gradient of bias
	vector<shared_ptr<AbstractMatrix>> biasGradient;

	// momentum of bias
	vector<shared_ptr<AbstractMatrix>> biasMomentum;

	// hidden value
	vector<shared_ptr<AbstractMatrix>> hiddenValue;

	// gradient of feature map
	vector<shared_ptr<AbstractMatrix>> hiddenvGradient;
	vector<shared_ptr<AbstractMatrix>> visualValue;
	vector<shared_ptr<AbstractMatrix>> visualGradient;

	void initialization();

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
	size_t calculateHiddenSize();
	double getRegularizationRate();
	void setRegularizationRate(double regularizationRate);
	double getKernelLearningRate();
	void setKernelLearningRate(double kernelLearningRate);
	double getBiasLearningRate();
	void setBiasLearningRate(double biasLearningRate);
	double getMomentumRate();
	void setMomentumRate(double momentumRate);
	size_t getKernelSize();
	void setKernelSize(size_t kernel);
	size_t getStride();
	void setStride(size_t stride);
	size_t getFeatureMapNum();
	void setFeatureMap(size_t featureMap);
	void writeSelf(string);
	void readSelf(string);
	void initializeVector();
	string getComponentName() { return "CNN1D"; };
};


#endif