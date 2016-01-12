#ifndef __FUDANDNN_MATRIX_H
#define __FUDANDNN_MATRIX_H

#include "utils.h"
#include "cblas.h"
#include <math.h>

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
	shared_ptr<Matrix> clone();
	void print();

protected:
	double *data;
	double **matrix;
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
	shared_ptr<Matrix> allocMatrixUnClean(size_t rowSize, size_t columnSize);
};

#endif