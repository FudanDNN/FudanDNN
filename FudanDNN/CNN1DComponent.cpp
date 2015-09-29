#include "CNN1DComponent.h"

CNN1DComponent::CNN1DComponent(size_t kernelSize, size_t stride,
	size_t featureMapNum, size_t num, size_t visualRow, size_t scheme)
{
	this->kernelSize = kernelSize;
	this->stride = stride;
	this->featureMapNum = featureMapNum;
	this->visualRow = visualRow;

	initialization(scheme);
}

CNN1DComponent::~CNN1DComponent()
{
}

void CNN1DComponent::initialization(size_t scheme)
{

	for (size_t f = 0; f < featureMapNum; f++)
	{
		double lowerBound = 0;
		double upperBound = 0;

		size_t visualUnit = visualRow;

		switch (scheme)
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

		shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, 1));
		m->initializeValue(lowerBound, upperBound);
		convKernels.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(kernelSize, 1));
		m->initializeValue(0, 0);
		convKernelsGradient.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
		m->initializeValue(lowerBound, upperBound);
		bias.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
		m->initializeValue(0, 0);
		biasGradient.push_back(m);

	}

	for (size_t v = 0; v < num; v++)
	{
		shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, 1));
		m->initializeValue(0, 0);
		visualValue.push_back(m);
	}

}

void CNN1DComponent::calculate()
{
	compute();
}

void CNN1DComponent::compute() {

	hiddenValue.clear();
	for (size_t f = 0; f < featureMapNum; f++)
	{
		for (size_t v = 0; v < num; v++)
		{
			shared_ptr<AbstractMatrix> fm(new Matrix(kernelSize, kernelSize));
			for (size_t i = 0; i < visualRow; i += stride)
			{
				fm->add(i / stride, 0,
					visualValue[v]->convolve(i, i + kernelSize - 1, 0, 0, convKernels[f]));
			}
			fm->add(bias[f]);
			hiddenValue.push_back(fm);
		}
	}
}

void CNN1DComponent::gradient(){

	for (size_t v = 0; v < num; v++)
	{
		for (size_t f = 0; f < featureMapNum; f++)
		{
			for (size_t i = 0; i < visualRow; i += stride)
			{
				double gradient = hiddenGradient[f]->getValue(i / stride, num);
				visualGradient[v]->add(i, i + kernelSize - 1, 0, 0,
					convKernels[f]->multiple(gradient));
				convKernelsGradient[f]->add(visualValue[v]->submatrix(i, i + kernelSize - 1, 0, 0)->multiple(gradient));
			}
			biasGradient[f]->add(hiddenGradient[f]);
		}
	}

}

void CNN1DComponent::update() 
{
	for (int i = 0; i < featureMapNum; i++)
	{
		convKernels[i] = convKernels[i]->add(convKernelsGradient[i]->multiple(kernelLearningRate));
		bias[i] = bias[i]->add(biasGradient[i]->multiple(biasLearningRate));
	}
}

void CNN1DComponent::regularize()
{
	for (int i = 0; i < featureMapNum; i++)
	{
		convKernels[i] = convKernels[i]->multiple(regularizationRate);
		bias[i] = bias[i]->multiple(regularizationRate);
	}
}

double CNN1DComponent::getRegularizationRate()
{
	return regularizationRate;
}

void CNN1DComponent::setRegularizationRate(double regularizationRate)
{
	this->regularizationRate = regularizationRate;
}

double CNN1DComponent::getKernelLearningRate()
{
	return kernelLearningRate;
}

void CNN1DComponent::setKernelLearningRate(double kernelLearningRate)
{
	this->kernelLearningRate = kernelLearningRate;
}

double CNN1DComponent::getBiasLearningRate()
{
	return biasLearningRate;
}

void CNN1DComponent::setBiasLearningRate(double biasLearningRate)
{
	this->biasLearningRate = biasLearningRate;
}
