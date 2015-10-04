#include "Matrix.h"

Matrix::Matrix(size_t rowSize, size_t columnSize)
{

	this->rowSize = rowSize;
	this->columnSize = columnSize;

	this->initialize();

}

Matrix::Matrix()
{

	this->rowSize = 0;
	this->columnSize = 0;

	this->initialize();

}

Matrix::~Matrix()
{
	for (size_t i = 0; i < rowSize; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

void Matrix::initialize()
{

	matrix = new double *[rowSize];

	for (size_t i = 0; i < rowSize; i++)
	{
		matrix[i] = new double[columnSize];
	}
	this->initializeValue(0, 0);
}

void Matrix::initializeBinaryValue(double p)
{

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			matrix[i][j] = ((double)rand()) / RAND_MAX < p ? 1 : 0;
		}
	}

}

void Matrix::initializeValue(double lowerBound, double upperBound)
{

	double range = upperBound - lowerBound;
	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			matrix[i][j] = ((double)rand()) / RAND_MAX * range + lowerBound;
		}
	}

}

double Matrix::sum()
{

	double result = 0;
	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			result += abs(matrix[i][j]);
		}
	}

	return result;

}

double Matrix::max()
{

	double result = matrix[0][0];
	for (size_t i = 0; i < this->getRowSize(); i++)
	{
		for (size_t j = 0; j < this->getColumnSize(); j++)
		{
			if (result < matrix[i][j])
				result = matrix[i][j];
		}
	}

	return result;

}

shared_ptr<MaxData> Matrix::max(int top, int bottom, int left, int right) {

	if (!bounded(top, left) || !bounded(bottom - 1, right - 1)){
		cerr << "max(int " << top << ", int " << bottom << ", int " << left << ", int " << right << ") out of bound!" << endl;
		return nullptr;
	}
	if ((bottom - top <= 0) || (right - left <= 0)){
		cerr << "max(int " << top << ", int " << bottom << ", int " << left << ", int " << right << ") out of bound!" << endl;
		return nullptr;
	}

	shared_ptr<MaxData> result(new MaxData());
	result->value = this->getValue(top, left);
	result->pos_i = top;
	result->pos_j = left;

	for (size_t i = top; i < bottom; i++)
	{
		for (size_t j = left; j < right; j++)
		{
			if (result->value < this->getValue(i, j))
			{
				result->value = this->getValue(i, j);
				result->pos_i = i;
				result->pos_j = j;
			}
		}
	}

	return result;

}

double Matrix::min()
{

	double result = matrix[0][0];
	for (size_t i = 0; i < this->getRowSize(); i++)
	{
		for (size_t j = 0; j < this->getColumnSize(); j++)
		{
			if (result > matrix[i][j])
				result = matrix[i][j];
		}
	}

	return result;

}

double Matrix::norm1()
{

	double result = 0;
	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			result += abs(matrix[i][j]);
		}
	}

	return result;

}

double Matrix::norm2()
{

	double result = 0;
	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			result += matrix[i][j] * matrix[i][j];
		}
	}

	return result;

}

double Matrix::trace()
{

	if (rowSize != columnSize)
		return 0;

	double result = 0;
	for (size_t i = 0; i < rowSize; i++)
	{
		result += matrix[i][i];
	}

	return result;

}

shared_ptr<AbstractMatrix> Matrix::transpose()
{

	shared_ptr<AbstractMatrix> result(new Matrix(columnSize, rowSize));
	for (size_t i = 0; i < columnSize; i++)
	{
		for (size_t j = 0; j < rowSize; j++)
		{
			result->setValue(i, j, matrix[j][i]);
		}
	}

	return result;

}

shared_ptr<AbstractMatrix> Matrix::submatrix(int t, int b, int l, int r)
{

	if (!bounded(t, l) || !bounded(b - 1, r - 1)){
		cerr << "submatrix(int t, int b, int l, int r) out of bound!" << endl;
		return nullptr;
	}

	return submatrixShrink(t, b, l, r);

}

