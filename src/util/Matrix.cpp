#include "util/Matrix.h"
#include <iostream>

shared_ptr<MatrixPool> MatrixPool::instance;
MatrixPool::MatrixPool() {
}

inline shared_ptr<MatrixPool> MatrixPool::getInstance() 
{
	return instance == nullptr ? (instance = shared_ptr<MatrixPool>(new MatrixPool())) : instance;
}

inline shared_ptr<Matrix> MatrixPool::allocMatrix(size_t rowSize, size_t columnSize) 
{
	for (int i = pool.size() - 1; i >= 0; i--){
		if (pool[i].use_count() == 1 && pool[i]->getRowSize() == rowSize && pool[i]->getColumnSize() == columnSize)
		{
			pool[i]->setValues(0);
			return pool[i];
		}
	}
	shared_ptr<Matrix> newMatrix(new Matrix(rowSize, columnSize));
	newMatrix->setValues(0);
	pool.push_back(newMatrix);
	return newMatrix;
}

inline shared_ptr<Matrix> MatrixPool::allocMatrixUnclean(size_t rowSize, size_t columnSize)
{
	for (int i = pool.size() - 1; i >= 0; i--){
		if (pool[i].use_count() == 1 && pool[i]->getRowSize() == rowSize && pool[i]->getColumnSize() == columnSize)
		{
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

double Matrix::max()
{
	return (*matrix).max();
}

double Matrix::sum()
{
	return arma::accu(*matrix);
}

double Matrix::norm(double k)
{
	return arma::norm(arma::vectorise(*matrix), k);
}

void Matrix::normalize_i(double k)
{
	double x = norm(k);
	muli(1 / x);
}

void Matrix::mulewi(shared_ptr<Matrix> m)
{
	*matrix %= *(m->matrix);
}

shared_ptr<Matrix> Matrix::add(double x)
{
	shared_ptr<Matrix> result = clone();
	*result->matrix += x;
	return result;
}

shared_ptr<Matrix> Matrix::addi(shared_ptr<Matrix> m)
{
	*matrix += *(m->matrix);
	return shared_from_this();
}

shared_ptr<Matrix> Matrix::add(shared_ptr<Matrix> m)
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	*result->matrix = *matrix + *(m->matrix);
	return result;
}

void Matrix::add(shared_ptr<Matrix> m, shared_ptr<Matrix> dst)
{
	*dst->matrix = *matrix + *(m->matrix);
}

shared_ptr<Matrix> Matrix::sub(double x)
{
	shared_ptr<Matrix> result = clone();
	*result->matrix -= x;
	return result;
}

shared_ptr<Matrix> Matrix::subi(shared_ptr<Matrix> m)
{
	*matrix -= *(m->matrix);
	return shared_from_this();
}

shared_ptr<Matrix> Matrix::sub(shared_ptr<Matrix> m)
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	*result->matrix = *matrix - *(m->matrix);
	return result;
}

void Matrix::sub(shared_ptr<Matrix> m, shared_ptr<Matrix> dst)
{
	*dst->matrix = *matrix - *(m->matrix);
}

shared_ptr<Matrix> Matrix::muli(double x)
{
	*matrix *= x;
	return shared_from_this();
}

shared_ptr<Matrix> Matrix::mul(double x)
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	*result->matrix = *matrix * x;
	return result;
}

void Matrix::mul(double x, shared_ptr<Matrix> dst)
{
	*dst->matrix = *matrix * x;
}

shared_ptr<Matrix> Matrix::mulri(shared_ptr<Matrix> m)
{
	*matrix *= *(m->matrix);
	return shared_from_this();
}

shared_ptr<Matrix> Matrix::mull(shared_ptr<Matrix> m)
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(m->rowSize, columnSize);
	*result->matrix = *(m->matrix) * *matrix;
	return result;

}
void Matrix::mull(shared_ptr<Matrix> m, shared_ptr<Matrix> dst)
{
	*dst->matrix = *(m->matrix) * *matrix;
}

shared_ptr<Matrix> Matrix::mulr(shared_ptr<Matrix> m)
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, m->columnSize);
	*result->matrix = *matrix * *(m->matrix);
	return result;
}

void Matrix::mulr(shared_ptr<Matrix> m, shared_ptr<Matrix> dst)
{
	*dst->matrix = *matrix * *(m->matrix);
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

void Matrix::narrowConv(shared_ptr<Matrix> kernel, int stride, shared_ptr<Matrix> dst)
{
	int rowSize = (this->rowSize - kernel->rowSize) / stride + 1;
	int columnSize = (this->columnSize - kernel->columnSize) / stride + 1;
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
		(*(dst->matrix))(i, j) = val;
	}
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

void Matrix::wideConv(shared_ptr<Matrix> kernel, int stride, shared_ptr<Matrix> dst)
{
	int rowSize = (this->rowSize + kernel->rowSize) / stride - 1;
	int columnSize = (this->columnSize + kernel->columnSize) / stride - 1;
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
		(*(dst->matrix))(i, j) = val;
	}
}

