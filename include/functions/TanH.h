#ifndef __FUDANDNN_TANH_H
#define __FUDANDNN_TANH_H

#include "util/utils.h"
#include "Function.h"

class TanH : public Function
{
public:
	TanH();
	TanH(double, double, double, double, double);
	double getValue(double);
	double getDerivate(double, double);
	/*
	arg0: scale, arg1: lowerbound, arg2: upperbound, arg3: precision, arg4: small slope for little inclination	
	*/
	void setParameters(double, double, double, double, double);

private:
	vector<double> map;
	double lowerbound;
	double upperbound;
	double precision;
	double mfactor;
	double scale;
	double incline;

};

#endif