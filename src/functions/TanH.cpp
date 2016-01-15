#include "functions/TanH.h"


inline shared_ptr<TanH> TanH::getInstance()
{
	return instance == nullptr ? (instance = shared_ptr<TanH>(new TanH())) : instance;
}

inline void TanH::setParameters(double s, double lb, double ub, double prec, double ic)
{
	if (instance != nullptr)
		return;
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

inline double TanH::getValue(double x)
{
	if (x <= lowerbound)
		return 0;
	else if (x >= upperbound)
		return 1;
	else
		return map[(int)((x - lowerbound) * mfactor)];
}

inline double TanH::getDerivate(double x, double y)
{
	if (x <= lowerbound || x >= upperbound)
		return incline;
	else
		return y * (1 - y) * 4;
}