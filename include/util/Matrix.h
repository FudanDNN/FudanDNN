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
	bool inrange(int i, int j);
	//element-wise multiplication
	void mulewi(shared_ptr<Matrix>);
	void addi(shared_ptr<Matrix>);
	shared_ptr<Matrix> add(shared_ptr<Matrix>);
	void subi(shared_ptr<Matrix>);
	shared_ptr<Matrix> sub(shared_ptr<Matrix>);
	void muli(shared_ptr<Matrix>);
	shared_ptr<Matrix> mul(shared_ptr<Matrix>);
	void trans_i();
	shared_ptr<Matrix> trans();
	shared_ptr<Matrix> narrowConv(shared_ptr<Matrix>, int);
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