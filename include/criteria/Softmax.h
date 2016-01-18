#pragma once
#include "Criteria.h"
#include "util\Matrix.h"

class Softmax : public Criteria
{
public:
	Softmax(size_t visualUnit);
	Softmax(){};
	double computeError() override;
	void gradient() override;
	string getType() override;
	shared_ptr<Matrix> getAnswer() override;

protected:
	shared_ptr<Matrix> predVec;
};