shared_ptr<AbstractMatrix> Matrix::submatrixShrink(int t, int b, int l, int r)
{

	if (l >= r || t >= b){
		cerr << "submatrixShrink(int t, int b, int l, int r) out of bound!" << endl;
		return nullptr;
	}

	size_t top = t < 0 ? 0 : (size_t)t;
	size_t bottom = b >(int)rowSize ? rowSize : (size_t)b;
	size_t left = l < 0 ? 0 : (size_t)l;
	size_t right = r >(int)columnSize ? columnSize : (size_t)r;

	shared_ptr<AbstractMatrix> result(new Matrix(bottom - top, right - left));

	for (size_t i = 0; i < result->getRowSize(); i++)
	{
		for (size_t j = 0; j < result->getColumnSize(); j++)
		{
			result->setValue(i, j, matrix[i + top][j + left]);
		}
	}

	return result;

}

shared_ptr<AbstractMatrix> Matrix::submatrixExpand(int top, int bottom, int left, int right)
{
	if (left >= right || top >= bottom){
		cerr << "submatrixExpand(int top, int bottom, int left, int right) out of bound!" << endl;
		return nullptr;
	}

	shared_ptr<AbstractMatrix> result(new Matrix((size_t)(bottom - top), (size_t)(right - left)));

	for (size_t i = 0; i < result->getRowSize(); i++)
	{
		for (size_t j = 0; j < result->getColumnSize(); j++)
		{
			result->setValue(i, j, bounded(i + top, j + left) ? matrix[i + top][j + left] : 0);
		}
	}

	return result;

}

shared_ptr<AbstractMatrix> Matrix::merge(shared_ptr<AbstractMatrix> right_top, shared_ptr<AbstractMatrix> left_bottom,
	shared_ptr<AbstractMatrix> right_bottom)
{

	if (columnSize != left_bottom->getColumnSize()
		|| right_top->getColumnSize() != right_bottom->getColumnSize()
		|| rowSize != right_top->getRowSize()
		|| left_bottom->getRowSize() != right_bottom->getRowSize()){
		cerr << "merge(shared_ptr<AbstractMatrix>right_top, shared_ptr<AbstractMatrix>left_bottom, shared_ptr<AbstractMatrix>right_bottom) out of bound!" << endl;
		return nullptr;
	}

	shared_ptr<AbstractMatrix> result(new Matrix(rowSize + left_bottom->getRowSize(), columnSize + right_top->getColumnSize()));

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			result->setValue(i, j, matrix[i][j]);
		}
	}

	for (size_t i = 0; i < right_top->getRowSize(); i++)
	{
		for (size_t j = 0; j < right_top->getColumnSize(); j++)
		{
			result->setValue(i, j + columnSize, right_top->getValue(i, j));
		}
	}

	for (size_t i = 0; i < left_bottom->getRowSize(); i++)
	{
		for (size_t j = 0; j < left_bottom->getColumnSize(); j++)
		{
			result->setValue(i + rowSize, j, left_bottom->getValue(i, j));
		}
	}

	for (size_t i = 0; i < right_bottom->getRowSize(); i++)
	{
		for (size_t j = 0; j < right_bottom->getColumnSize(); j++)
		{
			result->setValue(i + rowSize, j + columnSize, right_bottom->getValue(i, j));
		}
	}

	return result;

}

shared_ptr<AbstractMatrix> Matrix::mergeRow(shared_ptr<AbstractMatrix> bottom)
{

	if (columnSize != bottom->getColumnSize()){
		cerr << "mergeRow(shared_ptr<AbstractMatrix>bottom) out of bound!" << endl;
		return nullptr;
	}

	return merge(shared_ptr<AbstractMatrix>(new Matrix(rowSize, 0)), bottom,
		shared_ptr<AbstractMatrix>(new Matrix(bottom->getRowSize(), 0)));

}

