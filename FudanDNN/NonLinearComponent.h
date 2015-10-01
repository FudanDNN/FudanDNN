#pragma once
#include "Matrix.h"
#include "AbstractComponent.h"
#include <vector>
#include "NetworkFunctions.h"

using namespace std;
class NonLinearComponent : public AbstractComponent {
public:
	NonLinearComponent(int visualRow, int visualColumn, int num, int type);
	void gradient();
	void compute();
	void calculate();
	void update();
	void regularize();
protected:
	int type;
};