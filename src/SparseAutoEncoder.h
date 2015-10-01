#ifndef __FUDANDNN_SPARSEAUTOENCODER_H
#define __FUDANDNN_SPARSEAUTOENCODER_H
#include "utils.h"
#include "AutoEncoder.h"
#include "NonLinearLayer.h"
#include "NonLinearFactory.h"
#include "XMLDataParser.h"

using namespace std;
class SparseAutoEncoder : public AutoEncoder
{
public:
	SparseAutoEncoder(size_t visualUnit, size_t hiddenUnit, int init_scheme, size_t nonlinearType
		,size_t sparseRate,size_t sparsePercent);
	SparseAutoEncoder() {};
	string getNetworkName() override;
	size_t getSparseRate();
	void setSparseRate(size_t sparseRate);
	size_t getSparsePercent();
	void setSparsePercent(size_t sparsePercent);
	void singleTraining(shared_ptr<AbstractMatrix> v);
	void writeSelf(string) {};
	void readSelf(string) {};
protected:
	size_t sparseRate;
	size_t sparsePercent;
};
#endif