shared_ptr<AbstractMatrix> Matrix::mergeColumn(shared_ptr<AbstractMatrix> right)
{

	if (rowSize != right->getRowSize()){
		cerr << "mergeColumn(shared_ptr<AbstractMatrix>right) out of bound!" << endl;
		return nullptr;
	}

	return merge(right, shared_ptr<AbstractMatrix>(new Matrix(0, columnSize)),
		shared_ptr<AbstractMatrix>(new Matrix(0, right->getRowSize())));

}

vector<shared_ptr<AbstractMatrix>> Matrix::splitRow(size_t rowIndex)
{

	if (rowIndex > rowSize)
		rowIndex = rowSize;

	vector<shared_ptr<AbstractMatrix>> result;

	result.push_back(submatrixShrink(0, rowIndex, 0, columnSize));
	result.push_back(submatrixShrink(rowIndex, rowSize, 0, columnSize));

	return result;

}

vector<shared_ptr<AbstractMatrix>> Matrix::splitColumn(size_t columnIndex)
{

	if (columnIndex > columnSize)
		columnIndex = columnSize;

	vector<shared_ptr<AbstractMatrix>> result;

	result.push_back(submatrixShrink(0, rowSize, 0, columnIndex));
	result.push_back(submatrixShrink(0, rowSize, columnIndex, columnSize));

	return result;

}

shared_ptr<AbstractMatrix> Matrix::add(double x)
{

	shared_ptr<AbstractMatrix> result(new Matrix(rowSize, columnSize));

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			result->setValue(i, j, matrix[i][j] + x);
		}
	}

	return result;

}

shared_ptr<AbstractMatrix> Matrix::add(shared_ptr<AbstractMatrix> m)
{
	if (m->getColumnSize() != columnSize || m->getRowSize() != rowSize){
		cerr << "add(shared_ptr<AbstractMatrix> m) does not match!" << endl;
		return shared_ptr<AbstractMatrix>(this);
	}

	shared_ptr<AbstractMatrix> result(new Matrix(rowSize, columnSize));

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			result->setValue(i, j, matrix[i][j] + m->getValue(i, j));
		}
	}

	return result;

}

shared_ptr<AbstractMatrix> Matrix::add(int i, int j, double value) {
	if (!bounded(i, j))
	{
		cerr << "add(int i, int j, double value) out of bound!" << endl;
		return shared_ptr<AbstractMatrix>(this);
	}
	shared_ptr<AbstractMatrix> result = this->clone();
	result->setValue(i, j, result->getValue(i, j) + value);
	return result;


};

shared_ptr<AbstractMatrix> Matrix::add(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m)
{

	if (!bounded(top, left) || !bounded(bottom - 1, right - 1)){
		cerr << "add(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) out of bound!" << endl;
		return nullptr;
	}
	if ((bottom - top != m->getRowSize()) || (right - left != m->getColumnSize())){
		cerr << "add(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) out of bound!" << endl;
		return nullptr;
	}

	shared_ptr<AbstractMatrix> result = this->clone();
	for (size_t i = 0; i < m->getRowSize(); i++)
	{
		for (size_t j = 0; j < m->getColumnSize(); j++)
		{
			result->add(top + i, left + j, m->getValue(i, j));
		}
	}

	return result;

}

shared_ptr<AbstractMatrix> Matrix::subtract(double x)
{
	return add(-x);
}

shared_ptr<AbstractMatrix> Matrix::subtract(shared_ptr<AbstractMatrix> m)
{
	if (m->getColumnSize() != columnSize || m->getRowSize() != rowSize){
		cerr << "subtract(shared_ptr<AbstractMatrix>m) does not match!" << endl;
		return shared_ptr<AbstractMatrix>(this);
	}
	shared_ptr<AbstractMatrix> result(new Matrix(rowSize, columnSize));

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			result->setValue(i, j, matrix[i][j] - m->getValue(i, j));
		}
	}

	return result;

}

shared_ptr<AbstractMatrix> Matrix::multiple(double x)
{

	shared_ptr<AbstractMatrix> result(new Matrix(rowSize, columnSize));
	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			result->setValue(i, j, matrix[i][j] * x);
		}
	}

	return result;

}

