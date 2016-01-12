#ifndef __FUDANDNN_LAYER_H
#define __FUDANDNN_LAYER_H
#include "../util/utils.h"
#include "../util/Matrix.h"

class Layer
{
public:
	//forward propagation for training
	virtual void compute() {};

	//forward propagation for predicting
	virtual void calculate() {};

	//back propagation
	virtual void gradient() {};

	//update paramater
	virtual void update() {};

	//get network name
	virtual string getNetworkName() { return nullptr; };

	virtual void writeSelf(string) {};
	virtual void readSelf(string) {};
	virtual ~Layer() {};

	//update error
	void setVisualValue(shared_ptr<Matrix> visualValue);

	//set hidden layer error
	void addHiddenGradient(shared_ptr<Matrix> hiddenGradient);

	//get value of visable layer
	shared_ptr<Matrix> getVisualValue();

	//get value of hidden layer
	shared_ptr<Matrix> getHiddenValue();

	//get error of visable layer
	shared_ptr<Matrix> getVisualGradient();

	//get error of hidden layer
	shared_ptr<Matrix> getHiddenGradient();

	size_t getVisualRow();
	size_t getVisualColumn();

	size_t getHiddenRow();
	size_t getHiddenColumn();

	void setNetworkName(string networkName);

	// rate for regularize
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

	// rate for momentum
	double getMomentumRate();
	void setMomentumRate(double momentumRate);


protected:
	shared_ptr<Matrix> visualValue;
	shared_ptr<Matrix> hiddenValue;
	shared_ptr<Matrix> visualGradient;
	shared_ptr<Matrix> hiddenGradient;
	size_t visualRow;
	size_t visualColumn;
	size_t hiddenRow;
	size_t hiddenColumn;
	string networkname;
	double regularizationRate = 0;
	double weightLearningRate = 0.05;
	double biasLearningRate = 0.05;
	double dropoutRate = 0.5;
	double momentumRate = 0.9;

};


#endif