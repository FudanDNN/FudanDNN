#include "util/Matrix.h"
#include <iostream>

shared_ptr<MatrixPool> MatrixPool::instance;
MatrixPool::MatrixPool() 
{
}

inline shared_ptr<MatrixPool> MatrixPool::getInstance()
{
	return instance == nullptr ? (instance = shared_ptr<MatrixPool>(new MatrixPool())) : instance;
}

inline shared_ptr<Matrix> MatrixPool::allocMatrix(size_t rowSize, size_t columnSize){
	for (int i = pool.size() - 1; i >= 0; i--){
		if (pool[i].use_count() == 1 && pool[i]->getRowSize() == rowSize && pool[i]->getColumnSize() == columnSize){
			pool[i]->setValues(0);
			return pool[i];
		}
	}
	shared_ptr<Matrix> newMatrix(new Matrix(rowSize, columnSize));
	pool.push_back(newMatrix);
	return newMatrix;
}
inline shared_ptr<Matrix> MatrixPool::allocMatrixUnclean(size_t rowSize, size_t columnSize){
	for (int i = pool.size() - 1; i >= 0; i--){
		if (pool[i].use_count() == 1 && pool[i]->getRowSize() == rowSize && pool[i]->getColumnSize() == columnSize){
			//cout << "in" << endl;
			return pool[i];
		}
	}
	shared_ptr<Matrix> newMatrix(new Matrix(rowSize, columnSize));
	pool.push_back(newMatrix);
	return newMatrix;
}

Matrix::Matrix(int rowSize, int columnSize)
{
	this->rowSize = rowSize;
	this->columnSize = columnSize;
	this->size = rowSize * columnSize;
	this->matrix = new mat(rowSize, columnSize);
}

Matrix::~Matrix()
{
}

void Matrix::initializeRandom(double lowerBound, double upperBound)
{

	double interval = upperBound - lowerBound;
	matrix->randu();
	matrix->transform([interval, lowerBound](double x) { return interval * x + lowerBound; });

}

int Matrix::getSize()
{
	return size;
}

int Matrix::getRowSize()
{
	return rowSize;
}

int Matrix::getColumnSize()
{
	return columnSize;
}

double Matrix::getValue(int i, int j)
{
	return (*matrix)(i, j);
}

void Matrix::setValue(int i, int j, double val)
{
	(*matrix)(i, j) = val;
}

void Matrix::setValues(double value)
{
	matrix->fill(value);
}

bool Matrix::inrange(int i, int j)
{
	return (i >= 0) && (i < rowSize) && (j >= 0) && (j < columnSize);
}

void Matrix::mulewi(shared_ptr<Matrix> m)
{
	*matrix %= *(m->matrix);
}

void Matrix::addi(shared_ptr<Matrix> m)
{
	*matrix += *(m->matrix);
}

shared_ptr<Matrix> Matrix::add(shared_ptr<Matrix> m)
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	*result->matrix = *matrix + *(m->matrix);
	return result;
}

void Matrix::subi(shared_ptr<Matrix> m)
{
	*matrix -= *(m->matrix);
}

shared_ptr<Matrix> Matrix::sub(shared_ptr<Matrix> m)
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	*result->matrix = *matrix - *(m->matrix);
	return result;
}

void Matrix::muli(double x)
{
	*matrix *= x;
}

shared_ptr<Matrix> Matrix::mul(double x)
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	*result->matrix = *matrix * x;
	return result;
}

void Matrix::mulri(shared_ptr<Matrix> m)
{
	*matrix *= *(m->matrix);
}

shared_ptr<Matrix> Matrix::mull(shared_ptr<Matrix> m)
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(m->rowSize, columnSize);
	*result->matrix = *(m->matrix) * *matrix;
	return result;

}

shared_ptr<Matrix> Matrix::mulr(shared_ptr<Matrix> m)
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, m->columnSize);
	*result->matrix = *matrix * *(m->matrix);
	return result;
}

