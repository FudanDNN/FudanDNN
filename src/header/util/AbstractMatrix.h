#ifndef __FUDANDNN_ABSTRACTMATRIX_H
#define __FUDANDNN_ABSTRACTMATRIX_H
#include "utils.h"

struct MaxData
{
	double value;
	size_t pos_i;
	size_t pos_j;
};

class AbstractMatrix : public enable_shared_from_this<AbstractMatrix>
{
public:
	void setValue(size_t i, size_t j, double value);
	double getValue(size_t i, size_t j);
	void setRow(size_t i, vector<double> row);
	vector<double> getRow(size_t i);
	void setColumn(size_t j, vector<double> row);
	vector<double> getColumn(size_t j);
	size_t getRowSize();
	size_t getColumnSize();

	// whether (i, j) is in the matrix
	bool bounded(int i, int j);

	// initialize matrix with given bound
	virtual void initializeValue(double lowerBound, double upperBound) {};

	// randomly initialize matrix with 0 and 1
	virtual void initializeBinaryValue(double p) {};
	virtual double sum() { return 0; };
	virtual double max() { return 0; }

	// max value of sub matrix
	virtual shared_ptr<MaxData> max(int top, int bottom, int left, int right) { return nullptr; };
	virtual double min() { return 0; }
	virtual double norm1() { return 0; };
	virtual double norm2() { return 0; };
	virtual double trace() { return 0; };
	virtual shared_ptr<AbstractMatrix> transpose() { return shared_ptr<AbstractMatrix>(nullptr); };
	virtual shared_ptr<AbstractMatrix> submatrix(int top, int bottom, int left, int right) { return nullptr; };

	// get sub matrix 
	virtual shared_ptr<AbstractMatrix> submatrixShrink(int top, int bottom, int left, int right) { return nullptr; };
	virtual shared_ptr<AbstractMatrix> submatrixExpand(int top, int bottom, int left, int right) { return nullptr; };

	// split matrix, [0, row_index), [row_index, row_size)
	virtual vector<shared_ptr<AbstractMatrix>> splitRow(size_t rowIndex) { return *new vector<shared_ptr<AbstractMatrix>>(); };
	// split matrix,[0, collumn_index), [collumn_index, collumn_size)
	virtual vector<shared_ptr<AbstractMatrix>> splitColumn(size_t columnIndex) { return *new vector<shared_ptr<AbstractMatrix>>(); };
	// merge matrix
	virtual shared_ptr<AbstractMatrix> merge(shared_ptr<AbstractMatrix> rightTop, shared_ptr<AbstractMatrix> leftBottom,
		shared_ptr<AbstractMatrix> rightBottom) {
		return nullptr;
	};
	virtual shared_ptr<AbstractMatrix> mergeRow(shared_ptr<AbstractMatrix> bottom) { return nullptr; };
	virtual shared_ptr<AbstractMatrix> mergeColumn(shared_ptr<AbstractMatrix> right) { return nullptr; };

	virtual shared_ptr<AbstractMatrix> add(double x) { return nullptr; };
	virtual shared_ptr<AbstractMatrix> add(shared_ptr<AbstractMatrix> m) { return nullptr; };
	virtual shared_ptr<AbstractMatrix> add(int i, int j, double value) { return nullptr; };
	virtual shared_ptr<AbstractMatrix> add(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) { return nullptr; };
	virtual shared_ptr<AbstractMatrix> subtract(double x) { return nullptr; };
	virtual shared_ptr<AbstractMatrix> subtract(shared_ptr<AbstractMatrix> m) { return nullptr; };
	virtual shared_ptr<AbstractMatrix> multiple(double x) { return nullptr; };
	virtual shared_ptr<AbstractMatrix> multiple(shared_ptr<AbstractMatrix> m) { return nullptr; };

	// m * this
	virtual shared_ptr<AbstractMatrix> multipleLeft(shared_ptr<AbstractMatrix> m) { return nullptr; };
	// this * m
	virtual shared_ptr<AbstractMatrix> multipleRight(shared_ptr<AbstractMatrix> m) { return nullptr; };
	// execute func for all elements
	virtual shared_ptr<AbstractMatrix> map(double func(double)) { return nullptr; };

	//convolve sub matrix with m
	virtual double convolve(size_t top, size_t bottom, size_t left, size_t right, shared_ptr<AbstractMatrix> m) { return 0; }
	virtual shared_ptr<AbstractMatrix> m2vByColumn() { return nullptr; }
	virtual shared_ptr<AbstractMatrix> v2mByColomn(size_t size) { return nullptr; }


	virtual bool equals(shared_ptr<AbstractMatrix> m) { return false; };
	virtual shared_ptr<AbstractMatrix> clone() { return nullptr; };
	virtual void print() {};
	virtual ~AbstractMatrix() {};
	virtual shared_ptr<AbstractMatrix> getSharedPtr(){
		return shared_from_this();
	}

	virtual shared_ptr<AbstractMatrix> add_inplace(double x) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> add_inplace(shared_ptr<AbstractMatrix> m) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> add_inplace(int, int, double) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> add_inplace(size_t, size_t, size_t, size_t, shared_ptr<AbstractMatrix>) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> subtract_inplace(double) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> subtract_inplace(shared_ptr<AbstractMatrix>) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> multiple_inplace(double) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> multiple_inplace(shared_ptr<AbstractMatrix>) { return shared_from_this(); };
	virtual shared_ptr<AbstractMatrix> map_inplace(double func(double)) { return shared_from_this(); };

protected:
	size_t rowSize;
	size_t columnSize;
	double **matrix;

	virtual void initialize() {};

};


#endif