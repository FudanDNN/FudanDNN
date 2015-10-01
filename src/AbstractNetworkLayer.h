#pragma once
#include "utils.h"
#include "AbstractLayer.h"

class AbstractNetworkLayer : public AbstractLayer
{
public:
	// update paramteres
	virtual void update();
	string getNetworkName() override;

	void regularize();

	// get weight
	shared_ptr<AbstractMatrix> getWeight();

	// set weight
	void setWeight(shared_ptr<AbstractMatrix> weight);

	// get bias
	shared_ptr<AbstractMatrix> getBias();

	// set bias
	void setBias(shared_ptr<AbstractMatrix> bias);

	// generate dropout matrix
	void generateDropout();

	double getRegularizationRate();
	void setRegularizationRate(double regularizationRate);

	// learning rate for weight
	double getWeightLearningRate();
	void setWeightLearningRate(double weightLearningRate);

	// learning rate for bias
	double getBiasLearningRate();
	void setBiasLearningRate(double biasLearningRate);

	// rate for dropout
	double getDropoutRate();
	void setDropoutRate(double dropoutRate);
	virtual ~AbstractNetworkLayer() {};
	void writeSelf(string) {};
	void readSelf(string) {};

	// initializaion scheme
	enum initializationScheme
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
	double weightLearningRate = 0.05;
	double biasLearningRate = 0.05;
	double dropoutRate = 0.5;

	void initialization(int initScheme);

};

