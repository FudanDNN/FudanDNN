#pragma once
#include "Criteria.h"
#include "Matrix.h"

class Softmax : public Criteria
{
public:
	Softmax(size_t visualUnit);	
	Softmax(){};
	double computeError() override;
	void gradient() override;
	string getType() override;
	shared_ptr<AbstractMatrix> getAnswer() override;

protected:
	shared_ptr<AbstractMatrix> predVec;
};

