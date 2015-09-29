#include "ContractiveAutoEncoder.h"
#include "Sigmoid.h"
#include "Matrix.h"
ContractiveAutoEncoder::ContractiveAutoEncoder(size_t visualUnit, size_t hiddenUnit, int init_scheme, size_t nonlinearType,
	size_t jacobiPenalty):
	AutoEncoder(visualUnit, hiddenUnit, init_scheme, nonlinearType){
	this->jacobiPenalty = jacobiPenalty;
}

void ContractiveAutoEncoder::singleTraining(shared_ptr<AbstractMatrix> v){
	//前向传播
	//计算z=w*v+b
	shared_ptr<AbstractMatrix> temp1 = v->multipleLeft(weight)->add(bias);
	nonlinearLayer->setVisualValue(temp1);
	nonlinearLayer->compute();
	hiddenValue = nonlinearLayer->getHiddenValue();

	shared_ptr<AbstractMatrix> temp2 = hiddenValue->multipleLeft(decoderWeight)->add(decoderBias);
	outputNonlinearLayer->setVisualValue(temp2);
	outputNonlinearLayer->compute();
	outputValue = outputNonlinearLayer->getHiddenValue();


	//反向传播
	//计算误差值（期望输出-实际输出)
	shared_ptr<AbstractMatrix> subtract = outputValue->subtract(v);
	//目标函数为0.5*error^2
	shared_ptr<AbstractMatrix> errorForOutput = subtract->multiple(subtract)->multiple(0.5);

	cout << "error:" << errorForOutput->sum() << endl;

	shared_ptr<AbstractMatrix> outputGradient = subtract->multiple(-1);
	//将非线性层hiddenGradient设为计算出的误差值
	outputNonlinearLayer->setHiddenGradient(outputGradient);
	outputNonlinearLayer->gradient();
	shared_ptr<AbstractMatrix> non1 = outputNonlinearLayer->getVisualGradient();
	//得到hidden层误差
	//2(h(1-h))(1-2h)*w
	shared_ptr<AbstractMatrix> jacobiGradient = hiddenValue->multiple(hiddenValue->multiple(-1)->add(1))->
		multiple(hiddenValue->multiple(-1)->add(1))->multiple(2)->multiple(weight->multiple(weight)->sum());
	hiddenGradient = non1->transpose()->multipleRight(this->decoderWeight)->transpose()->add(jacobiGradient);
	//计算hidden到output的weightGradient和biasGradient

	decoderBiasGradient = decoderBiasGradient->add(non1);
	//计算input到hidden的非线性层gradient
	nonlinearLayer->setHiddenGradient(hiddenGradient);
	nonlinearLayer->gradient();
	shared_ptr<AbstractMatrix> non2 = nonlinearLayer->getVisualGradient();
	//计算visual层gradient
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