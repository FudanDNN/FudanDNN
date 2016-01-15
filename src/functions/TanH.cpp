#include "functions/TanH.h"

TanH::TanH()
{
}

TanH::TanH(double s, double lb, double ub, double prec, double ic)
{
	setParameters(s, lb, ub, prec, ic);
}

void TanH::setParameters(double s, double lb, double ub, double prec, double ic)
{
	scale = s;
	lowerbound = lb;
	upperbound = ub;
	precision = prec;
	mfactor = 1 / prec;
	incline = ic;
	int x = lowerbound;
	while (x < upperbound)
	{
		map.push_back(2 / (1 + exp(-2 * x / s)) - 1);
		x += precision;
	}

}

double TanH::getValue(double x)
{
	if (x <= lowerbound)
		return 0;
	else if (x >= upperbound)
		return 1;
	else
		return map[(int)((x - lowerbound) * mfactor)];
}

double TanH::getDerivate(double x, double y)
{
	if (x <= lowerbound || x >= upperbound)
		return incline;
	else
		return y * (1 - y) * 4;
}