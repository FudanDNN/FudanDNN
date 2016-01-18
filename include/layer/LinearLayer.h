#ifndef __FUDANDNN_LINEARLAYER_H
#define __FUDANDNN_LINEARLAYER_H
#include "util\utils.h"
#include "util\Matrix.h"
#include "Layer.h"

class LinearLayer : public Layer
{
public:

	LinearLayer(size_t visualUnit, size_t hiddenUnit, size_t initScheme, double dropoutRate, shared_ptr<Solver> solver, size_t num);
	LinearLayer() {};
	string getNetworkName() override;
	void compute() override;
	void calculate()override;
	void gradient()override;
	void update();

	void writeSelf(string) override;
	void readSelf(string) override;
	void init(int initScheme);

protected:
	void initGradient();
	bool initialized = false;
	size_t initScheme;
	double dropoutRate;
	vector<shared_ptr<Matrix>> weight;
	vector<shared_ptr<Matrix>> weightGradient;
	vector<shared_ptr<Matrix>> bias;
	vector<shared_ptr<Matrix>> biasGradient;
	vector<shared_ptr<Matrix>> weightMomentum;
	vector<shared_ptr<Matrix>> biasMomentum;

};
#endif