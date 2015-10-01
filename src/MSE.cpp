#include "MSE.h"
#include "Matrix.h"

MSE::MSE(size_t visualUnit)
{
	this->visualUnit = visualUnit;
	this->type = "MSE";
}

double MSE::computeError() {

	errorRate = 0;
	for (size_t i = 0; i < this->visualUnit; i++)
	{
		double diff = this->getPredictValue()->getValue(i, 0) - this->getExpectedValue()->getValue(i, 0);
		errorRate += diff * diff;
	}
	errorRate /= 2;
	return errorRate;

}

void MSE::gradient() {

	predictGradient = shared_ptr<AbstractMatrix>(new Matrix(this->visualUnit, 1));
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

string MSE::getType()
{
	return this->type;
}