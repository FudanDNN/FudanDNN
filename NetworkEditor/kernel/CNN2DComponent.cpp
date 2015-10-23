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
	this->scheme = scheme;
	this->hiddenRow = visualRow - kernelSize + 1;
	this->hiddenColumn = visualColumn - kernelSize + 1;
}

CNN2DComponent::CNN2DComponent(){

}
size_t CNN2DComponent::calculateHiddenSize(){
	return hiddenRow*hiddenColumn*featureMapNum;
}
void CNN2DComponent::initializeVector(){

	for (size_t f = 0; f < featureMapNum; f++)
	{
		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t i = 0; i < num; i++)
			{
				shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, kernelSize));
				mv.push_back(m);
			}
			convKernels.push_back(mv);
		}

		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t i = 0; i < num; i++)
			{
				shared_ptr<AbstractMatrix> m = shared_ptr<AbstractMatrix>(new Matrix(kernelSize, kernelSize));
				m->setAllValue(0);
				mv.push_back(m);
			}
			convKernelsGradient.push_back(mv);
		}

		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t i = 0; i < num; i++)
			{
				shared_ptr<AbstractMatrix> m = shared_ptr<AbstractMatrix>(new Matrix(kernelSize, kernelSize));
				m->setAllValue(0);
				mv.push_back(m);
			}
			convKernelMomentum.push_back(mv);
		}

		for (size_t i = 0; i < num; i++)
		{
			shared_ptr<AbstractMatrix> m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
			bias.push_back(m);
		}

		for (size_t i = 0; i < num; i++)
		{
			shared_ptr<AbstractMatrix> m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
			m->setAllValue(0);
			biasGradient.push_back(m);
		}

		for (size_t i = 0; i < num; i++)
		{
			shared_ptr<AbstractMatrix> m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
			m->setAllValue(0);
			biasMomentum.push_back(m);
		}

	}
}
void CNN2DComponent::initialization()
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

	for (size_t f = 0; f < featureMapNum; f++)
	{

		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t i = 0; i < num; i++)
			{
				shared_ptr<AbstractMatrix> m(new Matrix(kernelSize, kernelSize));
				m->initializeValue(lowerBound, upperBound);
				mv.push_back(m);
			//	m->print();
			}
			convKernels.push_back(mv);
		}

		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t i = 0; i < num; i++)
			{
				shared_ptr<AbstractMatrix> m = shared_ptr<AbstractMatrix>(new Matrix(kernelSize, kernelSize));
				m->setAllValue(0);
				mv.push_back(m);
			}
			convKernelsGradient.push_back(mv);
		}

		{
			vector<shared_ptr<AbstractMatrix>> mv;
			for (size_t i = 0; i < num; i++)
			{
				shared_ptr<AbstractMatrix> m = shared_ptr<AbstractMatrix>(new Matrix(kernelSize, kernelSize));
				m->setAllValue(0);
				mv.push_back(m);
			}
			convKernelMomentum.push_back(mv);
		}

		for (size_t i = 0; i < num; i++)
		{
			shared_ptr<AbstractMatrix> m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
			m->initializeValue(lowerBound, upperBound);
			bias.push_back(m);
		}

		for (size_t i = 0; i < num; i++)
		{
			shared_ptr<AbstractMatrix> m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
			m->setAllValue(0);
			biasGradient.push_back(m);
		}

		for (size_t i = 0; i < num; i++)
		{
			shared_ptr<AbstractMatrix> m = shared_ptr<AbstractMatrix>(new Matrix(1, 1));
			m->setAllValue(0);
			biasMomentum.push_back(m);
		}
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
	/*cout << "visualValue" << num << endl;
	for (int i = 0; i < visualValue.size(); i++){
		visualValue[i]->print();
	}*/

	this->hiddenValue.clear();
	//cout << "convKernels" << num <<endl;
	for (size_t f = 0; f < featureMapNum; f++)
	{
		shared_ptr<AbstractMatrix> fm(new Matrix(visualRow - kernelSize + 1, visualColumn - kernelSize + 1));
		fm->setAllValue(0);
		for (size_t v = 0; v < visualValue.size(); v++)
		{
			for (size_t i = 0; i < visualRow - kernelSize + 1; i += stride)
			{
				for (size_t j = 0; j < visualColumn - kernelSize + 1; j += stride) {
					fm->add_inplace(i / stride, j / stride, 
						visualValue[v]->convolve(i, i + kernelSize, j, j + kernelSize, convKernels[f][v]));
				}
			}
			//convKernels[f][v]->print();
		}
		fm->add_inplace(bias[f]->getValue(0, 0));
		this->hiddenValue.push_back(fm);
		//fm->print();
	}
	/*cout << "bias" << num << endl;
	for (int i = 0; i < bias.size(); i++){
		bias[i]->print();
	}*/
	/*cout << "hiddenValue" << num << endl;
	for (int i = 0; i < hiddenValue.size(); i++){
		hiddenValue[i]->print();
	}*/
}

