#ifndef __FUDANDNN_LINEARLAYER_H
#define __FUDANDNN_LINEARLAYER_H
#include "util\utils.h"
#include "util\Matrix.h"
#include "Layer.h"

class LinearLayer : public Layer
{
public:

	LinearLayer(size_t visualUnit, size_t hiddenUnit, size_t initScheme, double dropoutRate, shared_ptr<Solver> solver);
	LinearLayer() {};
	string getNetworkName() override;
	void compute() override;
	void calculate()override;
	void gradient()override;
	void update();

	void writeSelf(string) override;
	void readSelf(string) override;
	void init() override;
	void initialization(int initScheme);

protected:
	void initGradient();
	bool initialized = false;
	size_t initScheme;
	double dropoutRate;
	shared_ptr<Matrix> weight;
	shared_ptr<Matrix> weightGradient;
	shared_ptr<Matrix> bias;
	shared_ptr<Matrix> biasGradient;
	shared_ptr<Matrix> weightMomentum;
	shared_ptr<Matrix> biasMomentum;
};
#endif