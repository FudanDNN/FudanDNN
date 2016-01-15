#include "functions\FunctionFactory.h"

shared_ptr<Function> FunctionFactory::createFunction(int type, double s, double lb, double ub, double prec, double ic)
{
	shared_ptr<Function> function;
	switch (type){
	case SIGMOID:
		function = shared_ptr<Sigmoid>(new Sigmoid(s, lb, ub, prec, ic));
	}
	return function;
}