shared_ptr<AbstractMatrix> Matrix::multiple(shared_ptr<AbstractMatrix> m)
{
	if (m->getColumnSize() != columnSize || m->getRowSize() != rowSize){
		cerr << "mutiple(shared_ptr<AbstractMatrix>m) does not match!" << endl;
		return shared_ptr<AbstractMatrix>(this);
	}
	shared_ptr<AbstractMatrix> result(new Matrix(rowSize, columnSize));
	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			result->setValue(i, j, matrix[i][j] * m->getValue(i, j));
		}
	}
	return result;
}

shared_ptr<AbstractMatrix> Matrix::multipleLeft(shared_ptr<AbstractMatrix> m)
{

	if (m->getColumnSize() != rowSize){
		cerr << "m->getColumnSize() != rowSize" << endl;
		return clone();
	}

	shared_ptr<AbstractMatrix> result;

	if (rowSize < 4 || columnSize < 4 || m->getRowSize() < 4) {

		result = shared_ptr<AbstractMatrix>(new Matrix(m->getRowSize(), columnSize));

		for (size_t i = 0; i < m->getRowSize(); i++)
		{
			for (size_t j = 0; j < columnSize; j++)
			{
				double ans = 0;
				for (size_t k = 0; k < rowSize; k++)
				{
					ans += m->getValue(i, k) * matrix[k][j];
				}
				result->setValue(i, j, ans);
			}
		}

		return result;

	}

	int rsz = (m->getRowSize() + 3) / 4;
	int csz = (columnSize + 3) / 4;
	int msz = (rowSize + 3) / 4;

	shared_ptr<AbstractMatrix> L[4][4];
	shared_ptr<AbstractMatrix> R[4][4];

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			L[i][j] = m->submatrixExpand(rsz * i, rsz * (i + 1), msz * j, msz * (j + 1));
			R[i][j] = submatrixExpand(msz * i, msz * (i + 1), csz * j, csz * (j + 1));
		}
	}

	shared_ptr<AbstractMatrix> x[7];

	shared_ptr<AbstractMatrix> L0 = L[0][0]->add(L[2][2])->merge(L[0][1]->add(L[2][3]), L[1][0]->add(L[3][2]), L[1][1]->add(L[3][3]));
	shared_ptr<AbstractMatrix> R0 = R[0][0]->add(R[2][2])->merge(R[0][1]->add(R[2][3]), R[1][0]->add(R[3][2]), R[1][1]->add(R[3][3]));
	x[0] = R0->multipleLeft(L0);

	shared_ptr<AbstractMatrix> L1 = L[2][0]->add(L[2][2])->merge(L[2][1]->add(L[2][3]), L[3][0]->add(L[3][2]), L[3][1]->add(L[3][3]));
	shared_ptr<AbstractMatrix> R1 = R[0][0]->merge(R[0][1], R[1][0], R[1][1]);
	x[1] = R1->multipleLeft(L1);

	shared_ptr<AbstractMatrix> L2 = L[0][0]->merge(L[0][1], L[1][0], L[1][1]);
	shared_ptr<AbstractMatrix> R2 = R[0][2]->subtract(R[2][2])->merge(R[0][3]->subtract(R[2][3]), R[1][2]->subtract(R[3][2]), R[1][3]->subtract(R[3][3]));
	x[2] = R2->multipleLeft(L2);

	shared_ptr<AbstractMatrix> L3 = L[2][2]->merge(L[2][3], L[3][2], L[3][3]);
	shared_ptr<AbstractMatrix> R3 = R[2][0]->subtract(R[0][0])->merge(R[2][1]->subtract(R[0][1]), R[3][0]->subtract(R[1][0]), R[3][1]->subtract(R[1][1]));
	x[3] = R3->multipleLeft(L3);

	shared_ptr<AbstractMatrix> L4 = L[0][0]->add(L[0][2])->merge(L[0][1]->add(L[0][3]), L[1][0]->add(L[1][2]), L[1][1]->add(L[1][3]));
	shared_ptr<AbstractMatrix> R4 = R[2][2]->merge(R[2][3], R[3][2], R[3][3]);
	x[4] = R4->multipleLeft(L4);

	shared_ptr<AbstractMatrix> L5 = L[2][0]->subtract(L[0][0])->merge(L[2][1]->subtract(L[0][1]), L[3][0]->subtract(L[1][0]), L[3][1]->subtract(L[1][1]));
	shared_ptr<AbstractMatrix> R5 = R[0][0]->add(R[0][2])->merge(R[0][1]->add(R[0][3]), R[1][0]->add(R[1][2]), R[1][1]->add(R[1][3]));
	x[5] = R5->multipleLeft(L5);

	shared_ptr<AbstractMatrix> L6 = L[0][2]->subtract(L[2][2])->merge(L[0][3]->subtract(L[2][3]), L[1][2]->subtract(L[3][2]), L[1][3]->subtract(L[3][3]));
	shared_ptr<AbstractMatrix> R6 = R[2][0]->add(R[2][2])->merge(R[2][1]->add(R[2][3]), R[3][0]->add(R[3][2]), R[3][1]->add(R[3][3]));
	x[6] = R6->multipleLeft(L6);

	shared_ptr<AbstractMatrix> y[7][2][2];

	for (size_t k = 0; k < 7; k++)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				y[k][i][j] = x[k]->submatrixExpand(rsz * i, rsz * (i + 1), msz * j, msz * (j + 1));
			}
		}
	}

	shared_ptr<AbstractMatrix> A[4][4];

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			A[i][j] = y[0][i][j]->add(y[3][i][j])->subtract(y[4][i][j])->add(y[6][i][j]);
			A[i][j + 2] = y[2][i][j]->add(y[4][i][j]);
			A[i + 2][j] = y[1][i][j]->add(y[3][i][j]);
			A[i + 2][j + 2] = y[0][i][j]->subtract(y[1][i][j])->add(y[2][i][j])->add(y[5][i][j]);
		}
	}

	shared_ptr<AbstractMatrix>B[2][2];
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			B[i][j] = A[i * 2][j * 2]->merge(A[i * 2][j * 2 + 1], A[i * 2 + 1][j * 2], A[i * 2 + 1][j * 2 + 1]);
		}
	}

	result = B[0][0]->merge(B[0][1], B[1][0], B[1][1]);

	return result->submatrixShrink(0, m->getRowSize(), 0, columnSize);

}

