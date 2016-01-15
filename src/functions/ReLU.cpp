#include "functions/ReLU.h"

ReLU::ReLU()
{
}

ReLU::ReLU(double ic)
{
	setParameters(ic);
}

void ReLU::setParameters(double ic)
{
	incline = ic;
}

double ReLU::getValue(double x)
{
	if (x > 0)
		return x;
	else
		return 0;
}

double ReLU::getDerivate(double x)
{
	if (x > 0)
		return 1;
	else
		return incline;
}