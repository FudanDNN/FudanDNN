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
	void setValues(double value);
	bool inrange(int i, int j);
	void operator %= (Matrix);
	void operator += (Matrix);
	shared_ptr<Matrix> operator + (Matrix);
	void operator *= (Matrix);
	void operator -= (Matrix);
	shared_ptr<Matrix> operator - (Matrix);
	shared_ptr<Matrix> operator * (Matrix);
	void trans_i();
	shared_ptr<Matrix> trans();
	shared_ptr<Matrix> narrowConv(shared_ptr<Matrix>, int);
	shared_ptr<Matrix> wideConv(shared_ptr<Matrix>, int);
	void map(double function(double));
	shared_ptr<Matrix> clone();
	void print();

protected:
	mat matrix;
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