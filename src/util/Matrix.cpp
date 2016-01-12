#include "../header/util/Matrix.h"

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
inline shared_ptr<Matrix> MatrixPool::allocMatrixUnClean(size_t rowSize, size_t columnSize){
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
	this->data = new double[size];
	this->matrix = new double *[rowSize];
	this->initialize();
}

Matrix::~Matrix()
{
}

void Matrix::initialize()
{
	double *pointer = data;
	for (size_t i = 0; i < rowSize; i++)
	{
		matrix[i] = pointer;
		pointer += columnSize;
	}
}

void Matrix::initializeRandom(double lowerBound, double upperBound)
{

	initialize();
	double range = upperBound - lowerBound;
	for (size_t i = 0; i < size; i++)
	{
		data[i] = ((double)rand()) / RAND_MAX * range + lowerBound;
	}

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

void Matrix::setValues(double value)
{
	memset(data, value, sizeof(double) * size);
}

shared_ptr<Matrix> Matrix::clone()
{

	shared_ptr<Matrix> result = MatrixPool::getInstance()->allocMatrix(rowSize, columnSize);
	cblas_dcopy(size, this->data, 1, result->data, 1);

	return result;

}

void Matrix::print()
{

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			cout << "\t" << matrix[i][j];
		}
		cout << "\n";
	}
	
}

int main()
{

	shared_ptr<MatrixPool> mp = MatrixPool::getInstance();
	shared_ptr<Matrix> A = mp->allocMatrix(3, 3);
	A->initializeRandom(-1, 1);
	A->print();

}