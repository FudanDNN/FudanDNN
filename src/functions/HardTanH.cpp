#include "functions/HardTanH.h"

HardTanH::HardTanH()
{
}

HardTanH::HardTanH(double slp, double ic)
{
	setParameters(slp, ic);
}

void HardTanH::setParameters(double slp, double ic)
{
	slope = slp;
	incline = ic;
	lowerbound = -1 / slope;
	upperbound = -lowerbound;
}

double HardTanH::getValue(double x)
{
	if (x < lowerbound)
		return -1;
	else if (x > upperbound)
		return 1;
	else
		return x * slope;
}

double HardTanH::getDerivate(double x)
{
	if (x < lowerbound || x > upperbound)
		return incline;
	else
		return slope;
}
