#pragma once
#include "utils.h"
#include "Criteria.h"

class MSE : public Criteria
{
public:
	MSE(size_t visualUnit);
	MSE() {};
	double computeError() override;
	void gradient() override;
	string getType() override;

};

