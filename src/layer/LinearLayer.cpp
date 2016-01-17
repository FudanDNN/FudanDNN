#include "layer\LinearLayer.h"

LinearLayer::LinearLayer(size_t visualUnit, size_t hiddenUnit, size_t initScheme,double dropoutRate, shared_ptr<Solver> solver)
{
	this->visualColumn = 1;
	this->visualRow = visualUnit;
	this->hiddenColumn = 1;
	this->hiddenRow = hiddenUnit;
	this->solver = solver;
	this->initScheme = initScheme;
	this->visualSize = 1;
	this->hiddenSize = 1;
	this->dropoutRate = dropoutRate;
	this->initialization(initScheme);
}

void LinearLayer::init()
{
	this->initialization(initScheme);
}

void LinearLayer::initialization(int init_scheme)
{
	if (initialized){
		return;
	}
	double lowerBound = 0;
	double upperBound = 0;

	switch (init_scheme)
	{
	case RANDOM:
		lowerBound = -1;
		upperBound = 1;
		break;
	case RANDOM_SQRT:
		lowerBound = -1.0 / sqrt(visualRow);
		upperBound = 1.0 / sqrt(visualRow);
		break;
	case RANDOM_NORM1:
		lowerBound = -1.0 / visualRow;
		upperBound = 1.0 / visualRow;
		break;
	default:
		;
	}

	weight = shared_ptr<Matrix>(new Matrix(hiddenRow, visualRow));
	bias = shared_ptr<Matrix>(new Matrix(hiddenRow, 1));

	weight->initializeRandom(lowerBound, upperBound);
	bias->initializeRandom(lowerBound, upperBound);

	initGradient();
	initialized = true;
}

void LinearLayer::initGradient()
{
	weightGradient= shared_ptr<Matrix>(new Matrix(hiddenRow, visualRow));
	biasGradient = shared_ptr<Matrix>(new Matrix(hiddenRow, 1));

	weightGradient->setValues(0);
	biasGradient->setValues(0);

	weightMomentum = shared_ptr<Matrix>(new Matrix(hiddenRow, visualRow));
	biasMomentum = shared_ptr<Matrix>(new Matrix(hiddenRow, 1));

	weightMomentum->setValues(0);
	biasMomentum->setValues(0);
}

void LinearLayer::compute()
{
	hiddenValue.push_back(visualValue[0]->mull(weight)->add(bias));
}

void LinearLayer::calculate()
{
	hiddenValue.push_back(visualValue[0]->mull(weight)->add(bias));
	/*cout << "linearLayer:" << this->getVisualRow() << endl;
	cout << "visualValue" << endl;
	visualValue[0]->print();
	cout << "hiddenValue" << endl;
	hiddenValue[0]->print();
	cout << "weight" << endl;
	weight->print();*/
}

void LinearLayer::gradient()
{
	visualGradient.push_back(hiddenGradient[0]->trans()->mulr(this->weight)->trans());

	cout << "linearLayer:" << this->getVisualRow() << endl;
	cout << "hiddenGradient" << endl;
	hiddenGradient[0]->print();
	cout << "visualGradient" << endl;
	visualGradient[0]->print();
	cout << "weight" << endl;
	weight->print();

	weightGradient->addi(visualValue[0]->trans()->mull(hiddenGradient[0]));
	biasGradient->addi(hiddenGradient[0]);
	hiddenGradient.clear();
}

void LinearLayer::writeSelf(string filename)
{

}

void LinearLayer::readSelf(string filename)
{

}
string LinearLayer::getNetworkName(){
	return "LinearLayer";
}

void LinearLayer::update(){
	this->solver->update(weight, weightMomentum, weightGradient, bias, biasMomentum, biasGradient, 1);
}