shared_ptr<Matrix> Matrix::narrowRCorr(shared_ptr<Matrix> kernel, int stride)
{
	int rowSize = (this->rowSize - kernel->rowSize) / stride + 1;
	int columnSize = (this->columnSize - kernel->columnSize) / stride + 1;
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = this->rowSize - 1 - i * stride;
		double val = 0;
		for (int ki = kernel->rowSize - 1; ki >= 0; ki--, ii--) {
			int jj = this->columnSize - 1 - j * stride;
			for (int kj = kernel->columnSize - 1; kj >= 0; kj--, jj--)
			{
				val += (*matrix)(ii, jj) * (*kernel->matrix)(ki, kj);
			}
		}
		(*(result->matrix))(i, j) = val;
	}
	return result;
}

void Matrix::narrowRCorr(shared_ptr<Matrix> kernel, int stride, shared_ptr<Matrix> dst)
{
	int rowSize = (this->rowSize - kernel->rowSize) / stride + 1;
	int columnSize = (this->columnSize - kernel->columnSize) / stride + 1;
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = this->rowSize - 1 - i * stride;
		double val = 0;
		for (int ki = kernel->rowSize - 1; ki >= 0; ki--, ii--) {
			int jj = this->columnSize - 1 - j * stride;
			for (int kj = kernel->columnSize - 1; kj >= 0; kj--, jj--)
			{
				val += (*matrix)(ii, jj) * (*kernel->matrix)(ki, kj);
			}
		}
		(*(dst->matrix))(i, j) = val;
	}
}

shared_ptr<Matrix> Matrix::wideRCorr(shared_ptr<Matrix> kernel, int stride)
{
	int rowSize = (this->rowSize + kernel->rowSize) / stride - 1;
	int columnSize = (this->columnSize + kernel->columnSize) / stride - 1;
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = this->rowSize - 1 - (i * stride - kernel->rowSize + 1);
		double val = 0;
		for (int ki = kernel->rowSize - 1; ki >= 0; ki--, ii++) {
			int jj = this->columnSize - 1 - (j * stride - kernel->columnSize + 1);
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

void Matrix::wideRCorr(shared_ptr<Matrix> kernel, int stride, shared_ptr<Matrix> dst)
{
	int rowSize = (this->rowSize + kernel->rowSize) / stride - 1;
	int columnSize = (this->columnSize + kernel->columnSize) / stride - 1;
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = this->rowSize - 1 - (i * stride - kernel->rowSize + 1);
		double val = 0;
		for (int ki = kernel->rowSize - 1; ki >= 0; ki--, ii++) {
			int jj = this->columnSize - 1 - (j * stride - kernel->columnSize + 1);
			for (int kj = kernel->columnSize - 1; kj >= 0; kj--, jj++)
			{
				if (!inrange(ii, jj)) continue;
				val += (*matrix)(ii, jj) * (*kernel->matrix)(ki, kj);
			}
		}
		(*(dst->matrix))(i, j) = val;
	}
	
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

void Matrix::narrowCorr(shared_ptr<Matrix> kernel, int stride, shared_ptr<Matrix> dst)
{
	int rowSize = (this->rowSize - kernel->rowSize) / stride + 1;
	int columnSize = (this->columnSize - kernel->columnSize) / stride + 1;
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = i * stride;
		double val = 0;
		for (int ki = 0; ki < kernel->rowSize; ki++, ii++) {
			int jj = j * stride;
			for (int kj = 0; kj < kernel->columnSize; kj++, jj++)
			{
				val += (*matrix)(ii, jj) * (*kernel->matrix)(ki, kj);
			}
		}
		(*(dst->matrix))(i, j) = val;
	}
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

void Matrix::wideCorr(shared_ptr<Matrix> kernel, int stride, shared_ptr<Matrix> dst)
{
	int rowSize = (this->rowSize + kernel->rowSize) / stride - 1;
	int columnSize = (this->columnSize + kernel->columnSize) / stride - 1;
	for (int i = 0; i < rowSize; i++)
	for (int j = 0; j < columnSize; j++)
	{
		int ii = i * stride - kernel->rowSize + 1;
		int jj = j * stride - kernel->columnSize + 1;
		double val = 0;
		for (int ki = 0; ki < kernel->rowSize; ki++, ii++) {
			int jj = j * stride - kernel->columnSize + 1;
			for (int kj = 0; kj < kernel->columnSize; kj++, jj++)
			{
				if (!inrange(ii, jj)) continue;
				val += (*matrix)(ii, jj) * (*kernel->matrix)(ki, kj);
			}
		}
		(*(dst->matrix))(i, j) = val;
	}
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
		double max = (*matrix)(i, j);
		for (size_t ki = 0; ki < kRowSize; ki++, ii++)
		{
			int jj = j * stride;
			for (size_t kj = 0; kj < kColumnSize; kj++, jj++)
			{
				if (max < (*matrix)(ii, jj))
					max = (*matrix)(ii, jj);
			}
		}
		(*(result->matrix))(i, j) = max;
	}
	return result;
}

void Matrix::maxSubSampling(int kRowSize, int kColumnSize, int stride, shared_ptr<Matrix> dst)
{
	int rowSize = this->rowSize / stride;
	int columnSize = this->columnSize / stride;
	for (size_t i = 0; i < rowSize; i++)
	for (size_t j = 0; j < columnSize; j++)
	{
		int ii = i * stride;
		double max = (*matrix)(i, j);
		for (size_t ki = 0; ki < kRowSize; ki++, ii++)
		{
			int jj = j * stride;
			for (size_t kj = 0; kj < kColumnSize; kj++, jj++)
			{
				if (max < (*matrix)(ii, jj))
					max = (*matrix)(ii, jj);
			}
		}
		(*(dst->matrix))(i, j) = max;
	}
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
		int mi = ii;
		int mj = jj;
		double max = (*matrix)(ii, jj);
		for (size_t ki = 0; ki < kRowSize; ki++, ii++)
		{
			int jj = j * stride;
			for (size_t kj = 0; kj < kColumnSize; kj++, jj++)
			{
				if (max < (*matrix)(ii, jj))
				{
					max = (*matrix)(ii, jj);
					mi = ii;
					mj = jj;
				}
			}
		}
		(*(result->matrix))(mi, mj) = (*(m->matrix))(i, j);
	}
	return result;
}

