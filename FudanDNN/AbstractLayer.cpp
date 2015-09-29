#include "stdafx.h"
#include "AbstractLayer.h"
#include "Matrix.h"

void AbstractLayer::setVisualValue(shared_ptr<AbstractMatrix> visualValue)
{
	this->visualValue = visualValue;
}

void AbstractLayer::setHiddenGradient(shared_ptr<AbstractMatrix> hiddenGradient)
{
	if (this->hiddenGradient == nullptr) {
		this->hiddenGradient = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, 1));
		this->hiddenGradient->initializeValue(0, 0);
		this->hiddenGradient = this->hiddenGradient->add(hiddenGradient);
	}
	else{
		shared_ptr<AbstractMatrix> releaseHidden = this->hiddenGradient;
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