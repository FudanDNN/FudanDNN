#include "../header/layer/Layer.h"
#include "../header/util/Matrix.h"


//update error
void Layer::setVisualValue(shared_ptr<Matrix> visualValue)
{
	this->visualValue = visualValue;
}

//add hidden layer gradient
void Layer::addHiddenGradient(shared_ptr<Matrix> hiddenGradient)
{
	if (this->hiddenGradient == nullptr) {
		this->hiddenGradient = shared_ptr<Matrix>(new Matrix(hiddenRow, hiddenColumn));
		this->hiddenGradient->initializeValue(0, 0);
		this->hiddenGradient = this->hiddenGradient->add(hiddenGradient);
	}
	else{
		shared_ptr<Matrix> releaseHidden = this->hiddenGradient;
		this->hiddenGradient = this->hiddenGradient->add(hiddenGradient);
	}
}

shared_ptr<Matrix> Layer::getVisualValue()
{
	return visualValue;
}

shared_ptr<Matrix> Layer::getHiddenValue()
{
	return hiddenValue;
}

shared_ptr<Matrix> Layer::getVisualGradient()
{
	return visualGradient;
}

shared_ptr<Matrix> Layer::getHiddenGradient()
{
	return hiddenGradient;
}

size_t Layer::getVisualRow()
{
	return this->visualRow;
};

size_t Layer::getVisualColumn()
{
	return this->visualColumn;
};

size_t Layer::getHiddenRow()
{
	return this->hiddenRow;
};

size_t Layer::getHiddenColumn()
{
	return this->hiddenColumn;
};

void Layer::setNetworkName(string networkName)
{
	this->networkname = networkName;
};

// rate for regularize
double Layer::getRegularizationRate()
{
	return this->regularizationRate;
};
void Layer::setRegularizationRate(double regularizationRate)
{
	this->regularizationRate = regularizationRate;
};

// learning rate for weight
double Layer::getWeightLearningRate()
{
	return this->weightLearningRate;
};
void Layer::setWeightLearningRate(double weightLearningRate)
{
	this->weightLearningRate = weightLearningRate;
};

// learning rate for bias
double Layer::getBiasLearningRate()
{
	return this->biasLearningRate;
};
void Layer::setBiasLearningRate(double biasLearningRate)
{
	this->biasLearningRate = biasLearningRate;
};

// rate for dropout
double Layer::getDropoutRate()
{
	return this->dropoutRate;
};
void Layer::setDropoutRate(double dropoutRate)
{
	this->dropoutRate = dropoutRate;
};

// rate for momentum
double Layer::getMomentumRate()
{
	return this->momentumRate;
};
void Layer::setMomentumRate(double momentumRate)
{
	this->momentumRate = momentumRate;
};