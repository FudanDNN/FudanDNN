#include "AbstractMatrix.h"


void AbstractMatrix::setValue(size_t i, size_t j, double value)
{
	matrix[i][j] = value;
}
double AbstractMatrix::getValue(size_t i, size_t j)
{
	return matrix[i][j];
}

size_t AbstractMatrix::getRowSize()
{
	return this->rowSize;
}

size_t AbstractMatrix::getColumnSize()
{
	return this->columnSize;
}

void AbstractMatrix::setRow(size_t i, vector<double> row)
{
	for (int j = 0; j < this->columnSize; j++){
		matrix[i][j] = row[j];
	}
}

vector<double> AbstractMatrix::getRow(size_t i)
{
	vector<double> result;
	for (size_t j = 0; j < columnSize; j++)
	{
		result.push_back(matrix[i][j]);
	}
	return result;
}

void AbstractMatrix::setColumn(size_t j, vector<double> column)
{
	for (int i = 0; i < this->rowSize; i++) {
		matrix[i][j] = column[i];
	}
}

vector<double> AbstractMatrix::getColumn(size_t j)
{
	vector<double> result;
	for (int i = 0; i < this->rowSize; i++) {
		result.push_back(matrix[i][j]);
	}
	return result;
}

bool AbstractMatrix::bounded(int i, int j)
{
	return (i >= 0) && (i < (int)rowSize) && (j >= 0) && (j < (int)columnSize);
}
