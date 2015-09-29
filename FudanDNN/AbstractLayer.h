#pragma once
#include "stdafx.h"
#include "AbstractMatrix.h"
#include <fstream>

using namespace std;

class AbstractLayer
{
public:
	//训练前传
	virtual void compute() {};
	//预测前传
	virtual void calculate() {};
	//误差反传
	virtual void gradient() {};
	virtual string getNetworkName() { return nullptr; };
	//误差更新
	void setVisualValue(shared_ptr<AbstractMatrix> visualValue);
	//读入hidden层误差
	void setHiddenGradient(shared_ptr<AbstractMatrix> hiddenGradient);
	//获取visual层值
	shared_ptr<AbstractMatrix> getVisualValue();
	//获取hidden层值
	shared_ptr<AbstractMatrix> getHiddenValue();
	//获取visual层误差
	shared_ptr<AbstractMatrix> getVisualGradient();
	//获取hidden层误差
	shared_ptr<AbstractMatrix> getHiddenGradient();
	//获取visual层数量
	size_t getVisualUnit();
	//获取hidden层数量
	size_t getHiddenUnit();
	void setNetworkName(string networkName);
	//序列化输出
	virtual void writeSelf(string) {};
	virtual void readSelf(string) {};
	virtual ~AbstractLayer() {};

protected:
	shared_ptr<AbstractMatrix> visualValue;
	shared_ptr<AbstractMatrix> hiddenValue;
	shared_ptr<AbstractMatrix> visualGradient;
	shared_ptr<AbstractMatrix> hiddenGradient;
	size_t visualUnit;
	size_t hiddenUnit;
	string networkname;

};

