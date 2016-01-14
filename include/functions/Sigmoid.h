#ifndef __FUDANDNN_SIGMOID_H
#define __FUDANDNN_SIGMOID_H

#include "util/utils.h"

class Sigmoid
{
public:
	static double getValue(double);
	static double getDerivate(double, double);
	static shared_ptr<Sigmoid> getInstance();
	static void setParameters(double, double, double, double);

private:
	Sigmoid();
	static shared_ptr<Sigmoid> instance;
	static vector<double> map;
	static double lowerbound;
	static double upperbound;
	static double precision;
	static double scale;
	static double incline;

};


#endif