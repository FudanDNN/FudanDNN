#ifndef __FUDANDNN_MATRIX_H
#define __FUDANDNN_MATRIX_H

#include "utils.h"
#include "openblas/cblas.h"
#include <math.h>
#include <armadillo/armadillo>

using namespace std;
using namespace arma;

class Matrix : public enable_shared_from_this<Matrix>
{
public:
	Matrix(int rowSize, int columnSize);
	~Matrix();

	void initialize();
	void initializeRandom(double lowerBound, double upperBound);
	int getSize();
	int getRowSize();
	int getColumnSize();
	double getValue(int i, int j);
	void setValue(int i, int j, double val);
	void setValues(double value);
	//inside matrix
	bool inrange(int i, int j);
	//max of matrix
	double max();
	//sum of matrix
	double sum();
	//k-norm 
	double norm(double k);
	//normalize matrix with k-norm
	void normalize_i(double k);
	//element-wise multiplication
	void mulewi(shared_ptr<Matrix>);
	//in-place addition
	shared_ptr<Matrix> addi(shared_ptr<Matrix>);
	//addition
	shared_ptr<Matrix> add(shared_ptr<Matrix>);
	//addition
	void add(shared_ptr<Matrix>, shared_ptr<Matrix> dst);
	//in-place subtraction
	shared_ptr<Matrix> subi(shared_ptr<Matrix>);
	//subtraction
	shared_ptr<Matrix> sub(shared_ptr<Matrix>);
	//subtraction
	void sub(shared_ptr<Matrix>, shared_ptr<Matrix> dst);
	//in-place multiplication
	shared_ptr<Matrix> muli(double x);
	//multiplication
	shared_ptr<Matrix> mul(double x);
	//multiplication
	void mul(double x, shared_ptr<Matrix> dst);
	//in-place right multiplication (this = this * x)
	shared_ptr<Matrix> mulri(shared_ptr<Matrix>);
	//left multiplicatioin (return this * x)
	shared_ptr<Matrix> mull(shared_ptr<Matrix>);
	//left multiplicatioin (return this * x)
	void mull(shared_ptr<Matrix>, shared_ptr<Matrix> dst);
	//right multiplicatioin (return x * this)
	shared_ptr<Matrix> mulr(shared_ptr<Matrix>);
	//right multiplicatioin (return x * this)
	void mulr(shared_ptr<Matrix>, shared_ptr<Matrix> dst);
	//in-place transposition (this = this')
	shared_ptr<Matrix> trans_i();
	//transposition ( return this')
	shared_ptr<Matrix> trans();
	//transposition ( return this')
	void trans(shared_ptr<Matrix>);
	//narrow convolution
	shared_ptr<Matrix> narrowConv(shared_ptr<Matrix>, int);
	//narrow convolution
	void narrowConv(shared_ptr<Matrix>, int, shared_ptr<Matrix> dst);
	//wide convolution
	shared_ptr<Matrix> wideConv(shared_ptr<Matrix>, int);
	//wide convolution
	void wideConv(shared_ptr<Matrix>, int, shared_ptr<Matrix> dst);
	//narrow correlation
	shared_ptr<Matrix> narrowCorr(shared_ptr<Matrix>, int);
	//narrow correlation
	void narrowCorr(shared_ptr<Matrix>, int, shared_ptr<Matrix> dst);
	//wide correlation
	shared_ptr<Matrix> wideCorr(shared_ptr<Matrix>, int);
	//wide correlation
	void wideCorr(shared_ptr<Matrix>, int, shared_ptr<Matrix> dst);
	//narrow correlation as kernel role
	shared_ptr<Matrix> narrowRCorr(shared_ptr<Matrix>, int);
	//narrow correlation as kernel role
	void narrowRCorr(shared_ptr<Matrix>, int, shared_ptr<Matrix> dst);
	//wide correlation as kernel role
	shared_ptr<Matrix> wideRCorr(shared_ptr<Matrix>, int);
	//wide correlation as kernel role
	void wideRCorr(shared_ptr<Matrix>, int, shared_ptr<Matrix> dst);
	//max-pooing(subsampling) with specific kernel size
	shared_ptr<Matrix> maxSubSampling(int kRowSize, int kColumnSize, int stride);
	//max-pooing(subsampling) with specific kernel size
	void maxSubSampling(int kRowSize, int kColumnSize, int stride, shared_ptr<Matrix> dst);
	//max-pooing(upsampling) with specific kernel size
	shared_ptr<Matrix> maxUpSampling(int kRowSize, int kColumnSize, int stride, shared_ptr<Matrix> m);
	//max-pooing(upsampling) with specific kernel size
	void maxUpSampling(int kRowSize, int kColumnSize, int stride, shared_ptr<Matrix> m, shared_ptr<Matrix> dst);
	//submatrix [ (top, left), (bottom, right) )
	shared_ptr<Matrix> submatrix(int top, int bottom, int left, int right);
	//submatrix [ (top, left), (bottom, right) )
	void submatrix(int top, int bottom, int left, int right, shared_ptr<Matrix> dst);
	//set submatrix [ (top, left), (top + m.row, left + m.column) )
	void setSubmatrix(int top, int left, shared_ptr<Matrix> m);
	//in-place reshape matrix
	void reshape_i(int rowSize, int columnSize);
	//reshape matrix
	shared_ptr<Matrix> reshape(int rowSize, int columnSize);
	//merge two matrix by row
	shared_ptr<Matrix> mergeRow(shared_ptr<Matrix>);
	//merge two matrix by row
	void mergeRow(shared_ptr<Matrix>, shared_ptr<Matrix> dst);
	shared_ptr<Matrix> clone();
	void clone(shared_ptr<Matrix> dst);
	void map(double(double), shared_ptr<Matrix>);
	void print();

protected:
	mat *matrix;
	int rowSize;
	int columnSize;
	int size;

};

class MatrixPool{
private:
	vector<shared_ptr<Matrix>> pool;
	static shared_ptr<MatrixPool> instance;
	MatrixPool();
public:
	static shared_ptr<MatrixPool> getInstance();
	shared_ptr<Matrix> allocMatrix(size_t rowSize, size_t columnSize);
	shared_ptr<Matrix> allocMatrixUnclean(size_t rowSize, size_t columnSize);
};

#endif