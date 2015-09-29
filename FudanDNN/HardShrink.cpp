#include "stdafx.h"
#include "HardShrink.h"

HardShrink::HardShrink(size_t size)
{
	this->visualUnit = size;
	this->hiddenUnit = size;
}

size_t HardShrink::getSize()
{
	return this->visualUnit;
}

void HardShrink::compute()
{

}

void HardShrink::calculate()
{

}

void HardShrink::gradient()
{

}
string HardShrink::getNetworkName(){
	return "HardShrink";
}