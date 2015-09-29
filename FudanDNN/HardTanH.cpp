#include "stdafx.h"
#include "HardTanH.h"

HardTanH::HardTanH(size_t size)
{
	this->visualUnit = size;
	this->hiddenUnit = size;
}

size_t HardTanH::getSize()
{
	return this->visualUnit;
}

void HardTanH::compute()
{
	this->hiddenValue = this->visualValue->map(hardTanH);
}

void HardTanH::calculate()
{
	this->hiddenGradient = this->visualValue->map(hardTanH);
}

void HardTanH::gradient()
{
	this->visualGradient = this->hiddenGradient->multiple(this->hiddenValue->map(hardTanHDerivative));
}
string HardTanH::getNetworkName(){
	return "HardTanH";
}