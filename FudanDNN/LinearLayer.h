#pragma once
#include "Matrix.h"
#include "AbstractNetworkLayer.h"

class LinearLayer : public AbstractNetworkLayer
{
public:
	
	//构造函数
	LinearLayer(size_t visualUnit, size_t hiddenUnit, size_t initScheme);
	LinearLayer() {};
	string getNetworkName() override;
	//训练前传
	void compute() override;
	//预测前传
	void calculate()override;
	//误差反传
	void gradient()override;

	//参数写入
	void writeSelf(string) override;
	void readSelf(string) override;
	
};