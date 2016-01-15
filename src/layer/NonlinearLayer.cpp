#include "layer/NonlinearLayer.h"

NonlinearLayer::NonlinearLayer(shared_ptr<Function> function, string name, size_t rowSize, size_t columnSize, size_t featureMap)
{
	this->fun = function;
	this->name = name;
	this->visualRow = this->hiddenRow = rowSize;
	this->visualColumn = this->hiddenColumn = columnSize;
	this->visualSize = this->hiddenSize = featureMap;
}

string NonlinearLayer::getNetworkName()
{
	return name;
}

void NonlinearLayer::readSelf(string)
{}

void NonlinearLayer::writeSelf(string)
{}

void NonlinearLayer::compute()
{
	for (size_t f = 0; f < visualSize; f++)
	for (size_t i = 0; i < visualRow; i++)
	for (size_t j = 0; j < visualColumn; j++)
		hiddenValue[f]->setValue(i, j, fun->getValue(visualValue[f]->getValue(i, j)));

}

void NonlinearLayer::calculate()
{
	for (size_t f = 0; f < visualSize; f++)
	for (size_t i = 0; i < visualRow; i++)
	for (size_t j = 0; j < visualColumn; j++)
		hiddenValue[f]->setValue(i, j, fun->getValue(visualValue[f]->getValue(i, j)));

}

void NonlinearLayer::gradient()
{
	for (size_t f = 0; f < visualSize; f++)
	for (size_t i = 0; i < visualRow; i++)
	for (size_t j = 0; j < visualColumn; j++)
		visualGradient[f]->setValue(i, j, hiddenGradient[f]->getValue(i, j) * fun->getDerivate(visualValue[f]->getValue(i, j)));
}