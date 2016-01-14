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
	void setValue(int i, int j, int val);
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