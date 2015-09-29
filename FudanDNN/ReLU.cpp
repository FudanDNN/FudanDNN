#include "stdafx.h"
#include "ReLU.h"

ReLU::ReLU(size_t size)
{
	this->visualUnit = size;
	this->hiddenUnit = size;
}

size_t ReLU::getSize()
{
	return this->visualUnit;
}

void ReLU::compute()
{
	this->hiddenValue = this->visualValue->map(reLU);
}

void ReLU::calculate()
{
	this->hiddenGradient = this->visualValue->map(reLU);
}

void ReLU::gradient()
{
	this->visualGradient = this->hiddenGradient->multiple(this->hiddenValue->map(reLUDerivative));
}
string ReLU::getNetworkName(){
	return "ReLU";
}