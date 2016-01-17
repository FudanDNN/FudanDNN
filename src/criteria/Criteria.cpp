#include "criteria\Criteria.h"


size_t Criteria::getVisualUnit()
{
	return this->visualUnit;
}

void Criteria::setPredictValue(shared_ptr<Matrix> predictValue)
{
	this->predictValue = predictValue;
}

shared_ptr<Matrix> Criteria::getPredictValue()
{
	return this->predictValue;
}

void Criteria::setExpectedValue(shared_ptr<Matrix> expectedValue)
{
	this->expectedValue = expectedValue;
}

void Criteria::setVisualUnit(size_t visualUnit)
{
	this->visualUnit = visualUnit;
}

shared_ptr<Matrix> Criteria::getExpectedValue()
{
	return this->expectedValue;
}

shared_ptr<Matrix> Criteria::getPredictGradient()
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

	fout << visualUnit << endl;

	fout.close();
}
int Criteria::getPredictType(){
	int maxIndex = 0;
	double maxValue = -99;
	for (int i = 0; i < predictValue->getRowSize(); i++){
		double temp = predictValue->getValue(i, 0);
		if (temp >= maxValue){
			maxValue = temp;
			maxIndex = i;
		}
	}
	return maxIndex;
}
int Criteria::getExpectedType(){
	int maxIndex = 0;
	double maxValue = -99;
	for (int i = 0; i < expectedValue->getRowSize(); i++){
		double temp = expectedValue->getValue(i, 0);
		if (temp >= maxValue){
			maxValue = temp;
			maxIndex = i;
		}
	}
	return maxIndex;
}

string Criteria::getType()
{
	return type;
}