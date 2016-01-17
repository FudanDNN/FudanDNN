#ifndef __FUDANDNN_HARDTANH_H
#define __FUDANDNN_HARDTANH_H

#include "util/utils.h"
#include "Function.h"

class HardTanH : public Function
{
public:
	HardTanH(); 
	HardTanH(double slp, double ic);
	double getValue(double) override;
	double getDerivate(double, double) override;
	void setParameters(double, double);

private:
	double lowerbound;
	double upperbound;
	double slope;
	double incline;


};

#endif 
