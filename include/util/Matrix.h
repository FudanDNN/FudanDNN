#ifndef __FUDANDNN_MATRIX_H
#define __FUDANDNN_MATRIX_H

#include "utils.h"
#include "openblas/cblas.h"
#include <math.h>
#include <armadillo/armadillo>

using namespace std;
using namespace arma;

class Matrix
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
	//element-wise multiplication
	void mulewi(shared_ptr<Matrix>);
	//in-place addition
	void addi(shared_ptr<Matrix>);
	//addition
	shared_ptr<Matrix> add(shared_ptr<Matrix>);
	//in-place subtraction
	void subi(shared_ptr<Matrix>);
	//subtraction
	shared_ptr<Matrix> sub(shared_ptr<Matrix>);
	//in-place multiplication
	void muli(double x);
	//in-place multiplication
	shared_ptr<Matrix> mul(double x);
	//in-place right multiplication (this = this * x)
	void mulri(shared_ptr<Matrix>);
	//left multiplicatioin (return this * x)
	shared_ptr<Matrix> mull(shared_ptr<Matrix>);
	//right multiplicatioin (return x * this)
	shared_ptr<Matrix> mulr(shared_ptr<Matrix>);
	//in-place transposition (this = this')
	void trans_i();
	//transposition ( return this')
	shared_ptr<Matrix> trans();
	//narrow convolution
	shared_ptr<Matrix> narrowConv(shared_ptr<Matrix>, int);
	//wide convolution
	shared_ptr<Matrix> wideConv(shared_ptr<Matrix>, int);
	//narrow correlation
	shared_ptr<Matrix> narrowCorr(shared_ptr<Matrix>, int);
	//wide correlation
	shared_ptr<Matrix> wideCorr(shared_ptr<Matrix>, int);
	//narrow correlation as kernel role
	shared_ptr<Matrix> narrowRCorr(shared_ptr<Matrix>, int);
	//wide correlation as kernel role
	shared_ptr<Matrix> wideRCorr(shared_ptr<Matrix>, int);
	//max-pooing(subsampling) with specific kernel size
	shared_ptr<Matrix> maxSubSampling(int kRowSize, int kColumnSize, int stride);
	//max-pooing(upsampling) with specific kernel size
	shared_ptr<Matrix> maxUpSampling(int kRowSize, int kColumnSize, int stride, shared_ptr<Matrix> m);
	//submatrix [ (top, left), (bottom, right) )
	shared_ptr<Matrix> submatrix(int top, int bottom, int left, int right);
	//merge two matrix by row
	shared_ptr<Matrix> mergeRow(shared_ptr<Matrix>);
	shared_ptr<Matrix> clone();
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