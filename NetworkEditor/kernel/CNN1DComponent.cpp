#include "CNN1DComponent.h"

CNN1DComponent::CNN1DComponent(size_t kernelSize, size_t stride,
	size_t featureMapNum, size_t num, size_t visualRow, size_t scheme)
{
	this->kernelSize = kernelSize;
	this->stride = stride;
	this->featureMapNum = featureMapNum;
	this->visualRow = visualRow;
	this->scheme = scheme;
	this->hiddenRow = visualRow - kernelSize + 1;
}
size_t CNN1DComponent::calculateHiddenSize(){
	return visualRow*featureMapNum;
}
CNN1DComponent::CNN1DComponent()
{

}
CNN1DComponent::~CNN1DComponent()
{
}

void CNN1DComponent::initializeVector()
{

	for (size_t f = 0; f < featureMapNum; f++)
	{
		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t v = 0; v < num; v++)
			{
				shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, 1));
				mv.push_back(m);
			}
			convKernels.push_back(mv);
		}

		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t v = 0; v < num; v++)
			{
				shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, 1));
				m->setAllValue(0);
				mv.push_back(m);
			}
			convKernelsGradient.push_back(mv);
		}

		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t v = 0; v < num; v++)
			{
				shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, 1));
				m->setAllValue(0);
				mv.push_back(m);
			}
			convKernelsGradient.push_back(mv);
		}

		shared_ptr<AbstractMatrix> m(new Matrix(1, 1));
		bias.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
		m->setAllValue(0);
		biasGradient.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
		m->setAllValue(0);
		biasMomentum.push_back(m);

	}

}

void CNN1DComponent::initialization()
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

	for (size_t f = 0; f < featureMapNum; f++)
	{

		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t v = 0; v < num; v++)
			{
				shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, 1));
				m->initializeValue(lowerBound, upperBound);
				mv.push_back(m);
			}
			convKernels.push_back(mv);
		}

		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t v = 0; v < num; v++)
			{
				shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, 1));
				m->setAllValue(0);
				mv.push_back(m);
			}
			convKernelsGradient.push_back(mv);
		}

		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t v = 0; v < num; v++)
			{
				shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, 1));
				m->setAllValue(0);
				mv.push_back(m);
			}
			convKernelsGradient.push_back(mv);
		}

		shared_ptr<AbstractMatrix> m(new Matrix(1, 1));
		m->initializeValue(lowerBound, upperBound);
		bias.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
		m->setAllValue(0);
		biasGradient.push_back(m);

		m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
		m->setAllValue(0);
		biasMomentum.push_back(m);

	}

	for (size_t v = 0; v < num; v++)
	{
		shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, 1));
		m->setAllValue(0);
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
		shared_ptr<AbstractMatrix> fm(new Matrix(kernelSize, kernelSize));
		for (size_t v = 0; v < num; v++)
		{
			for (size_t i = 0; i < visualRow; i += stride)
			{
				fm->add_inplace(i / stride, 0,
					visualValue[v]->convolve(i, i + kernelSize - 1, 0, 1, convKernels[f][v]));
			}
		}
		fm->add_inplace(bias[f]->getValue(0, 0));
		hiddenValue.push_back(fm);
	}
}

void CNN1DComponent::gradient(){

	for (size_t v = 0; v < num; v++)
	{
		visualGradient[v]->setAllValue(0);
		for (size_t f = 0; f < featureMapNum; f++)
		{
			for (size_t i = 0; i < visualRow; i += stride)
			{
				double gradient = hiddenGradient[f]->getValue(i / stride, num);
				visualGradient[v]->add_inplace(i, i + kernelSize - 1, 0, 0,
					convKernels[f][v]->multiple(gradient));
				convKernelsGradient[f][v]->add_inplace(visualValue[v]->submatrix(i, i + kernelSize - 1, 0, 0)->multiple(gradient));
			}
		}
	}

	for (size_t f = 0; f < featureMapNum; f++)
	{
		biasGradient[f]->add_inplace(hiddenGradient[f]->sum());
	}

}

void CNN1DComponent::update() 
{
	for (int i = 0; i < featureMapNum; i++)
	{
		for (size_t v = 0; v < num; v++)
		{
			convKernelMomentum[i][v]->multiple_inplace(momentumRate)->add_inplace(convKernelsGradient[i][v]->multiple_inplace(kernelLearningRate));
			convKernels[i][v]->add_inplace(convKernelMomentum[i][v]);
			convKernelsGradient[i][v]->setAllValue(0);
		}
		biasMomentum[i]->multiple_inplace(momentumRate)->add_inplace(biasGradient[i]->multiple_inplace(biasLearningRate));
		bias[i]->add_inplace(biasMomentum[i]);
		biasGradient[i]->setAllValue(0);
	}
}

void CNN1DComponent::regularize()
{
	for (int i = 0; i < featureMapNum; i++)
	{
		for (size_t v = 0; v < num; v++)
		{
			convKernels[i][v]->multiple_inplace(1 - regularizationRate);
		}
		bias[i]->multiple_inplace(1 - regularizationRate);
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
void CNN1DComponent::writeSelf(string filename){

	ofstream fout(filename);

	fout << kernelSize << endl;
	fout << stride << endl;
	fout << featureMapNum << endl;
	fout << num << endl;
	fout << visualRow << endl;
	fout << visualColumn << endl;

	for (size_t i = 0; i < featureMapNum; i++)
	{
		for (size_t j = 0; j < num; j++)
		{
			for (size_t ii = 0; ii < kernelSize; ii++)
			{
				fout << convKernels[i][j]->getValue(ii, 1) << endl;
			}
		}
	}

	for (size_t i = 0; i < featureMapNum; i++)
	{
		cout << bias[i]->getValue(1, 1) << endl;
	}

	fout.close();

}

void CNN1DComponent::readSelf(string filename){

	ifstream fin(filename);

	fin >> kernelSize;
	fin >> stride;
	fin >> featureMapNum;
	fin >> num;
	fin >> visualRow;
	fin >> visualColumn;

	initializeVector();

	for (size_t i = 0; i < featureMapNum; i++)
	{
		for (size_t j = 0; j < num; j++)
		{
			for (size_t ii = 0; ii < kernelSize; ii++)
			{
				double val;
				fin >> val;
				convKernels[i][j]->setValue(ii, 1, val);
			}
		}
	}

	for (size_t i = 0; i < featureMapNum; i++)
	{
		double val;
		fin >> val;
		bias[i]->setValue(1, 1, val);
	}

	fin.close();

}

double CNN1DComponent::getMomentumRate()
{
	return momentumRate;
}

void CNN1DComponent::setMomentumRate(double momentumRate)
{
	this->momentumRate = momentumRate;
}

size_t CNN1DComponent::getKernelSize()
{
	return kernelSize;
}

void CNN1DComponent::setKernelSize(size_t kernelSize)
{
	this->kernelSize = kernelSize;
}

size_t CNN1DComponent::getStride()
{
	return stride;
}

void CNN1DComponent::setStride(size_t stride)
{
	this->stride = stride;
}

size_t CNN1DComponent::getFeatureMapNum()
{
	return featureMapNum;
}

void CNN1DComponent::setFeatureMap(size_t featureMap)
{
	this->featureMapNum = featureMap;
}
