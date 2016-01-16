#ifndef __FUDANDNN_LAYER_H
#define __FUDANDNN_LAYER_H
#include "util/utils.h"
#include "util/Matrix.h"
#include "Solver.h"

class Layer
{
public:
	//forward propagation for training
	virtual void compute() {};

	//forward propagation for predicting
	virtual void calculate() {};

	//back propagation
	virtual void gradient() {};

	//update parameter
	virtual void update() {};

	//get network name
	virtual string getNetworkName() { return nullptr; };

	virtual void writeSelf(string) {};
	virtual void readSelf(string) {};
	virtual ~Layer() {};

	//update error
	void setVisualValue(vector<shared_ptr<Matrix>> visualValue);

	//set hidden layer error
	void addHiddenGradient(vector<shared_ptr<Matrix>> hiddenGradient);

	//get value of visible layer
	vector<shared_ptr<Matrix>> getVisualValue();

	//get value of hidden layer
	vector<shared_ptr<Matrix>> getHiddenValue();

	//get error of visible layer
	vector<shared_ptr<Matrix>> getVisualGradient();

	//get error of hidden layer
	vector<shared_ptr<Matrix>> getHiddenGradient();

	size_t getVisualRow();
	size_t getVisualColumn();

	size_t getHiddenRow();
	size_t getHiddenColumn();

	size_t getVisualSize();
	size_t getHiddenSize();

	void setNetworkName(string networkName);

	// rate for dropout
	double getDropoutRate();
	void setDropoutRate(double dropoutRate);
	virtual void init(){};


protected:
	vector<shared_ptr<Matrix>> visualValue;
	vector<shared_ptr<Matrix>> hiddenValue;
	vector<shared_ptr<Matrix>> visualGradient;
	vector<shared_ptr<Matrix>> hiddenGradient;
	shared_ptr<Solver> solver;
	size_t visualSize;
	size_t hiddenSize;
	size_t visualRow;
	size_t visualColumn;
	size_t hiddenRow;
	size_t hiddenColumn;
	string networkname;
	double dropoutRate = 0.5;

};


#endif