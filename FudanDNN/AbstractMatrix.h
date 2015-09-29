#pragma once
#include "stdafx.h"
#include <vector>

using namespace std;

struct MaxData
{
	double value;
	size_t pos_i;
	size_t pos_j;
};

class AbstractMatrix : public enable_shared_from_this<AbstractMatrix>
{
public:
	void setValue(size_t i, size_t j, double value);
	double getValue(size_t i, size_t j);
	void setRow(size_t i, vector<double> row);
	vector<double> getRow(size_t i);
	void setColumn(size_t j, vector<double> row);
	vector<double> getColumn(size_t j);
	size_t getRowSize();
	size_t getColumnSize();
	//判断i,j是否在矩阵内
	bool bounded(int i, int j);
	//初始化矩阵，使得矩阵的初始化值上下界在[lowerBound, upperBound)之间
	virtual void initializeValue(double lowerBound, double upperBound) {};
	//随机01矩阵，取1概率为p
	virtual void initializeBinaryValue(double p) {};
	//元素和
	virtual double sum() { return 0; };
	//元素最大值
	virtual double max() { return 0; }
	//子矩阵元素最大值
	virtual shared_ptr<MaxData> max(int top, int bottom, int left, int right) { return nullptr; };
	//元素最小值
	virtual double min() { return 0; }
	//1-范数，矩阵元素绝对值和
	virtual double norm1() { return 0; };
	//2-范数，矩阵元素平方和
	virtual double norm2() { return 0; };
	//矩阵的迹，即矩阵对角线元素和
	virtual double trace() { return 0; };
	//矩阵转置
	virtual shared_ptr<AbstractMatrix> transpose() { return shared_ptr<AbstractMatrix>(nullptr); };
	//子矩阵，越界报错
	virtual shared_ptr<AbstractMatrix> submatrix(int top, int bottom, int left, int right) { return nullptr; };
	//子矩阵,越界截取
	virtual shared_ptr<AbstractMatrix> submatrixShrink(int top, int bottom, int left, int right) { return nullptr; };
	//子矩阵，越界填补	
	virtual shared_ptr<AbstractMatrix> submatrixExpand(int top, int bottom, int left, int right) { return nullptr; };
	//矩阵按行分裂,[0, row_index), [row_index, row_size)
	virtual vector<shared_ptr<AbstractMatrix>> splitRow(size_t rowIndex) { return *new vector<shared_ptr<AbstractMatrix>>(); };
	//矩阵按列分裂,[0, collumn_index), [collumn_index, collumn_size)
	virtual vector<shared_ptr<AbstractMatrix>> splitColumn(size_t columnIndex) { return *new vector<shared_ptr<AbstractMatrix>>(); };
	//合并子矩阵获得新矩阵，默认当前矩阵为左上矩阵
	virtual shared_ptr<AbstractMatrix> merge(shared_ptr<AbstractMatrix> rightTop, shared_ptr<AbstractMatrix> leftBottom, 
		shared_ptr<AbstractMatrix> rightBottom) { return nullptr; };
	//按行合并子矩阵获得新矩阵，默认当前矩阵为上矩阵
	virtual shared_ptr<AbstractMatrix> mergeRow(shared_ptr<AbstractMatrix> bottom) { return nullptr; };
	//按列合并子矩阵获得新矩阵，默认当前矩阵为左矩阵
	virtual shared_ptr<AbstractMatrix> mergeColumn(shared_ptr<AbstractMatrix> right) { return nullptr; };
	//矩阵所有值加d
	virtual shared_ptr<AbstractMatrix> add(double x) { return nullptr; };
	//矩阵相加
	virtual shared_ptr<AbstractMatrix> add(shared_ptr<AbstractMatrix> m) { return nullptr; };
	//位置(i,j)加value
	virtual shared_ptr<AbstractMatrix> add(int i, int j, double value) { return nullptr; };
	//加子矩阵
	virtual shared_ptr<AbstractMatrix> add(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) { return nullptr; };
	//矩阵所有值减d
	virtual shared_ptr<AbstractMatrix> subtract(double x) { return nullptr; };
	//矩阵相减
	virtual shared_ptr<AbstractMatrix> subtract(shared_ptr<AbstractMatrix> m) { return nullptr; };
	//矩阵所有值乘d
	virtual shared_ptr<AbstractMatrix> multiple(double x) { return nullptr; };
	//矩阵所有值相同位置相乘
	virtual shared_ptr<AbstractMatrix> multiple(shared_ptr<AbstractMatrix> m) { return nullptr; };
	//输入矩阵左乘当前矩阵，即m*this
	virtual shared_ptr<AbstractMatrix> multipleLeft(shared_ptr<AbstractMatrix> m) { return nullptr; };
	//输入矩阵右乘当前矩阵，即this*m
	virtual shared_ptr<AbstractMatrix> multipleRight(shared_ptr<AbstractMatrix> m) { return nullptr; };
	//输入一个函数，所有元素执行这个函数 
	virtual shared_ptr<AbstractMatrix> map(double func(double)) { return nullptr; };
	//将矩阵的子矩阵与输入矩阵做卷积
	virtual double convolve(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) { return 0; }
	virtual shared_ptr<AbstractMatrix> m2vByColumn() { return nullptr; }
	virtual shared_ptr<AbstractMatrix> v2mByColomn(size_t size) { return nullptr; }
	//矩阵相等
	virtual bool equals(shared_ptr<AbstractMatrix> m) { return false; };
	virtual shared_ptr<AbstractMatrix> clone() { return nullptr; };
	virtual void print() {};
	virtual ~AbstractMatrix() {};
    virtual shared_ptr<AbstractMatrix> getSharedPtr(){
        return shared_from_this();
    }

	virtual shared_ptr<AbstractMatrix> add_inplace(double x) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> add_inplace(shared_ptr<AbstractMatrix> m) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> add_inplace(int, int, double) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> add_inplace(size_t, size_t, size_t, size_t, shared_ptr<AbstractMatrix>) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> subtract_inplace(double) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> subtract_inplace(shared_ptr<AbstractMatrix>) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> multiple_inplace(double) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> multiple_inplace(shared_ptr<AbstractMatrix>) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> map_inplace(double func(double)) { return shared_from_this(); };

protected:
	size_t rowSize;
	size_t columnSize;
	double **matrix;

	virtual void initialize() {};

};

