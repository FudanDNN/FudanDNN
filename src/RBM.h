#pragma once
#include "utils.h"
#include "Matrix.h"
#include "AbstractNetworkLayer.h"
#include "NetworkFunctions.h"
#include "NonLinearLayer.h"
#include "NonLinearFactory.h"
#include <fstream>
#include <sstream>

class RBM : public AbstractNetworkLayer
{

public:
	RBM(size_t visualUnit, size_t hiddenUnit, size_t nonLinearType, bool sparse, size_t initScheme);
	RBM() {};
	string getNetworkName() override;
	void compute() override;
	void calculate() override;
	void gradient() override;

	// perform pretraing use energy function
	void preTraining(string filename);
	void setSelfBatchSize(size_t selfBatchSize);
	size_t getSelfBatchSize();
	void setSelfTrainingTime(size_t selfTrainingTime);
	size_t getSelfTrainingTime();
	void setSampleLength(size_t sampleLength);
	size_t getSampleLength();
	void setNonLinearType(size_t nonLinearType);
	size_t getNonLinearType();
	void setSparse(bool sparse);
	bool getSparse();
	double singleTraining(shared_ptr<AbstractMatrix> v);
	void selfUpdate(bool regularize);
	void writeSelf(string) override;
	void readSelf(string) override;
	~RBM();

protected:
	shared_ptr<AbstractMatrix> biasH2V;
	shared_ptr<AbstractMatrix> biasH2VGradient;
	shared_ptr<NonLinearLayer> nonlinearLayer;
	size_t selfBatchSize = 1;
	size_t selfTrainingTime = 1000;
	size_t sampleLength = 1;
	size_t nonLinearType;
	bool sparse = false;

	shared_ptr<AbstractMatrix> backwardCompute(shared_ptr<AbstractMatrix> h);
	shared_ptr<AbstractMatrix> forwardCompute(shared_ptr<AbstractMatrix> v);
	shared_ptr<AbstractMatrix> readDatum(ifstream& fin);

};
