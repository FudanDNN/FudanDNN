#include "criteria\Softmax.h"

Softmax::Softmax(size_t visualUnit)
{
	this->visualUnit = visualUnit;
	this->type = "Softmax";
}

void Softmax::gradient()
{
	predictGradient = shared_ptr<Matrix>(new Matrix(this->visualUnit, 1));
	for (size_t i = 0; i < this->visualUnit; i++)
	{
		predictGradient->setValue(i, 0, this->getExpectedValue()->getValue(i, 0) - this->predVec->getValue(i, 0));
	}
	/*
	qDebug() << "gradient" << endl;
	predictGradient->print();
	qDebug() << "predictValue" << endl;
	predVec->print();
	qDebug() << "expectedValue" << endl;
	expectedValue->print();*/
}

double Softmax::computeError()
{
	//qDebug() << "predictValue" << endl;
	//predictValue->print();
	predVec = predictValue->add(-predictValue->max());
	predVec = predVec->map(exp);
	double sum = predVec->sum();
	predVec->multiple_inplace(1 / sum);
	//qDebug() << "predictVec" << endl;
	//predVec->print();
	return predVec->map(log)->multiple_inplace(expectedValue)->sum()*(-1);
}

string Softmax::getType()
{
	return "Softmax";
}

shared_ptr<Matrix> Softmax::getAnswer()
{
	predVec = predictValue->add(-predictValue->max());
	predVec = predVec->map(exp);
	double sum = predVec->sum();
	predVec->multiple_inplace(1 / sum);
	return predVec;
}
