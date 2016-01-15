#ifndef __FUDANDNN_FUNCTION_H
#define __FUDANDNN_FUNCTION_H

class Function
{
public:
	Function();
	~Function();
	virtual double getValue(double);
	virtual double getDerivate(double);

};

#endif