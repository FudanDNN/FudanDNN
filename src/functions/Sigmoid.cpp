#include "functions/Sigmoid.h"

Sigmoid::Sigmoid()
{
}

Sigmoid::Sigmoid(double s, double lb, double ub, double prec, double ic)
{
	setParameters(s, lb, ub, prec, ic);
}

void Sigmoid::setParameters(double s, double lb, double ub, double prec, double ic)
{
	scale = 1 / s;
	lowerbound = lb;
	upperbound = ub;
	precision = prec;
	mfactor = 1 / prec;
	incline = ic;
	double x = lowerbound;
	while (x < upperbound)
	{
		map.push_back(1 / (1 + exp(-x / scale)));
		x += precision;
	}

}

double Sigmoid::getValue(double x)
{
	if (x <= lowerbound)
		return 0;
	else if (x >= upperbound)
		return 1;
	else
		return map[(int)((x - lowerbound) * mfactor)];
}

double Sigmoid::getDerivate(double x, double y)
{
	if (x <= lowerbound || x >= upperbound)
		return incline;
	else
		return y * (1 - y) * 4;
}