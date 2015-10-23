#ifndef __FUDANDNN_TRAININGSAMPLE_H
#define __FUDANDNN_TRAININGSAMPLE_H
#include "utils.h"
#include "AbstractMatrix.h"
#include "Sample.h"

class TrainingSample:public Sample
{
private:
	shared_ptr<AbstractMatrix> output;
public:
	TrainingSample(shared_ptr<AbstractMatrix> input, shared_ptr<AbstractMatrix> output, string type,
		size_t iRowSize, size_t iColumnSize, size_t oRowSize, size_t oColumnSize);
	void setOutput(shared_ptr<AbstractMatrix> output);
	shared_ptr<AbstractMatrix> getOutput();
};


#endif