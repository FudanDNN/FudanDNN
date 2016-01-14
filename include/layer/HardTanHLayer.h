#ifndef __FUDANDNN_HARDTANH_LAYER_H
#define __FUDANDNN_HARDTANH_LAYER_H

#include "util/utils.h"
#include "NonlinearLayer.h"
#include "functions/HardTanH.h"

class HardTanHLayer : public NonlinearLayer
{

public:
	HardTanHLayer(size_t rowSize, size_t columnSize, size_t featureMap);
	HardTanHLayer(size_t size);
	string getNetworkName() override;
	void compute() override;
	void calculate() override;
	void gradient() override;
	void writeSelf(string) override;
	void readSelf(string) override;
	size_t getSize();

protected:
	shared_ptr<HardTanH> instance = HardTanH::getInstance();

};


#endif