#include "layer/ConvolutionLayer.h"

ConvolutionLayer::ConvolutionLayer(size_t rowSize, size_t columnSize, size_t krowSize, size_t kcolumnSize, 
	size_t visualSize, size_t hiddenSize, size_t stride, size_t initScheme, double dropoutRate, shared_ptr<Solver> solver)
{
	this->visualRow = rowSize;
	this->visualColumn = columnSize;
	this->krowSize = krowSize;
	this->kcolumnSize = kcolumnSize;
	this->hiddenRow = rowSize - krowSize + 1;;
	this->hiddenColumn = columnSize - kcolumnSize + 1;
	this->visualSize = visualSize;
	this->hiddenSize = hiddenSize;
	this->stride = stride;
	this->solver = solver;
	this->initScheme = initScheme;
	this->dropoutRate = dropoutRate;
	this->instance = MatrixPool::getInstance();
	init(initScheme);
}

ConvolutionLayer::~ConvolutionLayer()
{

}

void ConvolutionLayer::init(int init_scheme)
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

	for (size_t h = 0; h < hiddenSize; h++)
	{
		vector<shared_ptr<Matrix>> temp;
		for (size_t v = 0; v < visualSize; v++)
		{
			shared_ptr<Matrix> k = instance->allocMatrixUnclean(krowSize, kcolumnSize);
			k->initializeRandom(lowerBound, upperBound);
			temp.push_back(k);
		}
		kernel.push_back(temp);
		bias.push_back(instance->allocMatrixUnclean(hiddenRow, hiddenColumn));
		bias[h]->initializeRandom(lowerBound, upperBound);
	}

	for (size_t h = 0; h < hiddenSize; h++)
	{
		vector<shared_ptr<Matrix>> temp;
		for (size_t v = 0; v < visualSize; v++)
		{
			temp.push_back(instance->allocMatrix(krowSize, kcolumnSize));
		}
		kernelGradient.push_back(temp);
		biasGradient.push_back(instance->allocMatrix(hiddenRow, hiddenColumn));
	}

	for (size_t h = 0; h < hiddenSize; h++)
	{
		vector<shared_ptr<Matrix>> temp;
		for (size_t v = 0; v < visualSize; v++)
		{
			temp.push_back(instance->allocMatrix(krowSize, kcolumnSize));
		}
		kernelMomentum.push_back(temp);
		biasMomentum.push_back(instance->allocMatrix(hiddenRow, hiddenColumn));
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
			visualGradient[j]->addi(hiddenGradient[i]->wideCorr(kernel[i][j], stride));
			kernelGradient[i][j]->addi(visualValue[j]->narrowRCorr(hiddenGradient[i], stride));
		}
		visualValue[j]->setValues(0);
	}
	for (size_t i = 0; i < hiddenSize; i++)
	{
		biasGradient[i]->addi(hiddenGradient[i]);
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
	this->solver->update(kernel, kernelMomentum, kernelGradient, bias, biasMomentum, biasGradient, this->hiddenSize, this->visualSize);
}

