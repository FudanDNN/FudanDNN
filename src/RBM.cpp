#include "RBM.h"

RBM::RBM(size_t visualUnit, size_t hiddenUnit, size_t nonLinearType, bool sparse, size_t initScheme)
{

	this->visualUnit = visualUnit;
	this->hiddenUnit = hiddenUnit;
	this->nonLinearType = nonLinearType;
	this->sparse = sparse;
	NonLinearFactory * factory = new NonLinearFactory();
	nonlinearLayer = factory->createNonLinear(nonLinearType, hiddenUnit);

	initialization(initScheme); 

	double lowerBound = 0;
	double upperBound = 0;

	switch (initScheme)
	{
	case RANDOM:
		lowerBound = -1;
		upperBound = 1;
		break;
	case RANDOM_SQRT:
		lowerBound = -1.0 / sqrt(visualUnit);
		upperBound = 1.0 / sqrt(visualUnit);
		break;
	case RANDOM_NORM1:
		lowerBound = -1.0 / visualUnit;
		upperBound = 1.0 / visualUnit;
		break;
	default:
		;
	}

	this->biasH2V = shared_ptr<AbstractMatrix>(new Matrix(visualUnit, 1));
	this->biasH2V->initializeValue(lowerBound, upperBound);

	this->biasH2VGradient = shared_ptr<AbstractMatrix>(new Matrix(visualUnit, 1));
	this->biasH2VGradient->initializeValue(0, 0);

}

RBM::~RBM()
{
}

void RBM::compute()
{

	shared_ptr<AbstractMatrix> h1 = visualValue->multipleLeft(this->weight)->add(this->bias);
	
	nonlinearLayer->setVisualValue(h1);
	nonlinearLayer->compute();

	hiddenValue = nonlinearLayer->getHiddenValue();

	if (sparse)
	{
		hiddenValue = hiddenValue->map(random);
	}

}

void RBM::calculate()
{

	shared_ptr<AbstractMatrix> h1 = visualValue->multipleLeft(this->weight)->add(this->bias);

	nonlinearLayer->setVisualValue(h1);
	nonlinearLayer->compute();

	hiddenValue = nonlinearLayer->getHiddenValue();

	if (sparse)
	{
		hiddenValue = hiddenValue->map(random);
	}

}

void RBM::gradient()
{


	nonlinearLayer->setHiddenGradient(hiddenGradient);
	nonlinearLayer->gradient();

	shared_ptr<AbstractMatrix> h2 = nonlinearLayer->getVisualGradient();

	visualGradient = h2->transpose()->multipleRight(this->weight)->transpose();

	weightGradient = weightGradient->add(visualValue->transpose()->multipleLeft(h2));
	biasGradient = biasGradient->add(h2);

}

void RBM::preTraining(string filename)
{

	for (size_t i = 0; i < selfTrainingTime; i++)
	{

		ifstream fin(filename, ios::in);

		size_t index = 0;
		while (!fin.eof())
		{
			index++;
			shared_ptr<AbstractMatrix> datum = readDatum(fin);
			double err = singleTraining(datum);
			if (index % 1000 == 0)
				printf("count: %d %.4f\n", index, err);
			//this->weight->print();
			//system("pause");

			if (index % selfBatchSize == 0)
				selfUpdate(false);
		}
		selfUpdate(true);

		fin.close();

	}


}

shared_ptr<AbstractMatrix> RBM::readDatum(ifstream& fin)
{

	shared_ptr<AbstractMatrix> m(new Matrix(visualUnit, 1));
	for (size_t i = 0; i < visualUnit; i++)
	{
		double d_value;
		fin >> d_value;
		m->setValue(i, 0, d_value);
	}
	
	return m;

}

double RBM::singleTraining(shared_ptr<AbstractMatrix> m)
{
	/*
	m->print();
	cout << endl;
	*/
	shared_ptr<AbstractMatrix> *v = new shared_ptr<AbstractMatrix> [sampleLength];
	shared_ptr<AbstractMatrix> *h = new shared_ptr<AbstractMatrix> [sampleLength];
	v[0] = m;
	h[0] = forwardCompute(v[0]);

	for (size_t i = 1; i < sampleLength; i++)
	{
		v[i] = backwardCompute(h[i - 1]);
		h[i] = forwardCompute(v[i]);
	}

	shared_ptr<AbstractMatrix> energyData = v[0]->transpose()->multipleLeft(h[0]);
	shared_ptr<AbstractMatrix> energyModel = v[sampleLength - 1]->transpose()->multipleLeft(h[sampleLength - 1]);
	shared_ptr<AbstractMatrix> diff = energyData->subtract(energyModel);
	
	//energyData->print();
	//cout << endl;
	//energyModel->print();
	//cout << endl;
	//diff->print();
	//cout << endl; 

	//system("pause");

	//weightGradient->print();
	//cout << endl;

	weightGradient = weightGradient->add(diff);
	biasGradient = biasGradient->add(h[0]->subtract(h[sampleLength - 1]));
	biasH2VGradient = biasH2VGradient->add(v[0]->subtract(v[sampleLength - 1]));

	//weightGradient->print();
	//cout << endl;
	//system("pause");

	return sqrt(diff->norm2());

}

