#include "TrainingSample.h"

TrainingSample::TrainingSample(shared_ptr<AbstractMatrix> input, shared_ptr<AbstractMatrix> output, string type,
	size_t iRowSize, size_t iColumnSize, size_t oRowSize, size_t oColumnSize):Sample(input,type,iRowSize,iColumnSize,oRowSize,oColumnSize){
	this->output = output;

}
void TrainingSample::setOutput(shared_ptr<AbstractMatrix> output){
	this->output = output;
}
shared_ptr<AbstractMatrix> TrainingSample::getOutput(){
	return this->output;
}