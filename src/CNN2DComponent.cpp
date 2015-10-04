#include "CNN2DComponent.h"

CNN2DComponent::CNN2DComponent(size_t kernelSize, size_t stride,
		size_t featureMapNum, size_t num, size_t visualRow, size_t visualColumn, size_t scheme)
{
	this->kernelSize = kernelSize;
	this->stride = stride;
	this->featureMapNum = featureMapNum;
	this->num = num;
	this->visualRow = visualRow;
	this->visualColumn = visualColumn;
	
	initialization(scheme);
}

void CNN2DComponent::initialization(size_t scheme)
{

	for (size_t f = 0; f < featureMapNum; f++)
	{
		double lowerBound = 0;
		double upperBound = 0;

		size_t visualUnit = visualRow * visualColumn;

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

		shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, kernelSize));
		m->initializeValue(lowerBound, upperBound);
		convKernels.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(kernelSize, kernelSize));
		m->initializeValue(0, 0);
		convKernelsGradient.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(kernelSize, kernelSize));
		m->initializeValue(0, 0);
		convKernelMomentum.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(kernelSize, 1));
		m->initializeValue(lowerBound, upperBound);
		bias.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(kernelSize, 1));
		m->initializeValue(0, 0);
		biasGradient.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(kernelSize, 1));
		m->initializeValue(0, 0);
		biasMomentum.push_back(m);

	}

}

CNN2DComponent::~CNN2DComponent()
{
}

void CNN2DComponent::calculate() 
{
	compute();
}

void CNN2DComponent::compute() {
	this->hiddenValue.clear();
	for (size_t f = 0; f < featureMapNum; f++)
	{
		shared_ptr<AbstractMatrix> fm(new Matrix(visualRow - kernelSize + 1, visualColumn - kernelSize + 1));
		fm->initializeValue(0, 0);
		for (size_t v = 0; v < num; v++)
		{
			for (size_t i = 0; i < visualRow - kernelSize + 1; i += stride)
			{
				for (size_t j = 0; j < visualColumn - kernelSize + 1; j += stride) {
					fm->add_inplace(i / stride, j / stride, 
						visualValue[v]->convolve(i, i + kernelSize, j, j + kernelSize, convKernels[f]));
				}
			}
		}
		fm->add_inplace(bias[f]->getValue(0, 0));
		this->hiddenValue.push_back(fm);
	}
	/*for (int i = 0; i < hiddenValue.size(); i++){
		hiddenValue[i]->print();
	}*/
}

void CNN2DComponent::gradient(){

	for (size_t v = 0; v < num; v++)
	{
		visualGradient[v]->initializeValue(0, 0);
		for (size_t f = 0; f < featureMapNum; f++)
		{
			for (size_t i = 0; i < visualRow - kernelSize + 1; i += stride)
			{
				for (size_t j = 0; j < visualColumn - kernelSize + 1; j += stride) {
					double gradient = hiddenGradient[f]->getValue(i / stride, j / stride);
					visualGradient[v]->add_inplace(i, i + kernelSize, j, j + kernelSize,
						convKernels[f]->multiple(gradient));
					convKernelsGradient[f]->add_inplace(visualValue[v]->submatrix(i, i + kernelSize, j, j + kernelSize)->multiple_inplace(gradient));
				}
			}
		}
	}

	for (size_t f = 0; f < featureMapNum; f++)
	{
		biasGradient[f]->add_inplace(hiddenGradient[f]->sum());
	}

}

void CNN2DComponent::update() 
{
	for (int i = 0; i < featureMapNum; i++)
	{
		convKernelMomentum[i]->multiple_inplace(momentumRate)->add_inplace(convKernelsGradient[i]->multiple_inplace(1 - momentumRate));
		convKernels[i]->add_inplace(convKernelMomentum[i]->multiple(kernelLearningRate));
		convKernelsGradient[i]->initializeValue(0, 0);
		biasMomentum[i]->multiple_inplace(momentumRate)->add_inplace(biasGradient[i]->multiple_inplace(1 - momentumRate));
		bias[i]->add_inplace(biasMomentum[i]->multiple(biasLearningRate));
		biasGradient[i]->initializeValue(0, 0);
	}
}

void CNN2DComponent::regularize()
{
	for (int i = 0; i < featureMapNum; i++)
	{
		convKernels[i] = convKernels[i]->multiple(regularizationRate);
		bias[i] = bias[i]->multiple(regularizationRate);
	}
}

double CNN2DComponent::getRegularizationRate()
{
	return regularizationRate;
}

void CNN2DComponent::setRegularizationRate(double regularizationRate)
{
	this->regularizationRate = regularizationRate;
}

double CNN2DComponent::getKernelLearningRate()
{
	return kernelLearningRate;
}

void CNN2DComponent::setKernelLearningRate(double kernelLearningRate)
{
	this->kernelLearningRate = kernelLearningRate;
}

double CNN2DComponent::getBiasLearningRate()
{
	return biasLearningRate;
}

void CNN2DComponent::setBiasLearningRate(double biasLearningRate)
{
	this->biasLearningRate = biasLearningRate;
}

