#include "functions/ReLU.h"


inline shared_ptr<ReLU> ReLU::getInstance()
{
	return instance == nullptr ? (instance = shared_ptr<ReLU>(new ReLU())) : instance;
}

inline void ReLU::setParameters(double ic)
{
	incline = ic;
}

inline double ReLU::getValue(double x)
{
	if (x > 0)
		return x;
	else
		return 0;
}

inline double ReLU::getDerivate(double x)
{
	if (x > 0)
		return 1;
	else
		return incline;
}