shared_ptr<Matrix> Matrix::narrowConv(shared_ptr<Matrix> kernel, int stride)
{
	int rowSize = (this->rowSize - kernel->rowSize) / stride + 1;
	int columnSize = (this->columnSize - kernel->columnSize) / stride + 1;
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = i * stride;
		double val = 0;
		for (int ki = kernel->rowSize - 1; ki >= 0; ki--, ii++) {
			int jj = j * stride;
			for (int kj = kernel->columnSize - 1; kj >= 0; kj--, jj++)
			{
				val += (*matrix)(ii, jj) * (*kernel->matrix)(ki, kj);
			}
		}
		(*(result->matrix))(i, j) = val;
	}
	return result;
}

shared_ptr<Matrix> Matrix::wideConv(shared_ptr<Matrix> kernel, int stride)
{
	int rowSize = (this->rowSize + kernel->rowSize) / stride - 1;
	int columnSize = (this->columnSize + kernel->columnSize) / stride - 1;
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = i * stride - kernel->rowSize + 1;
		int jj = j * stride - kernel->columnSize + 1;
		double val = 0;
		for (int ki = kernel->rowSize - 1; ki >= 0; ki--, ii++) {
			int jj = j * stride - kernel->columnSize + 1;
			for (int kj = kernel->columnSize - 1; kj >= 0; kj--, jj++)
			{
				if (!inrange(ii, jj)) continue;
				val += (*matrix)(ii, jj) * (*kernel->matrix)(ki, kj);
			}
		}
		(*(result->matrix))(i, j) = val;
	}
	return result;
}

shared_ptr<Matrix> Matrix::narrowRCorr(shared_ptr<Matrix> kernel, int stride)
{
	int rowSize = (this->rowSize - kernel->rowSize) / stride + 1;
	int columnSize = (this->columnSize - kernel->columnSize) / stride + 1;
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = rowSize - i * stride;
		double val = 0;
		for (int ki = kernel->rowSize - 1; ki >= 0; ki--, ii--) {
			int jj = columnSize - j * stride;
			for (int kj = kernel->columnSize - 1; kj >= 0; kj--, jj--)
			{
				val += (*matrix)(ii, jj) * (*kernel->matrix)(ki, kj);
			}
		}
		(*(result->matrix))(i, j) = val;
	}
	return result;
}

shared_ptr<Matrix> Matrix::wideRCorr(shared_ptr<Matrix> kernel, int stride)
{
	int rowSize = (this->rowSize + kernel->rowSize) / stride - 1;
	int columnSize = (this->columnSize + kernel->columnSize) / stride - 1;
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = rowSize - (i * stride - kernel->rowSize + 1);
		double val = 0;
		for (int ki = kernel->rowSize - 1; ki >= 0; ki--, ii++) {
			int jj = columnSize - (j * stride - kernel->columnSize + 1);
			for (int kj = kernel->columnSize - 1; kj >= 0; kj--, jj++)
			{
				if (!inrange(ii, jj)) continue;
				val += (*matrix)(ii, jj) * (*kernel->matrix)(ki, kj);
			}
		}
		(*(result->matrix))(i, j) = val;
	}
	return result;
}

shared_ptr<Matrix> Matrix::narrowCorr(shared_ptr<Matrix> kernel, int stride)
{
	int rowSize = (this->rowSize - kernel->rowSize) / stride + 1;
	int columnSize = (this->columnSize - kernel->columnSize) / stride + 1;
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = i * stride;
		double val = 0;
		for (int ki = 0; ki <  kernel->rowSize; ki++, ii++) {
			int jj = j * stride;
			for (int kj = 0; kj < kernel->columnSize; kj++, jj++)
			{
				val += (*matrix)(ii, jj) * (*kernel->matrix)(ki, kj);
			}
		}
		(*(result->matrix))(i, j) = val;
	}
	return result;
}

