#include "layer/ReLULayer.h"

ReLULayer::ReLULayer(size_t rowSize, size_t columnSize, size_t featureMap)
{
	this->visualRow = this->hiddenRow = rowSize;
	this->visualColumn = this->hiddenColumn = columnSize;
	this->featureMap = featureMap;
}

ReLULayer::ReLULayer(size_t size)
{
	this->visualRow = this->hiddenRow = 1;
	this->visualColumn = this->hiddenColumn = size;
	this->featureMap = 1;
}

string ReLULayer::getNetworkName()
{
	return "ReLULayer";
}

void ReLULayer::compute()
{
	for (size_t f = 0; f < featureMap; f++)
	for (size_t i = 0; i < visualRow; i++)
	for (size_t j = 0; j < visualColumn; j++)
		hiddenValue[f]->setValue(i, j, instance->getValue(visualValue[f]->getValue(i, j)));

}

void ReLULayer::calculate()
{
	for (size_t f = 0; f < featureMap; f++)
	for (size_t i = 0; i < visualRow; i++)
	for (size_t j = 0; j < visualColumn; j++)
		hiddenValue[f]->setValue(i, j, instance->getValue(visualValue[f]->getValue(i, j)));

}

void ReLULayer::gradient()
{
	for (size_t f = 0; f < featureMap; f++)
	for (size_t i = 0; i < visualRow; i++)
	for (size_t j = 0; j < visualColumn; j++)
		visualGradient[f]->setValue(i, j, instance->getDerivate(visualValue[f]->getValue(i, j)));
}

size_t ReLULayer::getSize()
{
	return this->visualColumn;
}