shared_ptr<AbstractMatrix>Matrix::multipleRight(shared_ptr<AbstractMatrix>m)
{
	shared_ptr<AbstractMatrix> tmp = m->multipleLeft(shared_ptr<AbstractMatrix>(getSharedPtr()));
	return tmp;
}

shared_ptr<AbstractMatrix>Matrix::map(double func(double))
{

	shared_ptr<AbstractMatrix> result(new Matrix(this->rowSize, this->columnSize));
	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++) {
			result->setValue(i, j, func(this->getValue(i, j)));
		}
	}
	return result;

}

double Matrix::convolve(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix>m)
{

	if (!bounded(top, left) || !bounded(bottom - 1, right - 1))
		return 0;
	if ((top - bottom != m->getRowSize()) || (right - left != m->getColumnSize()))
		return 0;

	double result = 0;
	for (size_t i = 0; i < m->getRowSize(); i++)
	{
		for (size_t j = 0; j < m->getColumnSize(); j++)
		{
			result += m->getValue(i, j) * this->getValue(top + i, left + j);
		}
	}

	return result;

}

shared_ptr<AbstractMatrix>Matrix::m2vByColumn()
{
	size_t unit = this->getRowSize() * this->getColumnSize();
	shared_ptr<AbstractMatrix> m(new Matrix(unit, 1));

	for (size_t i = 0; i < unit; i++)
	{
		m->setValue(i, 0, this->getValue(i % this->getRowSize(), i / this->getRowSize()));
	}

	return m;
}

