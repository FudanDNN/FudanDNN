#include "DenoiseAutoEncoder.h"
#include "Sigmoid.h"
#include "Matrix.h"
DenoiseAutoEncoder::DenoiseAutoEncoder(size_t visualUnit, size_t hiddenUnit, int init_scheme, size_t nonlinearType
	, size_t clearRate):
	AutoEncoder(visualUnit, hiddenUnit, init_scheme, nonlinearType){
	this->clearRate = clearRate;
}
void DenoiseAutoEncoder::singleTraining(shared_ptr<AbstractMatrix> v){
	for (size_t i = 0; i < v->getRowSize(); i++)
	{
		for (size_t j = 0; j < v->getColumnSize(); j++) {
			v->setValue(i, j, noise(v->getValue(i, j)));
		}
	}
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
	hiddenGradient = non1->transpose()->multipleRight(this->decoderWeight)->transpose();

	decoderBiasGradient = decoderBiasGradient->add(non1);
	nonlinearLayer->setHiddenGradient(hiddenGradient);
	nonlinearLayer->gradient();
	shared_ptr<AbstractMatrix> non2 = nonlinearLayer->getVisualGradient();
	visualGradient = non2->transpose()->multipleRight(this->weight)->transpose();

	weightGradient = weightGradient->add(non2->multipleRight(v->transpose()));
	biasGradient = biasGradient->add(non2);
}
double DenoiseAutoEncoder::noise(double x){
	return rand() > clearRate ? x : 0;
}
size_t DenoiseAutoEncoder::getClearRate(){
	return this->clearRate;
}
void DenoiseAutoEncoder::setClearRate(size_t clearRate){
	this->clearRate = clearRate;
}
string DenoiseAutoEncoder::getNetworkName(){
	return "DenoiseAutoEncoder";
}
/*int main(){
	srand(1);
	DenoiseAutoEncoder *ae = new DenoiseAutoEncoder(5, 3, 0, NonLinearFactory::SIGMOID,0.1);
	ae->setWeightLearningRate(0.2);
	ae->setBiasLearningRate(0.2);
	ae->setRegularizationRate(1);
	ae->setPreTrainingTime(100);
	ae->setSelfBatchSize(1);
	ae->preTraining("temp.txt");
	while (1);
}*/