void Matrix::maxUpSampling(int kRowSize, int kColumnSize, int stride, shared_ptr<Matrix> m, shared_ptr<Matrix> dst)
{
	int rowSize = this->rowSize / stride;
	int columnSize = this->columnSize / stride;
	for (size_t i = 0; i < rowSize; i++)
	for (size_t j = 0; j < columnSize; j++)
	{
		int ii = i * stride;
		int jj = j * stride;
		double max = (*matrix)(ii, jj);
		int mi = ii;
		int mj = jj;
		for (size_t ki = 0; ki < kRowSize; ki++, ii++)
		{
			jj = j * stride;
			for (size_t kj = 0; kj < kColumnSize; kj++, jj++)
			{
				if (max < (*matrix)(ii, jj))
				{
					max = (*matrix)(ii, jj);
					mi = ii;
					mj = jj;
				}
			}
		}
		(*(dst->matrix))(mi, mj) = (*(m->matrix))(i, j);
	}
}


shared_ptr<Matrix> Matrix::trans_i()
{
	inplace_trans(*matrix);
	return shared_from_this();
}

shared_ptr<Matrix> Matrix::trans()
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(columnSize, rowSize);
	*(result->matrix) = matrix->t();
	return result;
}

void Matrix::trans(shared_ptr<Matrix> dst)
{
	*(dst->matrix) = matrix->t();
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

void Matrix::submatrix(int top, int bottom, int left, int right, shared_ptr<Matrix> dst)
{
	if (!(inrange(top, left) && inrange(bottom - 1, right - 1)))
		return;
	(*(dst->matrix)) = matrix->submat(top, left, bottom - 1, right - 1);
}

void Matrix::setSubmatrix(int top, int left, shared_ptr<Matrix> m)
{
	if (!(inrange(top, left) && inrange(top + m->rowSize - 1, left + m->columnSize - 1)))
		return;
	for (size_t i = 0; i < m->rowSize; i++)
	for (size_t j = 0; j < m->columnSize; j++)
		(*matrix)(top + i, left + j) = m->getValue(i, j);
}

void Matrix::reshape_i(int rowSize, int columnSize)
{
	if (rowSize * columnSize != size)
		return;
	this->rowSize = rowSize;
	this->columnSize = columnSize;
	(*matrix).reshape(rowSize, columnSize);
}

shared_ptr<Matrix> Matrix::reshape(int rowSize, int columnSize)
{
	shared_ptr<Matrix> result = this->clone();
	result->reshape_i(rowSize, columnSize);
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

void Matrix::mergeRow(shared_ptr<Matrix> m, shared_ptr<Matrix> dst)
{
	(*(dst->matrix)) = arma::join_cols(*matrix, *(m->matrix));
}

shared_ptr<Matrix> Matrix::clone()
{
	shared_ptr<MatrixPool> instance = MatrixPool::getInstance();
	shared_ptr<Matrix> result = instance->allocMatrixUnclean(rowSize, columnSize);
	*(result->matrix) = matrix->submat(0, 0, rowSize - 1, columnSize - 1);
	return result;
}

void Matrix::clone(shared_ptr<Matrix> dst)
{
	*(dst->matrix) = matrix->submat(0, 0, rowSize - 1, columnSize - 1);
	dst->rowSize = rowSize;
	dst->columnSize = columnSize;
}

void Matrix::mapi(double f(double))
{
	(*matrix).transform(f);
}

void Matrix::map(double f(double), shared_ptr<Matrix> dst)
{
	clone(dst);
	dst->mapi(f);
}

shared_ptr<Matrix> Matrix::map(double f(double))
{
	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrixUnclean(rowSize, columnSize);
	map(f, result);
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