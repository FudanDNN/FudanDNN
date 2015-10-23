#include "SoftShrink.h"

SoftShrink::SoftShrink(size_t size, double lambda)
{
	this->visualUnit = size;
	this->hiddenUnit = size;
	this->lambda = lambda;
}

size_t SoftShrink::getSize()
{
	return this->visualUnit;
}

void SoftShrink::compute()
{
	//this->hiddenValue = this->visualValue->map((*softShrink)(lambda));
}

void SoftShrink::calculate()
{

}

void SoftShrink::gradient()
{

}
string SoftShrink::getNetworkName(){
	return "SoftShrink";
}
