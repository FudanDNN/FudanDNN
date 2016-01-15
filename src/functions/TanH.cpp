#include "functions/TanH.h"


inline shared_ptr<TanH> TanH::getInstance()
{
	return instance == nullptr ? (instance = shared_ptr<TanH>(new TanH())) : instance;
}

inline void TanH::setParameters(double lb, double ub, double prec, double ic)
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

inline double TanH::getValue(double x)
{
	if (x <= lowerbound)
		return 0;
	else if (x >= upperbound)
		return 1;
	else
		return map[(int)((x - lowerbound) * scale)];
}

inline double TanH::getDerivate(double x, double y)
{
	if (x <= lowerbound || x >= upperbound)
		return incline;
	else
		return y * (1 - y) * 4;
}