void CNN2DComponent::gradient(){
	/*cout << "hiddenGradient" <<num<< endl;
	for (int i = 0; i < hiddenGradient.size(); i++){
		hiddenGradient[i]->print();
	}*/
	//cout << "convGradient" << num << endl;
	for (size_t v = 0; v < num; v++)
	{
		visualGradient[v]->setAllValue(0);
		for (size_t f = 0; f < featureMapNum; f++)
		{
			for (size_t i = 0; i < visualRow - kernelSize + 1; i += stride)
			{
				for (size_t j = 0; j < visualColumn - kernelSize + 1; j += stride) {
					double gradient = hiddenGradient[f]->getValue(i / stride, j / stride);
					visualGradient[v]->add_inplace(i, i + kernelSize, j, j + kernelSize,
						convKernels[f][v], 0, kernelSize, 0, kernelSize, gradient);
					convKernelsGradient[f][v]->add_inplace(0, kernelSize, 0, kernelSize, 
						visualValue[v], i, i + kernelSize, j, j + kernelSize, gradient);
				}
			}
			//convKernelsGradient[f][v]->print();
		}
	}
	//cout << "biasGradient" << num << endl;
	//cout << "hiddenSum" << num <<endl;
	for (size_t f = 0; f < featureMapNum; f++)
	{
		//cout << hiddenGradient[f]->sum() << endl;
		biasGradient[f]->add_inplace(hiddenGradient[f]->sum());
		//biasGradient[f]->print();
	}
	/*cout << "visualGradient"<<this->num << endl;
	for (int i = 0; i < visualGradient.size(); i++){
		visualGradient[i]->print();
	}*/
	this->hiddenGradient.clear();
}

void CNN2DComponent::update() 
{
	//cout << "biasMomentum" << endl;
	for (int i = 0; i < featureMapNum; i++)
	{
		for (size_t j = 0; j < num; j++)
		{
			convKernelMomentum[i][j]->multiple_inplace(momentumRate)->add_inplace(convKernelsGradient[i][j]->multiple_inplace(kernelLearningRate));
			convKernels[i][j]->add_inplace(convKernelMomentum[i][j]);
			convKernelsGradient[i][j]->setAllValue(0);
		}
		biasMomentum[i]->multiple_inplace(momentumRate)->add_inplace(biasGradient[i]->multiple_inplace(biasLearningRate));
		//biasMomentum[i]->print();
		bias[i]->add_inplace(biasMomentum[i]);
		biasGradient[i]->setAllValue(0);
	}
}

void CNN2DComponent::regularize()
{
	for (int i = 0; i < featureMapNum; i++)
	{
		for (size_t j = 0; j < num; j++)
		{
			convKernels[i][j]->multiple_inplace(1 - regularizationRate);
		}
		bias[i]->multiple_inplace(1 - regularizationRate);
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

void CNN2DComponent::writeSelf(string filename){

	ofstream fout(filename);

	fout << kernelSize << endl;
	fout << stride << endl;
	fout << featureMapNum << endl;
	fout << num << endl;
	fout << visualRow << endl;
	fout << visualColumn << endl;;
	fout << hiddenRow << endl;
	fout << hiddenColumn << endl;

	for (size_t i = 0; i < featureMapNum; i++)
	{
		for (size_t j = 0; j < num; j++) 
		{
			for (size_t ii = 0; ii < kernelSize; ii++)
			{
				for (size_t jj = 0; jj < kernelSize; jj++)
				{
					fout << convKernels[i][j]->getValue(ii, jj) << endl;
				}
			}
		}
	}

	for (size_t i = 0; i < featureMapNum; i++)
	{
		fout << bias[i]->getValue(0,0) << endl;
	}

	fout.close();

}

void CNN2DComponent::readSelf(string filename){

	ifstream fin(filename);

	fin >> kernelSize;
	fin >> stride;
	fin >> featureMapNum;
	fin >> num;
	fin >> visualRow;
	fin >> visualColumn;
	fin >> hiddenRow;
	fin >> hiddenColumn;
	initializeVector();
	for (size_t i = 0; i < featureMapNum; i++)
	{
		for (size_t j = 0; j < num; j++)
		{
			for (size_t ii = 0; ii < kernelSize; ii++)
			{
				for (size_t jj = 0; jj < kernelSize; jj++)
				{
					double value;
					fin >> value;
					//cout << convKernels[i][j]->getRowSize() <<"-----"<< convKernels[i][j]->getColumnSize() << endl;
					convKernels[i][j]->setValue(ii, jj, value);
				}
			}
		}
	}

	for (size_t i = 0; i < featureMapNum; i++)
	{
		double value;
		fin >> value;
		bias[i]->setValue(0, 0, value);
	}

	fin.close();

}

double CNN2DComponent::getMomentumRate()
{
	return momentumRate;
}

void CNN2DComponent::setMomentumRate(double momentumRate)
{
	this->momentumRate = momentumRate;
}

size_t CNN2DComponent::getKernelSize()
{
	return kernelSize;
}

void CNN2DComponent::setKernelSize(size_t kernelSize)
{
	this->kernelSize = kernelSize;
}

size_t CNN2DComponent::getStride()
{ 
	return stride;
}

void CNN2DComponent::setStride(size_t stride)
{
	this->stride = stride;
}

size_t CNN2DComponent::getFeatureMapNum()
{
	return featureMapNum;
}

void CNN2DComponent::setFeatureMap(size_t featureMap)
{
	this->featureMapNum = featureMap;
}