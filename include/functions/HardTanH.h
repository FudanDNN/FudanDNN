#ifndef __FUDANDNN_HARDTANH_H
#define __FUDANDNN_HARDTANH_H

#include "util/utils.h"

class HardTanH
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
