#include "SparseAutoEncoder.h"
#include "Sigmoid.h"
#include "Matrix.h"
SparseAutoEncoder::SparseAutoEncoder(size_t visualUnit, size_t hiddenUnit, int init_scheme, 
	size_t nonlinearType, size_t sparseRate, size_t sparsePercent):
	AutoEncoder(visualUnit, hiddenUnit,init_scheme,nonlinearType){
	this->sparsePercent = sparsePercent;
	this->sparseRate = sparseRate;
}

void SparseAutoEncoder::singleTraining(shared_ptr<AbstractMatrix> v){
	shared_ptr<AbstractMatrix> temp1 = v->multipleLeft(weight)->add(bias);
	nonlinearLayer->setVisualValue(temp1);
	nonlinearLayer->compute();
	hiddenValue = nonlinearLayer->getHiddenValue();
	size_t sparseValue = 0;
	for (size_t i = 0; i < hiddenValue->getRowSize(); i++){
		for (size_t j = 0; j < hiddenValue->getColumnSize(); j++){
			if (hiddenValue->getValue(i, j)>0.5){
				sparseValue++;
			}
		}
	}

	shared_ptr<AbstractMatrix> temp2 = hiddenValue->multipleLeft(decoderWeight)->add(decoderBias);
	outputNonlinearLayer->setVisualValue(temp2);
	outputNonlinearLayer->compute();
	outputValue = outputNonlinearLayer->getHiddenValue();


	shared_ptr<AbstractMatrix> subtract = outputValue->subtract(v);
	shared_ptr<AbstractMatrix> errorForOutput = subtract->multiple(subtract)->multiple(0.5);

	cout << "error:" << errorForOutput->sum() << endl;

	shared_ptr<AbstractMatrix> outputGradient = subtract->multiple(-1);
	outputNonlinearLayer->setHiddenGradient(outputGradient);
	outputNonlinearLayer->gradient();
	shared_ptr<AbstractMatrix> non1 = outputNonlinearLayer->getVisualGradient();

	// !ERROR
	size_t sparseGradient;
	try{
		if (sparseValue != 0 && sparseValue != 1){
			sparseGradient = sparsePercent*(sparseRate*(-1) / sparseValue + (1 - sparseRate) / (1 - sparseValue));
		}
		else{
			sparseGradient = 0;
		}
	}
	catch (exception& e){
		sparseGradient = 0;
	}
	cout <<"sparseGradient:"<<sparseGradient << endl;
	hiddenGradient = non1->transpose()->multipleRight(this->decoderWeight)->transpose()
		->add(sparseGradient);

	decoderBiasGradient = decoderBiasGradient->add(non1);
	nonlinearLayer->setHiddenGradient(hiddenGradient);
	nonlinearLayer->gradient();
	shared_ptr<AbstractMatrix> non2 = nonlinearLayer->getVisualGradient();
	visualGradient = non2->transpose()->multipleRight(this->weight)->transpose();

	weightGradient = weightGradient->add(non2->multipleRight(v->transpose()));
	biasGradient = biasGradient->add(non2);
}

size_t SparseAutoEncoder::getSparseRate(){
	return this->sparseRate;
}

void SparseAutoEncoder::setSparseRate(size_t sparseRate){
	this->sparseRate = sparseRate;
}

size_t SparseAutoEncoder::getSparsePercent(){
	return this->sparsePercent;
}

void SparseAutoEncoder::setSparsePercent(size_t sparsePercent){
	this->sparsePercent = sparsePercent;
}
string SparseAutoEncoder::getNetworkName(){
	return "SparseAutoEncoder";
}
/*int main(){
	srand(1);
	SparseAutoEncoder *ae = new SparseAutoEncoder(5, 3, 0, NonLinearFactory::SIGMOID,0.1,0.1);
	ae->setWeightLearningRate(0.2);
	ae->setBiasLearningRate(0.2);
	ae->setRegularizationRate(1);
	ae->setPreTrainingTime(100);
	ae->setSelfBatchSize(1);
	ae->preTraining("temp.txt");
	while (1);
}*/