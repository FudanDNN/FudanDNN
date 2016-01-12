#ifndef __FUDANDNN_HARDTANH_H
#define __FUDANDNN_HARDTANH_H
#include "util/utils.h"
#include "Layer.h"

class SigmoidLayer : public Layer
{

public:
	SigmoidLayer(size_t size);
	string getNetworkName() override;
	void compute() override;
	void calculate() override;
	void gradient() override;
	void writeSelf(string) override;
	void readSelf(string) override;
	size_t getSize();

};


#endif