shared_ptr<AbstractMatrix> RBM::forwardCompute(shared_ptr<AbstractMatrix> v)
{

	shared_ptr<AbstractMatrix> h1 = v->multipleLeft(this->weight)->add(this->bias);

	nonlinearLayer->setVisualValue(h1);
	nonlinearLayer->compute();
	
	shared_ptr<AbstractMatrix> h = nonlinearLayer->getHiddenValue();

	if (sparse)
	{
		h = h->map(random);
	}
	return h;

}

shared_ptr<AbstractMatrix> RBM::backwardCompute(shared_ptr<AbstractMatrix> h) 
{

	shared_ptr<AbstractMatrix> v1 = h->multipleLeft(this->weight->transpose())->add(this->biasH2V);

	nonlinearLayer->setVisualValue(v1);
	nonlinearLayer->compute();

	shared_ptr<AbstractMatrix> v = nonlinearLayer->getHiddenValue();

	if (sparse)
	{
		v = v->map(random);
	}
	return v;

}

void RBM::selfUpdate(bool regularize)
{
	
	if (regularize)
	{
		weight = weight->multiple(1- regularizationRate);
		bias = bias->multiple(1 - regularizationRate);
		biasH2V = biasH2V->multiple(1 - regularizationRate);
	}

	weight = weight->add(weightGradient->multiple(weightLearningRate));
	bias = bias->add(biasGradient->multiple(biasLearningRate));
	biasH2V = biasH2V->add(biasH2VGradient->multiple(biasLearningRate));

	weightGradient->initializeValue(0, 0);
	biasGradient->initializeValue(0, 0);
	biasH2VGradient->initializeValue(0, 0);

}

void RBM::setSelfBatchSize(size_t selfBatchSize)
{
	this->selfBatchSize = selfBatchSize;
}

size_t RBM::getSelfBatchSize()
{
	return this->selfBatchSize;
}

void RBM::setSelfTrainingTime(size_t selfTrainingTime)
{
	this->selfTrainingTime = selfTrainingTime;
}

size_t RBM::getSelfTrainingTime()
{
	return this->selfTrainingTime;
}

void RBM::setSampleLength(size_t sampleLength)
{
	this->sampleLength = sampleLength;
}

size_t RBM::getSampleLength()
{
	return this->sampleLength;
}

void RBM::setNonLinearType(size_t nonLinearType)
{
	this->nonLinearType = nonLinearType;
}

size_t RBM::getNonLinearType()
{
	return this->nonLinearType;
}

void RBM::setSparse(bool sparse)
{
	this->sparse = sparse;
}

bool RBM::getSparse()
{
	return this->sparse;
}

void RBM::writeSelf(string filename)
{
	
	ofstream fout(filename, ios::out);

	fout << visualUnit;
	fout << hiddenUnit;

	fout << nonLinearType;
	fout << sparse;

	fout << selfBatchSize;
	fout << selfTrainingTime;
	fout << sampleLength;

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		for (size_t j = 0; j < visualUnit; j++)
		{
			fout << weight->getValue(i, j) << ' ';
		}
	}

	for (size_t i = 0; i < hiddenUnit; i++)
	{
		fout << bias->getValue(i, 0) << ' ';
	}

	for (size_t i = 0; i < visualUnit; i++)
	{
		fout << biasH2V->getValue(i, 0) << ' ';
	}

	fout.close();

}

void RBM::readSelf(string filename)
{
	
	ifstream fin(filename, ios::in);

	fin >> visualUnit;
	fin >> hiddenUnit;

	fin >> nonLinearType;
	fin >> sparse;

	fin >> selfBatchSize;
	fin >> selfTrainingTime;
	fin >> sampleLength;

	NonLinearFactory * factory = new NonLinearFactory();
	nonlinearLayer = factory->createNonLinear(nonLinearType, hiddenUnit);

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

	bias.reset(new Matrix(hiddenUnit, 1));
	for (size_t i = 0; i < hiddenUnit; i++)
	{
		double d_value;
		fin >> d_value;
		bias->setValue(i, 0, d_value);
	}

	biasH2V.reset(new Matrix(visualUnit, 1));
	for (size_t i = 0; i < visualUnit; i++)
	{
		double d_value;
		fin >> d_value;
		biasH2V->setValue(i, 0, d_value);
	}

	weightGradient.reset(new Matrix(hiddenUnit, visualUnit));
	bias.reset(new Matrix(hiddenUnit, 1));
	biasH2VGradient.reset(new Matrix(visualUnit, 1));

	fin.close();

}
string RBM::getNetworkName(){
	return "RBM";
}