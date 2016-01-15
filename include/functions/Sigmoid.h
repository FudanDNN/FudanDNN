#ifndef __FUDANDNN_SIGMOID_H
#define __FUDANDNN_SIGMOID_H

#include "util/utils.h"

class Sigmoid
{
public:
	Sigmoid();
	Sigmoid(double s, double lb, double ub, double prec, double ic);
	double getValue(double);
	double getDerivate(double, double);
	//arg0: scale, arg1: lowerbound, arg2: upperbound, arg3: precision, arg4: small slope for little inclination
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