#include "functions/HardTanH.h"


inline shared_ptr<HardTanH> HardTanH::getInstance()
{
	return instance == nullptr ? (instance = shared_ptr<HardTanH>(new HardTanH())) : instance;
}

inline void HardTanH::setParameters(double slp, double ic)
{
	slope = slp;
	incline = ic;
	lowerbound = -1 / slope;
	upperbound = -lowerbound;
}

inline double HardTanH::getValue(double x)
{
	if (x < lowerbound)
		return -1;
	else if (x > upperbound)
		return 1;
	else
		return x * slope;
}

inline double HardTanH::getDerivate(double x)
{
	if (x < lowerbound || x > upperbound)
		return incline;
	else
		return slope;
}
