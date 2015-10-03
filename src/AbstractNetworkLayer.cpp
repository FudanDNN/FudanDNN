#include "AbstractNetworkLayer.h"
#include "Matrix.h"

void AbstractNetworkLayer::update()
{

	weightMomentum->multiple_inplace(momentumRate)->add_inplace(weightGradient->multiple_inplace(1 - momentumRate));
	weight->add_inplace(weightMomentum->multiple(weightLearningRate));
	biasMomentum->multiple_inplace(momentumRate)->add_inplace(biasGradient->multiple_inplace(1 - momentumRate));
	bias->add_inplace(biasMomentum->multiple(biasLearningRate));

	weightGradient->initializeValue(0, 0);
	biasGradient->initializeValue(0, 0);

}

void AbstractNetworkLayer::regularize()
{
	weight = weight->multiple(1 - regularizationRate);
	bias = bias->multiple(1 - regularizationRate);
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

void AbstractNetworkLayer::initialization(int init_scheme)
{

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

	weightGradient = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, visualUnit));
	biasGradient = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, 1));

	weightGradient->initializeValue(0, 0);
	biasGradient->initializeValue(0, 0);

	weightMomentum = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, visualUnit));
	biasMomentum = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, 1));

	weightMomentum->initializeValue(0, 0);
	biasMomentum->initializeValue(0, 0);

}
string AbstractNetworkLayer::getNetworkName(){
	return "AbstractNetworkLayer";
}