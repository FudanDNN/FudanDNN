#pragma once
#include "utils.h"
#include "AbstractNetworkLayer.h"
#include "NonLinearLayer.h"
#include "NonLinearFactory.h"
#include "XMLDataParser.h"

class AutoEncoder : public AbstractNetworkLayer
{
public:
	AutoEncoder(size_t visualUnit, size_t hiddenUnit, int init_scheme, size_t nonlinearType);
	AutoEncoder() {};
	void compute()override;
	void calculate()override;
	void gradient()override;
	string getNetworkName() override;
	void setSelfBatchSize(size_t selfBatchSize);
	size_t getSelfBatchSize();
	void setPreTrainingTime(size_t preTrainingTime);
	size_t getPreTrainingTime();
	void setNonlinear(shared_ptr<NonLinearLayer> nonlinearLayer);
	virtual void singleTraining(shared_ptr<AbstractMatrix> v);
	void selfUpdate(bool regularize);
	void writeSelf(string) override;
	void readSelf(string) override;
	
protected:
	shared_ptr<AbstractMatrix> outputValue;
	shared_ptr<AbstractMatrix> decoderWeight;
	shared_ptr<AbstractMatrix> decoderBias;
	shared_ptr<AbstractMatrix> decoderBiasGradient;
	shared_ptr<NonLinearLayer> nonlinearLayer;
	shared_ptr<NonLinearLayer> outputNonlinearLayer;
	size_t nonLinearType;
	size_t selfBatchSize;
	size_t preTrainingTime;
	ifstream fin;
	shared_ptr<AbstractMatrix> readDatum();
};