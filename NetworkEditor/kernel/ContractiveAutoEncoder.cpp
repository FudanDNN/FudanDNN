#include "ContractiveAutoEncoder.h"
#include "Sigmoid.h"
#include "Matrix.h"
ContractiveAutoEncoder::ContractiveAutoEncoder(size_t visualUnit, size_t hiddenUnit, int init_scheme, size_t nonlinearType,
	size_t jacobiPenalty):
	AutoEncoder(visualUnit, hiddenUnit, init_scheme, nonlinearType){
	this->jacobiPenalty = jacobiPenalty;
}

void ContractiveAutoEncoder::singleTraining(shared_ptr<AbstractMatrix> v){
	shared_ptr<AbstractMatrix> temp1 = v->multipleLeft(weight)->add(bias);
	nonlinearLayer->setVisualValue(temp1);
	nonlinearLayer->compute();
	hiddenValue = nonlinearLayer->getHiddenValue();

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
	shared_ptr<AbstractMatrix> jacobiGradient = hiddenValue->multiple(hiddenValue->multiple(-1)->add(1))->
		multiple(hiddenValue->multiple(-1)->add(1))->multiple(2)->multiple(weight->multiple(weight)->sum());
	hiddenGradient = non1->transpose()->multipleRight(this->decoderWeight)->transpose()->add(jacobiGradient);

	decoderBiasGradient = decoderBiasGradient->add(non1);
	nonlinearLayer->setHiddenGradient(hiddenGradient);
	nonlinearLayer->gradient();
	shared_ptr<AbstractMatrix> non2 = nonlinearLayer->getVisualGradient();
	visualGradient = non2->transpose()->multipleRight(this->weight)->transpose();

	weightGradient = weightGradient->add(non2->multipleRight(v->transpose()));
	biasGradient = biasGradient->add(non2);
}

size_t ContractiveAutoEncoder::getJacobiPenalty(){
	return this->jacobiPenalty;
}

void ContractiveAutoEncoder::setJacobiPenalty(size_t jacobiPenalty){
	this->jacobiPenalty = jacobiPenalty;
}
string ContractiveAutoEncoder::getNetworkName(){
	return "ContractiveAutoEncoder";
}
/*int main(){
	srand(1);
	ContractiveAutoEncoder *ae = new ContractiveAutoEncoder(5, 3, 0, NonLinearFactory::SIGMOID,0.1);
	ae->setWeightLearningRate(0.2);
	ae->setBiasLearningRate(0.2);
	ae->setRegularizationRate(1);
	ae->setPreTrainingTime(100);
	ae->setSelfBatchSize(1);
	ae->preTraining("temp.txt");
	while (1);
}*/