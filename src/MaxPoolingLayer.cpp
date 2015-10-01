#include "MaxPoolingLayer.h"

MaxPoolingLayer::MaxPoolingLayer(size_t poolingSize, size_t stride, size_t visualRow, size_t visualColumn)
{
	this->poolingSize = poolingSize;
	this->stride = stride;
	this->visualRow = visualRow;
	this->visualColumn = visualColumn;
	this->hiddenRow = visualRow / stride;
	this->hiddenColumn = visualColumn / stride;
}

void MaxPoolingLayer::calculate()
{
	compute();
}

void MaxPoolingLayer::compute()
{
	hiddenValue.clear();
	maxPostionI.clear();
	maxPostionJ.clear();
	for (size_t index = 0; index < visualValue.size(); index++)
	{
		shared_ptr<AbstractMatrix> result(new Matrix(hiddenRow, hiddenColumn));
		shared_ptr<AbstractMatrix> maxPosI(new Matrix(hiddenRow, hiddenColumn));
		shared_ptr<AbstractMatrix> maxPosJ(new Matrix(hiddenRow, hiddenColumn));
		for (size_t i = 0; i < visualRow; i += stride)
		{
			for (size_t j = 0; j < visualColumn; j += stride)
			{
				shared_ptr<MaxData> poolingResult = visualValue[index]->max(i, i + poolingSize, j, j + poolingSize);
				result->setValue(i / stride, j / stride, poolingResult->value);
				maxPosI->setValue(i / stride, j / stride, poolingResult->pos_i);
				maxPosJ->setValue(i / stride, j / stride, poolingResult->pos_j);
			}
		}
		hiddenValue.push_back(result);
		maxPostionI.push_back(maxPosI);
		maxPostionJ.push_back(maxPosJ);
	}
}

void MaxPoolingLayer::gradient()
{

	for (size_t index = 0; index < visualValue.size(); index++)
	{
		visualGradient[index]->initializeValue(0, 0);
		for (size_t i = 0; i < visualRow; i += stride)
		{
			for (size_t j = 0; j < visualColumn; j += stride)
			{
				size_t maxPosI = maxPostionI[index]->getValue(i / stride, j / stride);
				size_t maxPosJ = maxPostionJ[index]->getValue(i / stride, j / stride);
				visualGradient[index]->add(maxPosI, maxPosJ, hiddenGradient[index]->getValue(i / stride, j / stride));
			}
		}
	}

}