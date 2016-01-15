#ifndef __FUDANDNN_TANH_H
#define __FUDANDNN_TANH_H

#include "util/utils.h"

class TanH
{
public:
	static double getValue(double);
	static double getDerivate(double, double);
	static shared_ptr<TanH> getInstance();
	static void setParameters(double, double, double, double);

private:
	TanH();
	static shared_ptr<TanH> instance;
	static vector<double> map;
	static double lowerbound;
	static double upperbound;
	static double precision;
	static double scale;
	static double incline;

}

#endif