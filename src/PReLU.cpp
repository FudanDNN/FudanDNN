#include "PReLU.h"

PReLU::PReLU(size_t size)
{
	this->visualUnit = size;
	this->hiddenUnit = size;
}

size_t PReLU::getSize()
{
	return this->visualUnit;
}

void PReLU::compute()
{

}

void PReLU::calculate()
{

}

void PReLU::gradient()
{

}
string PReLU::getNetworkName(){
	return "PReLU";
}