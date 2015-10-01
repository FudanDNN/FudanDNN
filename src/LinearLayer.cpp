#include "LinearLayer.h"

LinearLayer::LinearLayer(size_t visualUnit, size_t hiddenUnit, size_t initScheme)
{
	this->visualUnit = visualUnit;
	this->hiddenUnit = hiddenUnit;
	this->initialization(initScheme);

}

void LinearLayer::compute()
{
	hiddenValue = visualValue->multipleLeft(weight)->add(bias);
}

void LinearLayer::calculate()
{
	hiddenValue = visualValue->multipleLeft(weight)->add(bias);
}

void LinearLayer::gradient()
{
	visualGradient = hiddenGradient->transpose()->multipleRight(this->weight)->transpose();
	weightGradient->add_inplace(visualValue->transpose()->multipleLeft(hiddenGradient));
	biasGradient->add_inplace(hiddenGradient);
	hiddenGradient->initializeValue(0, 0);

}

void LinearLayer::writeSelf(string filename)
{
	ofstream fout(filename, ios::out);

	fout << visualUnit;
	fout << hiddenUnit;

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		for (size_t j = 0; j < visualUnit; j++)
		{
			fout << weight->getValue(i, j);
		}
	}

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		fout << weight->getValue(i, 0);
	}

	fout.close();
}

void LinearLayer::readSelf(string filename)
{

	ifstream fin(filename, ios::in);

	fin >> visualUnit;
	fin >> hiddenUnit;

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		for (size_t j = 0; j < visualUnit; j++)
		{
			double d_value;
			fin >> d_value;
			weight->setValue(i, j, d_value);
		}
	}

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		double d_value;
		fin >> d_value;
		bias->setValue(i, 0, d_value);
	}

	this->weightGradient = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, visualUnit));
	this->biasGradient = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, 1));

	fin.close();

}
string LinearLayer::getNetworkName(){
	return "LinearLayer";
}