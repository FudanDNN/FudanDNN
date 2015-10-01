#ifndef __FUDANDNN_ABSTRACTLAYER_H
#define __FUDANDNN_ABSTRACTLAYER_H
#include "utils.h"
#include "AbstractMatrix.h"

class AbstractLayer
{
public:
	//forward propagation for training
	virtual void compute() {};

	//forward propagation for predicting
	virtual void calculate() {};
	
	//back propagation
	virtual void gradient() {};

	virtual string getNetworkName() { return nullptr; };

	//update error
	void setVisualValue(shared_ptr<AbstractMatrix> visualValue);

	//set hidden layer error
	void setHiddenGradient(shared_ptr<AbstractMatrix> hiddenGradient);
	
	//get value of visable layer
	shared_ptr<AbstractMatrix> getVisualValue();

	//get value of hidden layer
	shared_ptr<AbstractMatrix> getHiddenValue();

	//get error of visable layer
	shared_ptr<AbstractMatrix> getVisualGradient();

	//get error of hidden layer
	shared_ptr<AbstractMatrix> getHiddenGradient();

	//get # of units in visable layer
	size_t getVisualUnit();

	//get # of units in hidden layer
	size_t getHiddenUnit();

	void setNetworkName(string networkName);
	virtual void writeSelf(string) {};
	virtual void readSelf(string) {};
	virtual ~AbstractLayer() {};

protected:
	shared_ptr<AbstractMatrix> visualValue;
	shared_ptr<AbstractMatrix> hiddenValue;
	shared_ptr<AbstractMatrix> visualGradient;
	shared_ptr<AbstractMatrix> hiddenGradient;
	size_t visualUnit;
	size_t hiddenUnit;
	string networkname;

};


#endif