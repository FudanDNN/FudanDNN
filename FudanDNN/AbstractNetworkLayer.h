#pragma once
#include "AbstractLayer.h"
#include <string>

using namespace std;

#pragma once
class AbstractNetworkLayer : public AbstractLayer
{
public:
	//更新网络参数
	virtual void update();
	string getNetworkName() override;
	//正则化网络参数
	void regularize();
	//获取权重
	shared_ptr<AbstractMatrix> getWeight();
	//设置权重
	void setWeight(shared_ptr<AbstractMatrix> weight);
	//获取偏置
	shared_ptr<AbstractMatrix> getBias();
	//设置偏置
	void setBias(shared_ptr<AbstractMatrix> bias);
	//生成dropout矩阵
	void generateDropout();
	//获取正则化参数
	double getRegularizationRate();
	//设置正则化参数
	void setRegularizationRate(double regularizationRate);
	//获取权重学习率
	double getWeightLearningRate();
	//设置权重学习率
	void setWeightLearningRate(double weightLearningRate);
	//获取偏置学习率
	double getBiasLearningRate();
	//设置偏置学习率
	void setBiasLearningRate(double biasLearningRate);
	//获取dropout率
	double getDropoutRate();
	//设置dropout率
	void setDropoutRate(double dropoutRate);
	virtual ~AbstractNetworkLayer() {};
	void writeSelf(string) {};
	void readSelf(string) {};
	//不同初始化方案
	static enum initializationScheme
	{
		RANDOM,			//[-1,1)
		RANDOM_SQRT,	//[-1/sqrt(visual), 1/sqrt(visual))
		RANDOM_NORM1	//[-1/visual, 1/visual)
	};

protected:
	shared_ptr<AbstractMatrix> weight;
	shared_ptr<AbstractMatrix> weightGradient;
	shared_ptr<AbstractMatrix> bias;
	shared_ptr<AbstractMatrix> biasGradient;
	shared_ptr<AbstractMatrix> dropout;
	double regularizationRate = 0;
	double weightLearningRate = 0.8;
	double biasLearningRate = 0.8;
	double dropoutRate = 0.5;

	void initialization(int initScheme);

};

