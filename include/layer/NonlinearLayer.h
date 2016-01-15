#ifndef __FUDANDNN_NONLINEARLAYER_H
#define __FUDANDNN_NONLINEARLAYER_H

#include "util/utils.h"
#include "util/Matrix.h"
#include "Layer.h"
#include "functions/Function.h"

class NonlinearLayer : public Layer
{
public:
	NonlinearLayer(shared_ptr<Function>, string, size_t rowSize, size_t columnSize, size_t featureMap);
	string getNetworkName();
	void compute() override;
	void calculate() override;
	void gradient() override;
	void readSelf(string);
	void writeSelf(string);

private:
	shared_ptr<Function> fun;
	string name;

};
#endif