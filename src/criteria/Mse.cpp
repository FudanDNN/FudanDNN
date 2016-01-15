#include "criteria\Mse.h"

Mse::Mse(size_t visualUnit)
{
	this->visualUnit = visualUnit;
	this->type = "Mse";
}

double Mse::computeError()
{

	errorRate = 0;
	for (size_t i = 0; i < this->visualUnit; i++)
	{
		double diff = this->getPredictValue()->getValue(i, 0) - this->getExpectedValue()->getValue(i, 0);
		errorRate += diff * diff;
	}
	errorRate /= 2;
	return errorRate;

}

void Mse::gradient()
{

	predictGradient = shared_ptr<Matrix>(new Matrix(this->visualUnit, 1));
	for (size_t i = 0; i < this->visualUnit; i++)
	{
		predictGradient->setValue(i, 0, this->getExpectedValue()->getValue(i, 0) - this->getPredictValue()->getValue(i, 0));
	}
	/*
	cout << "ExpectetValue:" << endl;
	this->expectedValue->print();
	cout << "PredictValue:" << endl;
	this->predictValue->print();
	cout << "PredictGradient:" << endl;
	this->predictGradient->print();
	*/
}
string Mse::getType()
{
	return "Mse";
}

shared_ptr<Matrix> Mse::getAnswer()
{
	return predictValue;
}