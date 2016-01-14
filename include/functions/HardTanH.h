#ifndef __FUDANDNN_HARDTANH_H
#define __FUDANDNN_HARDTANH_H

#include "util/utils.h"

class HardTanH
{
public:
	static double getValue(double);
	static double getDerivate(double);
	static shared_ptr<HardTanH> getInstance();
	static void setParameters(double, double);

private:
	HardTanH();
	static shared_ptr<HardTanH> instance;
	static double lowerbound;
	static double upperbound;
	static double slope;
	static double incline;


};

#endif 
