#include "AutoEncoder.h"
#include "Sigmoid.h"
#include "Matrix.h"

AutoEncoder::AutoEncoder(size_t visualUnit, size_t hiddenUnit, int init_scheme, size_t nonlinearType)
{
	this->visualUnit = visualUnit;
	this->hiddenUnit = hiddenUnit;
	this->initialization(init_scheme);

	double lowerBound = 0;
	double upperBound = 0;

	switch (init_scheme)
	{
	case RANDOM:
		lowerBound = -1;
		upperBound = 1;
		break;
	case RANDOM_SQRT:
		lowerBound = -1.0 / sqrt(visualUnit);
		upperBound = 1.0 / sqrt(visualUnit);
		break;
	case RANDOM_NORM1:
		lowerBound = -1.0 / visualUnit;
		upperBound = 1.0 / visualUnit;
		break;
	default:
		;
	}
	decoderWeight = weight->transpose();
	decoderBias = shared_ptr<Matrix>(new Matrix(visualUnit, 1));

	decoderBias->initializeValue(lowerBound, upperBound);

	decoderBiasGradient = shared_ptr<Matrix>(new Matrix(visualUnit, 1));

	decoderBiasGradient->initializeValue(0, 0);

	this->nonLinearType = nonlinearType;

	NonLinearFactory *factory = new NonLinearFactory();
	nonlinearLayer = factory->createNonLinear(nonlinearType, hiddenUnit);
	outputNonlinearLayer = factory->createNonLinear(nonlinearType, visualUnit);
}

//正向传播，计算得到output层
void AutoEncoder::compute() {
	//计算z=w*v+b
	shared_ptr<AbstractMatrix> z = visualValue->multipleLeft(weight)->add(bias);
	//计算经过非线性层得到的hiddenValue
	nonlinearLayer->setVisualValue(z);
	nonlinearLayer->compute();
	hiddenValue = nonlinearLayer->getHiddenValue();
}

void AutoEncoder::calculate() {
	//计算z=w*v+b
	shared_ptr<AbstractMatrix> z = visualValue->multipleLeft(weight)->add(bias);
	//计算经过非线性层得到的hiddenValue
	nonlinearLayer->setVisualValue(z);
	nonlinearLayer->compute();
	hiddenValue = nonlinearLayer->getHiddenValue();
}

void AutoEncoder::gradient() {
	nonlinearLayer->setHiddenGradient(hiddenGradient);
	nonlinearLayer->gradient();

	shared_ptr<AbstractMatrix> h2 = nonlinearLayer->getVisualGradient();

	visualGradient = h2->transpose()->multipleRight(this->weight)->transpose();

	weightGradient->add(visualValue->transpose()->multipleLeft(h2));
	biasGradient->add(h2);

}


void AutoEncoder::selfUpdate(bool regularize){
	if (regularize)
	{
		weight = weight->multiple(regularizationRate);
		bias = bias->multiple(regularizationRate);
		decoderWeight = decoderWeight->multiple(regularizationRate);
		decoderBias = decoderBias->multiple(regularizationRate);
	}

	weight = weight->add(weightGradient->multiple(weightLearningRate));

	decoderWeight = weight->transpose();

	bias = bias->add(biasGradient->multiple(biasLearningRate));

	decoderBias = decoderBias->add(decoderBiasGradient->multiple(biasLearningRate));

	weightGradient->initializeValue(0, 0);
	biasGradient->initializeValue(0, 0);

	decoderBiasGradient->initializeValue(0, 0);

}

void AutoEncoder::singleTraining(shared_ptr<AbstractMatrix> v){
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

	cout << "error:" << errorForOutput->sum()<< endl;

	shared_ptr<AbstractMatrix> outputGradient = subtract->multiple(-1);
	//将非线性层hiddenGradient设为计算出的误差值
	outputNonlinearLayer->setHiddenGradient(outputGradient);
	outputNonlinearLayer->gradient();
	shared_ptr<AbstractMatrix> non1 = outputNonlinearLayer->getVisualGradient();
	//得到hidden层误差
	hiddenGradient = non1->transpose()->multipleRight(this->decoderWeight)->transpose();
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

void AutoEncoder::setNonlinear(shared_ptr<NonLinearLayer> nonlinearLayer){
	this->nonlinearLayer = nonlinearLayer;
}

void AutoEncoder::setSelfBatchSize(size_t selfBatchSize)
{
	this->selfBatchSize = selfBatchSize;
}

size_t AutoEncoder::getSelfBatchSize()
{
	return this->selfBatchSize;
}

void AutoEncoder::setPreTrainingTime(size_t preTrainingTime)
{
	this->preTrainingTime = preTrainingTime;
}

size_t AutoEncoder::getPreTrainingTime()
{
	return this->preTrainingTime;
}

void AutoEncoder::writeSelf(string filename) 
{

	ofstream fout(filename, ios::out);

	fout << visualUnit;
	fout << hiddenUnit;

	fout << nonLinearType;

	fout << selfBatchSize;
	fout << preTrainingTime;

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		for (size_t j = 0; j < visualUnit; j++)
		{
			fout << weight->getValue(i, j) << ' ';
		}
	}

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		fout << bias->getValue(i, 0) << ' ';
	}

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		for (size_t j = 0; j < visualUnit; j++)
		{
			fout << decoderWeight->getValue(i, j) << ' ';
		}
	}

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		fout << decoderBias->getValue(i, 0) << ' ';
	}

	fout << nonLinearType;

	fout.close();

}

void AutoEncoder::readSelf(string filename)
{

	ifstream fin(filename, ios::in);

	fin >> visualUnit;
	fin >> hiddenUnit;

	fin >> nonLinearType;

	fin >> selfBatchSize;
	fin >> preTrainingTime;

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		for (size_t j = 0; j < visualUnit; j++)
		{
			double d;
			fin >> d;
			weight->setValue(i, j, d);
		}
	}

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		double d;
		fin >> d;
		bias->setValue(i, 0, d);
	}

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		for (size_t j = 0; j < visualUnit; j++)
		{
			double d;
			fin >> d;
			decoderWeight->setValue(i, j, d);
		}
	}

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		double d;
		fin >> d;
		decoderBias->setValue(i, 0, d);
	}

	fin >> nonLinearType;

	fin.close();

}
string AutoEncoder::getNetworkName(){
	return "AutoEncoder";
}
/*int main(){
	srand(1);
	AutoEncoder *ae = new AutoEncoder(5,3,0,NonLinearFactory::SIGMOID);
	ae->setWeightLearningRate(0.2);
	ae->setBiasLearningRate(0.2);
	ae->setRegularizationRate(1);
	ae->setPreTrainingTime(100);
	ae->setSelfBatchSize(1);
	ae->preTraining("temp.txt");
	while (1);
}*/