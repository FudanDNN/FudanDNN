#include "layer\LinearLayer.h"

LinearLayer::LinearLayer(size_t visualUnit, size_t hiddenUnit, size_t initScheme,double dropoutRate, shared_ptr<Solver> solver, size_t num)
{
	this->visualColumn = 1;
	this->visualRow = visualUnit;
	this->hiddenColumn = 1;
	this->hiddenRow = hiddenUnit;
	this->solver = solver;
	this->initScheme = initScheme;
	this->visualSize = num;
	this->hiddenSize = num;
	this->dropoutRate = dropoutRate;
	instance = MatrixPool::getInstance();
	this->init(initScheme);
}


void LinearLayer::init(int init_scheme)
{
	Layer::init();
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

	for (int i = 0; i < visualSize; i++){
		shared_ptr<Matrix> w = instance->allocMatrixUnclean(hiddenRow, visualColumn);
		w->initializeRandom(lowerBound, upperBound);
		weight.push_back(w);

		bias.push_back(instance->allocMatrixUnclean(hiddenRow, 1));
		bias[i]->initializeRandom(lowerBound, upperBound);

		weightGradient.push_back(instance->allocMatrix(hiddenRow, visualRow));
		biasGradient.push_back(instance->allocMatrix(hiddenRow, 1));

		weightMomentum.push_back(instance->allocMatrix(hiddenRow, visualRow));
		biasMomentum.push_back(instance->allocMatrix(hiddenRow, 1));
	}

}

void LinearLayer::compute()
{
	for (int i = 0; i < hiddenSize; i++){
		hiddenValue[i]->setValues(0);
		hiddenValue[i]->addi(visualValue[i]->mull(weight[i])->add(bias[i]));
		hiddenGradient[i]->setValues(0);
	}
}

void LinearLayer::calculate()
{
	for (int i = 0; i < hiddenSize; i++){
		hiddenValue[i]->setValues(0);
		hiddenValue[i]->addi(visualValue[i]->mull(weight[i])->add(bias[i]));
		hiddenGradient[i]->setValues(0);
	}
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
	for (int i = 0; i < visualSize; i++){
		visualGradient[i]->setValues(0);
		visualGradient[i]->addi(hiddenGradient[i]->trans()->mulr(this->weight[i])->trans());

		weightGradient[i]->addi(visualValue[i]->trans()->mull(hiddenGradient[i]));
		biasGradient[i]->addi(hiddenGradient[i]);

		visualValue[i]->setValues(0);
	}

	/*cout << "linearLayer:" << this->getVisualRow() << endl;
	cout << "hiddenGradient" << endl;
	hiddenGradient[0]->print();
	cout << "visualGradient" << endl;
	visualGradient[0]->print();
	cout << "weight" << endl;
	weight[0]->print();*/
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