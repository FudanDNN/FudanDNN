#pragma once
#include "AbstractMatrix.h"

using namespace std;

class Matrix : public AbstractMatrix
{

public:
	Matrix(size_t rowSize, size_t columnSize);
	Matrix();	
    ~Matrix() override;
	//初始化矩阵，使得矩阵的初始化值上下界在[lowerBound, upperBound)之间
	void initializeValue(double lowerBound, double upperBound) override;
	void initializeBinaryValue(double p) override;
	//元素和
	double sum() override;
	//元素最大值
	double max() override;
	//子矩阵元素最大值
	shared_ptr<MaxData> max(int top, int bottom, int left, int right) override;
	//元素最小值
	double min() override;
	//1-范数，矩阵元素绝对值和
	double norm1() override;
	//2-范数，矩阵元素平方和
	double norm2() override;
	//矩阵的迹，即矩阵对角线元素和
	double trace() override;
	//矩阵转置
	shared_ptr<AbstractMatrix> transpose() override;
	//子矩阵，越界报错
	shared_ptr<AbstractMatrix> submatrix(int top, int bottom, int left, int right) override;
	//子矩阵,越界截取
	shared_ptr<AbstractMatrix> submatrixShrink(int top, int bottom, int left, int right)override;
	//子矩阵，越界填补	
	shared_ptr<AbstractMatrix> submatrixExpand(int top, int bottom, int left, int right)override;
	//矩阵按行分裂,[0, row_index), [row_index, row_size)
	vector<shared_ptr<AbstractMatrix>> splitRow(size_t rowIndex) override;
	//矩阵按列分裂,[0, collumn_index), [collumn_index, collumn_size)
	vector<shared_ptr<AbstractMatrix>> splitColumn(size_t columnIndex) override;
	//合并子矩阵获得新矩阵，默认当前矩阵为左上矩阵
	shared_ptr<AbstractMatrix> merge(shared_ptr<AbstractMatrix> rightTop, shared_ptr<AbstractMatrix> leftBottom, 
		shared_ptr<AbstractMatrix> rightBottom) override;
	//按行合并子矩阵获得新矩阵，默认当前矩阵为上矩阵
	shared_ptr<AbstractMatrix> mergeRow(shared_ptr<AbstractMatrix> bottom) override;
	//按列合并子矩阵获得新矩阵，默认当前矩阵为左矩阵
	shared_ptr<AbstractMatrix> mergeColumn(shared_ptr<AbstractMatrix> right) override;
	//矩阵所有值加d
	shared_ptr<AbstractMatrix> add(double x) override;
	//矩阵相加
	shared_ptr<AbstractMatrix> add(shared_ptr<AbstractMatrix> m) override;
	//位置(i,j)加value
	shared_ptr<AbstractMatrix> add(int i, int j, double value) override;
	//加子矩阵
	shared_ptr<AbstractMatrix> add(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) override;
	//矩阵所有值减d
	shared_ptr<AbstractMatrix> subtract(double x) override;
	//矩阵相减
	shared_ptr<AbstractMatrix> subtract(shared_ptr<AbstractMatrix> m) override;
	//矩阵所有值乘d
	shared_ptr<AbstractMatrix> multiple(double x) override;
	//矩阵所有值相同位置相乘
	shared_ptr<AbstractMatrix> multiple(shared_ptr<AbstractMatrix> m) override;
	//输入矩阵左乘当前矩阵，即mnullptr
	shared_ptr<AbstractMatrix> multipleLeft(shared_ptr<AbstractMatrix> m) override;
	//输入矩阵右乘当前矩阵，即this*m
	shared_ptr<AbstractMatrix> multipleRight(shared_ptr<AbstractMatrix> m) override;
	//输入一个函数，所有元素执行这个函数
	shared_ptr<AbstractMatrix> map(double func(double)) override;
	//将矩阵的子矩阵与输入矩阵做卷积
	double convolve(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) override;
	shared_ptr<AbstractMatrix> m2vByColumn() override;
	shared_ptr<AbstractMatrix> v2mByColomn(size_t size) override;
	//矩阵相等
	bool equals(shared_ptr<AbstractMatrix> m) override;
	shared_ptr<AbstractMatrix> clone() override;
	void print() override;

	shared_ptr<AbstractMatrix> add_inplace(double x) override;
	shared_ptr<AbstractMatrix> add_inplace(shared_ptr<AbstractMatrix> m) override;
	shared_ptr<AbstractMatrix> add_inplace(int, int, double) override;
	shared_ptr<AbstractMatrix> add_inplace(size_t, size_t, size_t, size_t, shared_ptr<AbstractMatrix>) override;
	shared_ptr<AbstractMatrix> subtract_inplace(double) override;
	shared_ptr<AbstractMatrix> subtract_inplace(shared_ptr<AbstractMatrix>) override;
	shared_ptr<AbstractMatrix> multiple_inplace(double) override;
	shared_ptr<AbstractMatrix> multiple_inplace(shared_ptr<AbstractMatrix>) override;
	shared_ptr<AbstractMatrix> map_inplace(double func(double)) override;

private:
	void initialize() override;

};
