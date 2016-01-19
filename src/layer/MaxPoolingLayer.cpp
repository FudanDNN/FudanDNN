#include "layer/MaxPoolingLayer.h"

MaxPoolingLayer::MaxPoolingLayer(size_t rowSize, size_t columnSize, size_t krowSize, size_t kcolumnSize,
	size_t visualSize, size_t hiddenSize, size_t stride, double dropoutRate)
{
	this->visualRow = rowSize;
	this->visualColumn = columnSize;
	this->krowSize = krowSize;
	this->kcolumnSize = kcolumnSize;
	this->hiddenRow = (rowSize - krowSize) / stride + 1;;
	this->hiddenColumn = (columnSize - kcolumnSize) / stride + 1;
	this->visualSize = visualSize;
	this->hiddenSize = hiddenSize;
	this->stride = stride;
	this->dropoutRate = dropoutRate;
	this->instance = MatrixPool::getInstance();
	this->init();
}

MaxPoolingLayer::~MaxPoolingLayer()
{
}

void MaxPoolingLayer::init()
{
	Layer::init();
}

void MaxPoolingLayer::compute()
{
	for (size_t i = 0; i < hiddenSize; i++)
	{
		hiddenValue[i] = visualValue[i]->maxSubSampling(krowSize, kcolumnSize, stride);
	}
}

void MaxPoolingLayer::calculate()
{
	for (size_t i = 0; i < hiddenSize; i++)
	{
		hiddenValue[i] = visualValue[i]->maxSubSampling(krowSize, kcolumnSize, stride);
	}
}

void MaxPoolingLayer::gradient()
{
	for (size_t i = 0; i < visualSize; i++)
	{
		visualGradient[i] = visualValue[i]->maxUpSampling(krowSize, kcolumnSize, stride, hiddenGradient[i]);
	}
}

void MaxPoolingLayer::writeSelf(string filename)
{

}

void MaxPoolingLayer::readSelf(string filename)
{

}

string MaxPoolingLayer::getNetworkName()
{
	return "MaxPoolingLayer";
}


