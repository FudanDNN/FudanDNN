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

	/*cout << "gradient" << endl;
	predictGradient->print();
	cout << "predictValue" << endl;
	predVec->print();
	cout << "expectedValue" << endl;
	expectedValue->print();*/
}

double Softmax::computeError()
{
	predVec = predictValue->sub(predictValue->max());
	predVec->mapi(exp);
	predVec->normalize_i(1);

	shared_ptr<Matrix> vec = predVec->map([](double x) { return log(x <= 1e-5 ? 1e-5 : x); });
	vec->mulewi(expectedValue);
	return vec->sum()*(-1);
}

string Softmax::getType()
{
	return "Softmax";
}

shared_ptr<Matrix> Softmax::getAnswer()
{
	predVec = predictValue->add(-predictValue->max());
	predVec->map(exp, predVec);
	double sum = predVec->sum();
	predVec->muli(1 / sum);
	return predVec;
}