shared_ptr<AbstractMatrix>Matrix::v2mByColomn(size_t size)
{
	size_t rowSize = size;
	size_t columnSize = this->getRowSize() / size;
	shared_ptr<AbstractMatrix> m(new Matrix(rowSize, columnSize));
	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			m->setValue(i, j, this->getValue(j * rowSize + i, 0));
		}
	}
	return m;
}

bool Matrix::equals(shared_ptr<AbstractMatrix> m)
{

	if (rowSize != m->getRowSize()
		|| columnSize != m->getColumnSize())
		return false;

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			if (matrix[i][j] != m->getValue(i, j))
				return false;
		}
	}

	return true;

}

shared_ptr<AbstractMatrix>Matrix::clone()
{

	shared_ptr<AbstractMatrix> result(new Matrix(rowSize, columnSize));
	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			result->setValue(i, j, matrix[i][j]);
		}
	}

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

shared_ptr<AbstractMatrix> Matrix::add_inplace(double x)
{

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			this->add_inplace(i, j, x);
		}
	}

	return shared_from_this();

}

shared_ptr<AbstractMatrix> Matrix::add_inplace(shared_ptr<AbstractMatrix> m)
{
	if (m->getColumnSize() != columnSize || m->getRowSize() != rowSize){
		cerr << "add(shared_ptr<AbstractMatrix> m) does not match!" << endl;
		return shared_from_this();
	}

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			this->add_inplace(i, j, m->getValue(i, j));
		}
	}

	return shared_from_this();

}

shared_ptr<AbstractMatrix> Matrix::add_inplace(int i, int j, double value) {

	if (!bounded(i, j))
	{
		cerr << "add(int i, int j, double value) out of bound!" << endl;
		return shared_from_this();
	}
	this->setValue(i, j, matrix[i][j] + value);

	return shared_from_this();

};

shared_ptr<AbstractMatrix> Matrix::add_inplace(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m)
{

	if (!bounded(top, left) || !bounded(bottom - 1, right - 1)){
		cerr << "add(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) out of bound!" << endl;
		return shared_from_this();
	}
	if ((bottom - top != m->getRowSize()) || (right - left != m->getColumnSize())){
		cerr << "add(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) out of bound!" << endl;
		return shared_from_this();
	}

	for (size_t i = 0; i < m->getRowSize(); i++)
	{
		for (size_t j = 0; j < m->getColumnSize(); j++)
		{
			this->add_inplace(top + i, left + j, m->getValue(i, j));
		}
	}

	return shared_from_this();

}

shared_ptr<AbstractMatrix> Matrix::subtract_inplace(double x)
{
	return add(-x);
}

shared_ptr<AbstractMatrix> Matrix::subtract_inplace(shared_ptr<AbstractMatrix> m)
{
	if (m->getColumnSize() != columnSize || m->getRowSize() != rowSize){
		cerr << "subtract(shared_ptr<AbstractMatrix>m) does not match!" << endl;
		return shared_from_this();
	}

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			this->setValue(i, j, matrix[i][j] - m->getValue(i, j));
		}
	}

	return shared_from_this();

}

shared_ptr<AbstractMatrix> Matrix::multiple_inplace(double x)
{

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			this->setValue(i, j, matrix[i][j] * x);
		}
	}

	return shared_from_this();

}

shared_ptr<AbstractMatrix> Matrix::multiple_inplace(shared_ptr<AbstractMatrix> m)
{
	if (m->getColumnSize() != columnSize || m->getRowSize() != rowSize){
		cerr << "mutiple(shared_ptr<AbstractMatrix>m) does not match!" << endl;
		return shared_from_this();
	}

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++)
		{
			this->setValue(i, j, matrix[i][j] * m->getValue(i, j));
		}
	}
	return shared_from_this();
}

shared_ptr<AbstractMatrix>Matrix::map_inplace(double func(double))
{

	for (size_t i = 0; i < rowSize; i++)
	{
		for (size_t j = 0; j < columnSize; j++) {
			this->setValue(i, j, func(this->getValue(i, j)));
		}
	}
	return shared_from_this();

}