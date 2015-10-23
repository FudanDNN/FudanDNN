#include "Sigmoid.h"

Sigmoid::Sigmoid(size_t size)
{
	this->visualUnit = size;
	this->hiddenUnit = size;
}

size_t Sigmoid::getSize()
{
	return this->visualUnit;
}

void Sigmoid::compute()
{
	this->hiddenValue = this->visualValue->map(sigmoid);
}

void Sigmoid::calculate()
{
	this->hiddenValue = this->visualValue->map(sigmoid);
}

void Sigmoid::gradient()
{
	this->visualGradient = this->hiddenGradient->multiple(this->hiddenValue->map(sigmoidDerivative));
	this->hiddenGradient->setAllValue(0);

}
string Sigmoid::getNetworkName(){
	return "Sigmoid";
}
