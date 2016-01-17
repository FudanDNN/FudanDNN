#ifndef __FUDANDNN_CRITERIA_H
#define __FUDANDNN_CRITERIA_H
#include "util\utils.h"
#include "util\Matrix.h"

class Criteria
{
public:
	virtual double computeError() { return 0; };
	virtual void gradient() {};
	size_t getVisualUnit();
	void setPredictValue(shared_ptr<Matrix>);
	shared_ptr<Matrix> getPredictValue();
	void setExpectedValue(shared_ptr<Matrix>);
	shared_ptr<Matrix> getExpectedValue();
	shared_ptr<Matrix> getPredictGradient();
	virtual string getType();
	virtual shared_ptr<Matrix> getAnswer() = 0;
	double getErrorRate();
	int getPredictType();
	int getExpectedType();
	void setVisualUnit(size_t visualUnit);
	void readSelf(string);
	void writeSelf(string);

protected:
	size_t visualUnit;
	shared_ptr<Matrix> predictValue;
	shared_ptr<Matrix> expectedValue;
	shared_ptr<Matrix> predictGradient;
	double errorRate = 1;
	string type;
};

#endif