#ifndef __FUDANDNN_FUNCTION_H
#define __FUDANDNN_FUNCTION_H

class Function
{
public:
	Function(){};
	~Function(){};
	virtual double getValue(double){ return -1; };
	virtual double getDerivate(double){ return -1; };

};

#endif