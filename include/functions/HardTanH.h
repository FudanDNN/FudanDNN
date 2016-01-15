#ifndef __FUDANDNN_HARDTANH_H
#define __FUDANDNN_HARDTANH_H

#include "util/utils.h"
#include "Function.h"

class HardTanH : public Function
{
public:
	HardTanH(); 
	HardTanH(double slp, double ic);
	double getValue(double);
	double getDerivate(double);
	void setParameters(double, double);

private:
	double lowerbound;
	double upperbound;
	double slope;
	double incline;


};

#endif 
