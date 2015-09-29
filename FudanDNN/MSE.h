#pragma once
#include "Criteria.h"

class MSE : public Criteria
{
public:
	MSE(size_t visualUnit);
	MSE() {};
	//¼ÆËãÎó²î
	double computeError() override;
	//Îó²î·´´«
	void gradient() override;
	string getType() override;

};

