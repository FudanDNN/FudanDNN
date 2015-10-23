#ifndef __FUDANDNN_CNN2DCOMPONENT_H
#define __FUDANDNN_CNN2DCOMPONENT_H
#include "utils.h"
#include "Matrix.h"
#include "AbstractComponent.h"

class CNN2DComponent : public AbstractComponent {
protected:

	// size of kernel
	size_t kernelSize;

	// step of kernel
	size_t stride;

	// # of feature maps
	size_t featureMapNum;
	double regularizationRate = 0.0001;
	double kernelLearningRate = 0.01;
	double biasLearningRate = 0.01;
	double momentumRate = 0.5;

	size_t scheme;

	// conv kernel 
	vector<vector<shared_ptr<AbstractMatrix>>> convKernels;

	// gradient of kernel 
	vector<vector<shared_ptr<AbstractMatrix>>> convKernelsGradient;

	// momentum of kernel
	vector<vector<shared_ptr<AbstractMatrix>>> convKernelMomentum;

	// bias
	vector<shared_ptr<AbstractMatrix>> bias;

	// gradient of bias
	vector<shared_ptr<AbstractMatrix>> biasGradient;

	// momentum of bias
	vector<shared_ptr<AbstractMatrix>> biasMomentum;

	void initialization();

	enum initializationScheme
	{
		RANDOM,			//[-1,1)
		RANDOM_SQRT,	//[-1/sqrt(visual), 1/sqrt(visual))
		RANDOM_NORM1	//[-1/visual, 1/visual)
	};

public:
	size_t calculateHiddenSize();
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
	string getComponentName() { return "CNN2D"; };
};


#endif