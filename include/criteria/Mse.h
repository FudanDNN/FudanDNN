#ifndef __FUDANDNN_MSE_H
#define __FUDANDNN_MSE_H
#include "Criteria.h"

class Mse : public Criteria
{
public:
	Mse(size_t visualUnit);
	Mse() {};
	double computeError() override;
	void gradient() override;
	string getType() override;
	shared_ptr<Matrix> getAnswer() override;

};


#endif