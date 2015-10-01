#include "LogSigmoid.h"

LogSigmoid::LogSigmoid(size_t size)
{
	this->visualUnit = size;
	this->hiddenUnit = size;
}

size_t LogSigmoid::getSize()
{
	return this->visualUnit;
}

void LogSigmoid::compute()
{
	this->hiddenValue = this->visualValue->map(logsigmoid);
}

void LogSigmoid::calculate()
{
	this->hiddenGradient = this->visualValue->map(logsigmoid);
}

void LogSigmoid::gradient()
{
	this->visualGradient = this->hiddenGradient->multiple(this->hiddenValue->map(logsigmoidDerivative));
}
string LogSigmoid::getNetworkName(){
	return "LogSigmoid";
}