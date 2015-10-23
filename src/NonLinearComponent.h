#ifndef __FUDANDNN_NONLINEARCOMPONENT_H
#define __FUDANDNN_NONLINEARCOMPONENT_H
#include "Matrix.h"
#include "AbstractComponent.h"
#include "NetworkFunctions.h"

class NonLinearComponent : public AbstractComponent {
public:
	NonLinearComponent(int visualRow, int visualColumn, int num, int type);
	void gradient();
	void compute();
	void calculate();
	void update();
	void regularize();
	void writeSelf(string);
	void readSelf(string);
	string getComponentName() { return "NonLinear"; };
protected:
	int type;
};
#endif