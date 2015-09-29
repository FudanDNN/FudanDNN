#include "stdafx.h"
#include "SoftShrink.h"

SoftShrink::SoftShrink(size_t size)
{
	this->visualUnit = size;
	this->hiddenUnit = size;
}

size_t SoftShrink::getSize()
{
	return this->visualUnit;
}

void SoftShrink::compute()
{

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
