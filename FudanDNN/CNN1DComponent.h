#pragma once
#include "Matrix.h"
#include "AbstractComponent.h"
#include <vector>

using namespace std;

class CNN1DComponent : public AbstractComponent {
protected:
	size_t kernelSize;//卷积核的大小
	size_t stride;//卷积核移动步长
	size_t hiddenNum;
	size_t featureMapNum;//卷积出来的featureMap的数量
	double regularizationRate;
	double kernelLearningRate;
	double biasLearningRate;
	vector<shared_ptr<AbstractMatrix>> convKernels;//卷积核
	vector<shared_ptr<AbstractMatrix>> convKernelsGradient;//卷积核gradient
	vector<shared_ptr<AbstractMatrix>> bias;//bias
	vector<shared_ptr<AbstractMatrix>> biasGradient;//bias的gradient
	vector<shared_ptr<AbstractMatrix>> hiddenValue;//即hiddenValue
	vector<shared_ptr<AbstractMatrix>> hiddenvGradient;//featureMap的gradient
	vector<shared_ptr<AbstractMatrix>> visualValue;
	vector<shared_ptr<AbstractMatrix>> visualGradient;

	void initialization(size_t initScheme);

	//不同初始化方案
	static enum initializationScheme
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

