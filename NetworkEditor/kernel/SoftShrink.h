#ifndef __FUDANDNN_SOFTSHRINK_H
#define __FUDANDNN_SOFTSHRINK_H
#include "utils.h"
#include "NonLinearLayer.h"

class SoftShrink : public NonLinearLayer
{

public:
	SoftShrink(size_t size, double lamda);
	SoftShrink() {};
	string getNetworkName() override;
	void compute();
	void calculate();
	void gradient();
	size_t getSize();

protected:
	double lambda;

};


#endif