shared_ptr<Matrix> Matrix::wideCorr(shared_ptr<Matrix> kernel, int stride)
{
	int rowSize = (this->rowSize + kernel->rowSize) / stride - 1;
	int columnSize = (this->columnSize + kernel->columnSize) / stride - 1;
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = i * stride - kernel->rowSize + 1;
		int jj = j * stride - kernel->columnSize + 1;
		double val = 0;
		for (int ki = 0; ki <  kernel->rowSize; ki++, ii++) {
			int jj = j * stride - kernel->columnSize + 1;
			for (int kj = 0; kj <  kernel->columnSize; kj++, jj++)
			{
				if (!inrange(ii, jj)) continue;
				val += (*matrix)(ii, jj) * (*kernel->matrix)(ki, kj);
			}
		}
		(*(result->matrix))(i, j) = val;
	}
	return result;
}

shared_ptr<Matrix> Matrix::maxSubSampling(int kRowSize, int kColumnSize, int stride)
{
	int rowSize = this->rowSize / stride;
	int columnSize = this->columnSize / stride;
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	for (size_t i = 0; i < rowSize; i++)
	for (size_t j = 0; j < columnSize; j++)
	{
		int ii = i * stride;
		int jj = j * stride;
		double max = (*matrix)(i, j);
		for (size_t ki = 0; ki < kRowSize; ki++, ii++)
		for (size_t kj = 0; kj < kColumnSize; kj++, jj++)
		{
			if (max < (*matrix)(ii, jj))
				max = (*matrix)(ii, jj);
		}
		(*(result->matrix))(i, j) = max;
	}
	return result;
}

shared_ptr<Matrix> Matrix::maxUpSampling(int kRowSize, int kColumnSize, int stride, shared_ptr<Matrix> m)
{
	int rowSize = this->rowSize / stride;
	int columnSize = this->columnSize / stride;
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrix(this->rowSize, this->columnSize);
	for (size_t i = 0; i < rowSize; i++)
	for (size_t j = 0; j < columnSize; j++)
	{
		int ii = i * stride;
		int jj = j * stride;
		double max = (*matrix)(i, j);
		int mi = ii;
		int mj = jj;
		for (size_t ki = 0; ki < kRowSize; ki++, ii++)
		for (size_t kj = 0; kj < kColumnSize; kj++, jj++)
		{
			if (max < (*matrix)(ii, jj))
			{
				max = (*matrix)(ii, jj);
				mi = ii;
				mj = jj;
			}
		}
		(*(result->matrix))(mi, mj) = (*(m->matrix))(i, j);
	}
	return result;
}

void Matrix::trans_i()
{
	inplace_trans(*matrix);
}

shared_ptr<Matrix> Matrix::trans()
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(columnSize, rowSize);
	*(result->matrix) = matrix->t();
	return result;
}

shared_ptr<Matrix> Matrix::submatrix(int top, int bottom, int left, int right)
{
	if (!(inrange(top, left) && inrange(bottom - 1, right - 1)))
		return nullptr;
	int row = bottom - top;
	int column = right - left;
	shared_ptr<MatrixPool> instance = MatrixPool::getInstance();
	shared_ptr<Matrix> result = instance->allocMatrixUnclean(row, column);
	(*(result->matrix)) = matrix->submat(top, left, bottom - 1, right - 1);
	return result;
}


shared_ptr<Matrix> Matrix::mergeRow(shared_ptr<Matrix> m)
{
	size_t row = rowSize + m->rowSize;
	shared_ptr<MatrixPool> instance = MatrixPool::getInstance();
	shared_ptr<Matrix> result = instance->allocMatrixUnclean(row, columnSize);
	(*(result->matrix)) = arma::join_cols(*matrix, *(m->matrix));
	return result;
}

shared_ptr<Matrix> Matrix::clone()
{

	shared_ptr<MatrixPool> instance = MatrixPool::getInstance();
	shared_ptr<Matrix> result = instance->allocMatrixUnclean(rowSize, columnSize);
	*(result->matrix) = matrix->submat(0, rowSize - 1, 0, columnSize - 1);

	return result;

}

void Matrix::print()
{

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			cout << "\t" << (*matrix)(i, j);
		}
		cout << "\n";
	}
	cout << endl;
	
}	