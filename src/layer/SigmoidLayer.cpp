#include "layer/SigmoidLayer.h"

SigmoidLayer::SigmoidLayer(size_t rowSize, size_t columnSize, size_t featureMap)
{
	this->visualRow = this->hiddenRow = rowSize;
	this->visualColumn = this->hiddenColumn = columnSize;
	this->visualSize = this->hiddenSize = featureMap;
}

SigmoidLayer::SigmoidLayer(size_t size)
{
	this->visualRow = this->hiddenRow = 1;
	this->visualColumn = this->hiddenColumn = size;
	this->visualSize = this->hiddenSize = 1;
}

string SigmoidLayer::getNetworkName()
{
	return "SigmoidLayer";
}

void SigmoidLayer::compute()
{
	for (size_t f = 0; f < visualSize; f++)
	for (size_t i = 0; i < visualRow; i++)
	for (size_t j = 0; j < visualColumn; j++)
		hiddenValue[f]->setValue(i, j, instance->getValue(visualValue[f]->getValue(i, j)));
		
}

void SigmoidLayer::calculate() 
{
	for (size_t f = 0; f < visualSize; f++)
	for (size_t i = 0; i < visualRow; i++)
	for (size_t j = 0; j < visualColumn; j++)
		hiddenValue[f]->setValue(i, j, instance->getValue(visualValue[f]->getValue(i, j)));

}

void SigmoidLayer::gradient()
{
	for (size_t f = 0; f < visualSize; f++)
	for (size_t i = 0; i < visualRow; i++)
	for (size_t j = 0; j < visualColumn; j++)
		visualGradient[f]->setValue(i, j, instance->getDerivate(visualValue[f]->getValue(i, j), visualGradient[f]->getValue(i, j)));
}

size_t SigmoidLayer::getSize()
{
	return this->visualColumn;
}