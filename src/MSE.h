#ifndef __FUDANDNN_MSE_H
#define __FUDANDNN_MSE_H
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


#endif