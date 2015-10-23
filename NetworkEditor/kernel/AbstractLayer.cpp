#include "AbstractLayer.h"
#include "Matrix.h"

void AbstractLayer::setVisualValue(shared_ptr<AbstractMatrix> visualValue)
{
	this->visualValue = visualValue;
}

void AbstractLayer::setHiddenGradient(shared_ptr<AbstractMatrix> hiddenGradient)
{
	if (this->hiddenGradient == nullptr) 
	{
		this->hiddenGradient = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, 1));
		this->hiddenGradient->setAllValue(0);
		this->hiddenGradient = this->hiddenGradient->add(hiddenGradient);
	}
	else
	{
		this->hiddenGradient=this->hiddenGradient->add(hiddenGradient);
	}
}

shared_ptr<AbstractMatrix> AbstractLayer::getVisualValue()
{
	return visualValue;
}

shared_ptr<AbstractMatrix> AbstractLayer::getHiddenValue()
{
	return hiddenValue;
}

shared_ptr<AbstractMatrix> AbstractLayer::getVisualGradient()
{
	return visualGradient;
}

shared_ptr<AbstractMatrix> AbstractLayer::getHiddenGradient()
{
	return hiddenGradient;
}
size_t AbstractLayer::getVisualUnit()
{
	return this->visualUnit;
}

size_t AbstractLayer::getHiddenUnit()
{
	return this->hiddenUnit;
}

void AbstractLayer::setVisualUnit(size_t visualUnit)
{
	this->visualUnit = visualUnit;
}

void AbstractLayer::setHiddenUnit(size_t hiddenUnit)
{
	this->hiddenUnit = hiddenUnit;
}