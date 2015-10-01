#ifndef __FUDANDNN_DENOISEAUTOENCODER_H
#define __FUDANDNN_DENOISEAUTOENCODER_H
#include "utils.h"
#include "AutoEncoder.h"
#include "NonLinearLayer.h"
#include "NonLinearFactory.h"
#include "XMLDataParser.h"

class DenoiseAutoEncoder : public AutoEncoder
{
public:
	DenoiseAutoEncoder(size_t visualUnit, size_t hiddenUnit, int init_scheme, size_t nonlinearType
		,size_t clearRate);
	DenoiseAutoEncoder() {};
	string getNetworkName() override;
	void singleTraining(shared_ptr<AbstractMatrix> v);
	size_t getClearRate();
	void setClearRate(size_t clearRate);
private:
	double noise(double x);
protected:
	size_t clearRate;
};
#endif