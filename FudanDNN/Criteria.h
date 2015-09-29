#pragma once
#include "stdafx.h"
#include "AbstractMatrix.h"
#include <string>
#include <fstream>

using namespace std;

class Criteria
{
public:
	Criteria();
	//计算误差
	virtual double computeError() { return 0; };
	//误差反传
	virtual void gradient() {};
	//获取visual层数量
	size_t getVisualUnit();
	//设置预测值
	void setPredictValue(shared_ptr<AbstractMatrix>);
	//获取预测值
	shared_ptr<AbstractMatrix> getPredictValue();
	//设置期望值
	void setExpectedValue(shared_ptr<AbstractMatrix>);
	//获取期望值
	shared_ptr<AbstractMatrix> getExpectedValue();
	//获取visual层误差
	shared_ptr<AbstractMatrix> getPredictGradient();
	virtual string getType() { return nullptr; };
	//获取错误率
	double getErrorRate();
	void readSelf(string);
	void writeSelf(string);

protected:
	size_t visualUnit;
	shared_ptr<AbstractMatrix> predictValue;
	shared_ptr<AbstractMatrix> expectedValue;
	shared_ptr<AbstractMatrix> predictGradient;
	double errorRate = 1;
	string type;
};
