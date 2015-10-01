#pragma once
#include "utils.h"
#include "AbstractMatrix.h"

class Matrix : public AbstractMatrix
{

public:
	Matrix(size_t rowSize, size_t columnSize);
	Matrix();	
    ~Matrix() override;
	void initializeValue(double lowerBound, double upperBound) override;
	void initializeBinaryValue(double p) override;
	double sum() override;
	double max() override;
	shared_ptr<MaxData> max(int top, int bottom, int left, int right) override;
	double min() override;
	double norm1() override;
	double norm2() override;
	double trace() override;
	shared_ptr<AbstractMatrix> transpose() override;
	shared_ptr<AbstractMatrix> submatrix(int top, int bottom, int left, int right) override;
	shared_ptr<AbstractMatrix> submatrixShrink(int top, int bottom, int left, int right)override;
	shared_ptr<AbstractMatrix> submatrixExpand(int top, int bottom, int left, int right)override;
	vector<shared_ptr<AbstractMatrix>> splitRow(size_t rowIndex) override;
	vector<shared_ptr<AbstractMatrix>> splitColumn(size_t columnIndex) override;
	shared_ptr<AbstractMatrix> merge(shared_ptr<AbstractMatrix> rightTop, shared_ptr<AbstractMatrix> leftBottom, 
		shared_ptr<AbstractMatrix> rightBottom) override;
	shared_ptr<AbstractMatrix> mergeRow(shared_ptr<AbstractMatrix> bottom) override;
	shared_ptr<AbstractMatrix> mergeColumn(shared_ptr<AbstractMatrix> right) override;
	shared_ptr<AbstractMatrix> add(double x) override;
	shared_ptr<AbstractMatrix> add(shared_ptr<AbstractMatrix> m) override;
	shared_ptr<AbstractMatrix> add(int i, int j, double value) override;
	shared_ptr<AbstractMatrix> add(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) override;
	shared_ptr<AbstractMatrix> subtract(double x) override;
	shared_ptr<AbstractMatrix> subtract(shared_ptr<AbstractMatrix> m) override;
	shared_ptr<AbstractMatrix> multiple(double x) override;
	shared_ptr<AbstractMatrix> multiple(shared_ptr<AbstractMatrix> m) override;
	shared_ptr<AbstractMatrix> multipleLeft(shared_ptr<AbstractMatrix> m) override;
	shared_ptr<AbstractMatrix> multipleRight(shared_ptr<AbstractMatrix> m) override;
	shared_ptr<AbstractMatrix> map(double func(double)) override;
	double convolve(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) override;
	shared_ptr<AbstractMatrix> m2vByColumn() override;
	shared_ptr<AbstractMatrix> v2mByColomn(size_t size) override;
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
