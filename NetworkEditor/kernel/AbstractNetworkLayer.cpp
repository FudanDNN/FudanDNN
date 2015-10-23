#include "AbstractNetworkLayer.h"
#include "Matrix.h"

void AbstractNetworkLayer::update()
{

	weightMomentum->multiple_inplace(momentumRate)->add_inplace(weightGradient->multiple_inplace(weightLearningRate));
	weight->add_inplace(weightMomentum);
	biasMomentum->multiple_inplace(momentumRate)->add_inplace(biasGradient->multiple_inplace(biasLearningRate));
	bias->add_inplace(biasMomentum);

	weightGradient->setAllValue(0);
	biasGradient->setAllValue(0);

}

void AbstractNetworkLayer::regularize()
{
	weight->multiple_inplace(1 - regularizationRate);
	bias->multiple_inplace(1 - regularizationRate);
}

shared_ptr<AbstractMatrix> AbstractNetworkLayer::getWeight()
{
	return weight;
}

void AbstractNetworkLayer::setWeight(shared_ptr<AbstractMatrix> weight)
{
	this->weight = weight;
}

shared_ptr<AbstractMatrix> AbstractNetworkLayer::getBias()
{
	return bias;
}

void AbstractNetworkLayer::setBias(shared_ptr<AbstractMatrix> bias)
{
	this->weight = weight;
}

void AbstractNetworkLayer::generateDropout()
{
	for (size_t i = 0; i < hiddenUnit; i++)
	{
		for (size_t j = 0; j < visualUnit; j++)
		{
			dropout->setValue(i, j, rand() > dropoutRate ? 1 : 0);
		}
	}
}

double AbstractNetworkLayer::getRegularizationRate()
{
	return regularizationRate;
}

void AbstractNetworkLayer::setRegularizationRate(double regularizationRate)
{
	this->regularizationRate = regularizationRate;
}

double AbstractNetworkLayer::getWeightLearningRate()
{
	return weightLearningRate;
}

void AbstractNetworkLayer::setWeightLearningRate(double weightLearningRate)
{
	this->weightLearningRate = weightLearningRate;
}

double AbstractNetworkLayer::getBiasLearningRate()
{
	return biasLearningRate;
}

void AbstractNetworkLayer::setBiasLearningRate(double biasLearningRate)
{
	this->biasLearningRate = biasLearningRate;
}

void AbstractNetworkLayer::init()
{
	if (initialized){
		return;
	}
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

	weight = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, visualUnit));
	bias = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, 1));

	weight->initializeValue(lowerBound, upperBound);
	bias->initializeValue(lowerBound, upperBound);

	initialization();
	initialized = true;
}

void AbstractNetworkLayer::initialization() 
{
	weightGradient = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, visualUnit));
	biasGradient = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, 1));

	weightGradient->setAllValue(0);
	biasGradient->setAllValue(0);

	weightMomentum = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, visualUnit));
	biasMomentum = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, 1));

	weightMomentum->setAllValue(0);
	biasMomentum->setAllValue(0);
}

double AbstractNetworkLayer::getMomentumRate(){
	return this->momentumRate;
}

void AbstractNetworkLayer::setMomentumRate(double momentumRate){
	this->momentumRate = momentumRate;
}

string AbstractNetworkLayer::getNetworkName(){
	return "AbstractNetworkLayer";
}

double AbstractNetworkLayer::getDropoutRate(){
	return this->dropoutRate;
}

void AbstractNetworkLayer::setDropoutRate(double dropoutRate){
	this->dropoutRate = dropoutRate;
}
