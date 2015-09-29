#include "stdafx.h"
#include "Criteria.h"

Criteria::Criteria(){

}

size_t Criteria::getVisualUnit()
{
	return this->visualUnit;
}

void Criteria::setPredictValue(shared_ptr<AbstractMatrix> predictValue)
{
	this->predictValue = predictValue;
}

shared_ptr<AbstractMatrix> Criteria::getPredictValue()
{
	return this->predictValue;
}

void Criteria::setExpectedValue(shared_ptr<AbstractMatrix> expectedValue)
{
	this->expectedValue = expectedValue;
}

shared_ptr<AbstractMatrix> Criteria::getExpectedValue()
{
	return this->expectedValue;
}

shared_ptr<AbstractMatrix> Criteria::getPredictGradient()
{
	return this->predictGradient;
}

double Criteria::getErrorRate()
{
	if (errorRate < 0)
		computeError();
	return errorRate;
}

void Criteria::readSelf(string filename)
{
	ifstream fin(filename, ios::in);

	fin >> visualUnit;

	fin.close();
}

void Criteria::writeSelf(string filename)
{
	ofstream fout(filename, ios::out);

	fout << visualUnit;

	fout.close();
}