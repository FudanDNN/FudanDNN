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
	this->hiddenValue = this->visualValue->map(reLU);

}

void ReLU::gradient()
{
	this->visualGradient = this->hiddenGradient->multiple(this->hiddenValue->map(reLUDerivative));
	/*cout << "Visual Value" << endl;
	this->visualValue->print();
	cout << "Hidden Value" << endl;
	this->hiddenValue->print();
	cout << "Hidden Gradient" << endl;
	this->hiddenGradient->print();
	cout << "Visual Gradient" << endl;
	this->visualGradient->print();

	cout << "SD" << endl;*/
	this->hiddenGradient->setAllValue(0);

}

string ReLU::getNetworkName(){
	return "ReLU";
}