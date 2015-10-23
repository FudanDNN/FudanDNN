#ifndef __FUDANDNN_CRITERIA_H
#define __FUDANDNN_CRITERIA_H
#include "utils.h"
#include "AbstractMatrix.h"

class Criteria
{
public:
	Criteria();
	virtual double computeError() { return 0; };
	virtual void gradient() {};
	size_t getVisualUnit();
	void setPredictValue(shared_ptr<AbstractMatrix>);
	shared_ptr<AbstractMatrix> getPredictValue();
	void setExpectedValue(shared_ptr<AbstractMatrix>);
	shared_ptr<AbstractMatrix> getExpectedValue();
	shared_ptr<AbstractMatrix> getPredictGradient();
	virtual string getType();
	virtual shared_ptr<AbstractMatrix> getAnswer() = 0;
	double getErrorRate();
	int getPredictType();
	int getExpectedType();
	void setVisualUnit(size_t visualUnit);
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

#endif