#include "layer/Layer.h"
#include "util/Matrix.h"


//update error
void Layer::setVisualValue(vector<shared_ptr<Matrix>> visualValue)
{
	for (int i = 0; i < visualSize; i++){
		this->visualValue[i]->addi(visualValue[i]);
	}
}

//add hidden layer gradient
void Layer::addHiddenGradient(vector<shared_ptr<Matrix>> hiddenGradient)
{
	for (int i = 0; i < hiddenSize; i++){
		this->hiddenGradient[i]->addi(hiddenGradient[i]);
	}
}

vector<shared_ptr<Matrix>> Layer::getVisualValue()
{
	return visualValue;
}

vector<shared_ptr<Matrix>> Layer::getHiddenValue()
{
	return hiddenValue;
}

vector<shared_ptr<Matrix>> Layer::getVisualGradient()
{
	return visualGradient;
}

vector<shared_ptr<Matrix>> Layer::getHiddenGradient()
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

// rate for dropout
double Layer::getDropoutRate()
{
	return this->dropoutRate;
};

void Layer::setDropoutRate(double dropoutRate)
{
	this->dropoutRate = dropoutRate;
};

size_t Layer::getVisualSize(){
	return this->visualSize;
}

size_t Layer::getHiddenSize(){
	return this->hiddenSize;
}

void Layer::init(){
	for (size_t v = 0; v < visualSize; v++)
	{
		visualValue.push_back(instance->allocMatrixUnclean(visualRow, visualColumn));
		visualGradient.push_back(instance->allocMatrixUnclean(visualRow, visualColumn));
	}

	for (size_t v = 0; v < hiddenSize; v++)
	{
		hiddenValue.push_back(instance->allocMatrixUnclean(hiddenRow, hiddenColumn));
		hiddenGradient.push_back(instance->allocMatrixUnclean(hiddenRow, hiddenColumn));
	}
}



