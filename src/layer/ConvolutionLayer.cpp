#include "layer/ConvolutionLayer.h"

ConvolutionLayer::ConvolutionLayer(size_t rowSize, size_t columnSize, size_t krowSize, size_t kcolumnSize, 
	size_t visualSize, size_t hiddenSize, size_t stride, size_t initScheme, shared_ptr<Solver> solver)
{
	this->visualRow = rowSize;
	this->visualColumn = columnSize;
	this->krowSize = krowSize;
	this->kcolumnSize = columnSize;
	this->hiddenRow = rowSize - krowSize + 1;;
	this->hiddenColumn = columnSize - kcolumnSize + 1;
	this->visualSize = visualSize;
	this->hiddenSize = hiddenSize;
	this->stride = stride;
	this->solver = solver;
	this->initScheme = initScheme;
	this->initialization(initScheme);
}

ConvolutionLayer::~ConvolutionLayer()
{

}

void ConvolutionLayer::init()
{
	this->initialization(initScheme);
}

void ConvolutionLayer::initialization(int init_scheme)
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

	for (size_t h = 0; h < hiddenSize; h++)
	{
		kernel[h] = *(new vector<shared_ptr<Matrix>>());
		for (size_t v = 0; v < hiddenSize; v++)
		{
			shared_ptr<Matrix> k(new Matrix(krowSize, kcolumnSize));
			k->initializeRandom(lowerBound, upperBound);
			kernel[h].push_back(k);
		}
		bias.push_back(shared_ptr<Matrix>(new Matrix(hiddenRow, hiddenColumn)));
		bias[h]->initializeRandom(lowerBound, upperBound);
	}

	for (size_t v = 0; v < visualSize; v++)
	{
		visualGradient.push_back(shared_ptr<Matrix>(new Matrix(visualRow, visualColumn)));
	}

	for (size_t h = 0; h < hiddenSize; h++)
	{
		hiddenGradient.push_back(shared_ptr<Matrix>(new Matrix(hiddenRow, hiddenColumn)));
	}

	initGradient();
	initialized = true;

}

void ConvolutionLayer::initGradient()
{
	for (size_t h = 0; h < hiddenSize; h++)
	{
		kernelGradient[h] = *(new vector<shared_ptr<Matrix>>());
		kernelMomentum[h] = *(new vector<shared_ptr<Matrix>>());
		for (size_t v = 0; v < hiddenSize; v++)
		{
			shared_ptr<Matrix> k(new Matrix(krowSize, kcolumnSize));
			k->setValues(0);
			kernelGradient[h].push_back(k);
		}
		biasGradient.push_back(shared_ptr<Matrix>(new Matrix(hiddenRow, hiddenColumn)));
		bias[h]->setValues(0);
	}

	for (size_t h = 0; h < hiddenSize; h++)
	{
		kernelMomentum[h] = *(new vector<shared_ptr<Matrix>>());
		for (size_t v = 0; v < hiddenSize; v++)
		{
			shared_ptr<Matrix> k(new Matrix(krowSize, kcolumnSize));
			k->setValues(0);
			kernelMomentum[h].push_back(k);
		}
		biasMomentum.push_back(shared_ptr<Matrix>(new Matrix(hiddenRow, hiddenColumn)));
		bias[h]->setValues(0);
	}

}

void ConvolutionLayer::compute()
{
	for (int i = 0; i < hiddenSize; i++)
	{
		hiddenValue[i]->setValues(0);
		for (size_t j = 0; j < visualSize; j++)
		{
			hiddenValue[i]->addi(visualValue[j]->narrowConv(kernel[i][j], stride));
		}
		hiddenGradient[i]->setValues(0);
	}
}

void ConvolutionLayer::calculate()
{
	for (size_t i = 0; i < hiddenSize; i++)
	{
		hiddenValue[i]->setValues(0);
		for (size_t j = 0; j < visualSize; j++)
		{
			hiddenValue[i]->addi(visualValue[j]->narrowConv(kernel[i][j], stride));
		}
		hiddenGradient[i]->setValues(0);
	}
}

void ConvolutionLayer::gradient()
{
	for (size_t j = 0; j < visualSize; j++)
	{
		visualGradient[j]->setValues(0);
		for (size_t i = 0; i < hiddenSize; i++)
		{
		}
	}
}

void ConvolutionLayer::writeSelf(string filename)
{

}

void ConvolutionLayer::readSelf(string filename)
{

}

string ConvolutionLayer::getNetworkName(){
	return "ConvolutionLayer";
}

void ConvolutionLayer::update(){
	this->solver->update(kernel, kernelMomentum, kernelGradient, bias, biasMomentum, biasGradient, this->hiddenSize);
}

