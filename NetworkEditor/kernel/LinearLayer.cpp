#include "LinearLayer.h"
LinearLayer::LinearLayer(size_t visualUnit, size_t hiddenUnit, size_t initScheme)
{
	this->visualUnit = visualUnit;
	this->hiddenUnit = hiddenUnit;
	this->init_scheme = initScheme;
	this->initialization();
}

void LinearLayer::compute()
{
	hiddenValue = visualValue->multipleLeft(weight)->add(bias);
}

void LinearLayer::calculate()
{
	/*cout << "visualValue" <<visualUnit<< endl;
	visualValue->print();*/
	/*cout << "weight" << endl;
	weight->print();*/
	hiddenValue = visualValue->multipleLeft(weight)->add(bias);
	//hiddenValue->print();
}

void LinearLayer::gradient()
{
	/*cout << "hiddenGradient" <<this->getHiddenUnit()<< endl;
	hiddenGradient->print();*/
	//cout << "weight" << endl;
	//weight->print();
	visualGradient = hiddenGradient->transpose()->multipleRight(this->weight)->transpose();
	weightGradient->add_inplace(visualValue->transpose()->multipleLeft(hiddenGradient));
	biasGradient->add_inplace(hiddenGradient);
	hiddenGradient->setAllValue(0);
	/*cout << "visualGradient" << endl;
	visualGradient->print();
	cout << "-----" << endl;*/
}

void LinearLayer::writeSelf(string filename)
{
	ofstream fout(filename, ios::out);

	fout << visualUnit << endl;
	fout << hiddenUnit << endl;

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		for (size_t j = 0; j < visualUnit; j++)
		{
			fout << weight->getValue(i, j) << ' ';
		}
		fout << endl;
	}

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		fout << weight->getValue(i, 0) << ' ';
	}
	fout << endl;

	fout.close();
}

void LinearLayer::readSelf(string filename)
{

	ifstream fin(filename, ios::in);

	fin >> visualUnit;
	fin >> hiddenUnit;

	weight = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, visualUnit));
	for (size_t i = 0; i < hiddenUnit; i++)
	{
		for (size_t j = 0; j < visualUnit; j++)
		{
			double d_value;
			fin >> d_value;
			weight->setValue(i, j, d_value);
		}
	}
	//weight->print();
	bias = shared_ptr<AbstractMatrix>(new Matrix(hiddenUnit, 1));
	for (size_t i = 0; i < hiddenUnit; i++)
	{
		double d_value;
		fin >> d_value;
		bias->setValue(i, 0, d_value);
	}

	fin.close();
	this->initialized = true;
	initialization();

}
string LinearLayer::getNetworkName(){
	return "LinearLayer";
}