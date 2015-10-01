#pragma once
#include "utils.h"
#include "AbstractMatrix.h"
#include <string>
#include <fstream>

using namespace std;

class Criteria
{
public:
	Criteria();

	// calculate error
	virtual double computeError() { return 0; };
	
	// backward propagation 
	virtual void gradient() {};

	// get # of visual units
	size_t getVisualUnit();

	void setPredictValue(shared_ptr<AbstractMatrix>);
	shared_ptr<AbstractMatrix> getPredictValue();
	void setExpectedValue(shared_ptr<AbstractMatrix>);
	shared_ptr<AbstractMatrix> getExpectedValue();

	// get error of visual layer
	shared_ptr<AbstractMatrix> getPredictGradient();
	virtual string getType() { return nullptr; };
	double getErrorRate();
	void readSelf(string);
	void writeSelf(string);

protected:
	size_t visualUnit;
	shared_ptr<AbstractMatrix> predictValue;
	shared_ptr<AbstractMatrix> expectedValue;
	shared_ptr<AbstractMatrix> predictGradient;
	double errorRate = 1;
	string type;
};
