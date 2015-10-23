#include "NetworkFunctions.h"

double sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

double sigmoidDerivative(double y)
{
	return y * (1 - y);
}

double random(double x)
{
	return rand() > x ? 0 : 1;
}

//LogSigmoid
double logsigmoid(double x)
{
	return log(1 / (1 + exp(-x)));
}
double logsigmoidDerivative(double y)
{
	return 0;
}

//HardTanH
double hardTanH(double x)
{
	if (x > 1)
		return 1;
	else if (x < -1)
		return -1;
	else
		return x;
}

double hardTanHDerivative(double y)
{
	if (y > 1)
		return 0;
	else if (y < -1)
		return 0;
	else
		return 1;
}

//HardShrink
double hardShrink(double lambda, double x)
{
	if (x > lambda)
		return x;
	else if (x < -lambda)
		return -x;
	else
		return 0;
}
double hardShrinkDerivative(double lambda, double y)
{
	if (y > lambda)
		return 1;
	else if (y < -lambda)
		return 1;//??????????????
	else
		return 0;
}
//SoftShrink
double softShrink(double x, double lambda)
{
	if (x > lambda)
		return x - lambda;
	else if (x < -lambda)
		return lambda - x;
	else
		return 0;
}
double softShrinkDerivative(double y, double lambda)
{
	if (y > lambda)
		return 1;
	else if (y < -lambda)
		return 1;
	else
		return 0;
}
//ReLU
double reLU(double x)
{
	if (x > 0)
		return x;
	else
		return 0;
}
double reLUDerivative(double y)
{
	if (y > 0)
		return 1;
	else
		return 0;
}
//PReLU
double pReLU(double x, double rate)
{
	return max(0, x) + rate * min(0, x);
}
double pReLUDerivative(double y)
{
	if (y > 0)
		return 1;
	else
		return 1 / 4;
}

double max(double a, double b)
{
	if (a > b)
		return a;
	else
		return b;
}
double min(double a, double b)
{
	if (a > b)
		return b;
	else
		return a;
}