#pragma once
#include "Matrix.h"
#include "AbstractComponent.h"
#include <vector>

using namespace std;

class MaxPoolingLayer : public AbstractComponent {

protected:
	vector<shared_ptr<AbstractMatrix>> maxPostionI;//记录Max-Pooling时的最大元素i位置
	vector<shared_ptr<AbstractMatrix>> maxPostionJ;
	size_t stride;//pooling窗口步长
	size_t poolingSize;//pooling窗口大小

public:
	MaxPoolingLayer(size_t poolingSize, size_t stride, size_t visualRow, size_t visualColumn);
	void compute() override;
	void calculate() override;
	void gradient() override;

};