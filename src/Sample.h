#pragma once
#include "utils.h"
#include "AbstractMatrix.h"

class Sample
{
private:
	shared_ptr<AbstractMatrix> input;
	shared_ptr<AbstractMatrix> output;
	string type;
	size_t iRowSize;
	size_t iColumnSize;
	size_t oRowSize;
	size_t oColumnSize;
public:
	Sample(shared_ptr<AbstractMatrix> input, shared_ptr<AbstractMatrix> output, string type,
		size_t iRowSize, size_t iColumnSize, size_t oRowSize, size_t oColumnSize);
	void setInput(shared_ptr<AbstractMatrix> input);
	void setOutput(shared_ptr<AbstractMatrix> output);
	void setType(string type);
	void setIRowSize(size_t iRowSize);
	void setIColumnSize(size_t iColumnSize);
	void setORowSize(size_t oRowSize);
	void setOColumnSize(size_t oColumnSize);
	shared_ptr<AbstractMatrix> getInput();
	shared_ptr<AbstractMatrix> getOutput();
	string getType();
	size_t getIRowSize();
	size_t getIColumnSize();
	size_t getORowSize();
	size_t getOColumnSize();

};

