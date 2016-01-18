#ifndef __FUDANDNN_CONCATLAYER_H
#define __FUDANDNN_CONCATLAYER_H
#include "util\utils.h"
#include "util\Matrix.h"
#include "Layer.h"

class ConcatLayer : public Layer{
public:

	ConcatLayer(size_t visualRow, size_t visualColumn, size_t visualSize);
	string getNetworkName() override;
	void compute() override;
	void calculate()override;
	void gradient()override;

	void writeSelf(string) override;
	void readSelf(string) override;

};
#endif