#include "layer/SigmoidLayer.h"

SigmoidLayer::SigmoidLayer(size_t rowSize, size_t columnSize)
{
	this->visualRow = this->hiddenRow = rowSize;
	this->visualColumn = this->hiddenColumn = columnSize;
}

SigmoidLayer::SigmoidLayer(size_t size)
{
	this->visualRow = this->hiddenRow = 1;
	this->visualColumn = this->hiddenColumn = size;
}

string SigmoidLayer::getNetworkName()
{
	return "SigmoidLayer";
}

void SigmoidLayer::compute()
{
	for (shared_ptr<Matrix> m : hiddenValue)
	for (size_t i = 0; i < visualRow; i++)
	for (size_t j = 0; j < visualColumn; j++) 
		hiddenValue.
}
void calculate() override;
void gradient() override;
void writeSelf(string) override;
void readSelf(string) override;
size_t getSize();