#include "functions/Sigmoid.h"


inline shared_ptr<Sigmoid> Sigmoid::getInstance()
{
	return instance == nullptr ? (instance = shared_ptr<Sigmoid>(new Sigmoid())) : instance;
}

inline void Sigmoid::setParameters(double lb, double ub, double prec, double ic)
{
	if (instance != nullptr)
		return;
	lowerbound = lb;
	upperbound = ub;
	precision = prec;
	scale = 1 / prec;
	incline = ic;
	int x = lowerbound;
	while (x < upperbound)
	{
		map.push_back(1 / (1 + exp(-x)));
		x += precision;
	}

}

inline double Sigmoid::getValue(double x)
{
	if (x <= lowerbound)
		return 0;
	else if (x >= upperbound)
		return 1;
	else
		return map[(int)((x - lowerbound) * scale)];
}

inline double Sigmoid::getDerivate(double x, double y)
{
	if (x <= lowerbound || x >= upperbound)
		return incline;
	else
		return y * (1 - y) * 4;
}