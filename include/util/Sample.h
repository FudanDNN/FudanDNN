#ifndef __FUDANDNN_SAMPLE_H
#define __FUDANDNN_SAMPLE_H
#include "utils.h"
#include "Matrix.h"

class Sample
{
private:
	shared_ptr<Matrix> input;
	shared_ptr<Matrix> output;
	string type;
	size_t iRowSize;
	size_t iColumnSize;
	size_t oRowSize;
	size_t oColumnSize;
public:
	Sample(shared_ptr<Matrix> input, shared_ptr<Matrix> output, string type,
		size_t iRowSize, size_t iColumnSize, size_t oRowSize, size_t oColumnSize);
	void setInput(shared_ptr<Matrix> input);
	void setOutput(shared_ptr<Matrix> output);
	void setType(string type);
	void setIRowSize(size_t iRowSize);
	void setIColumnSize(size_t iColumnSize);
	void setORowSize(size_t oRowSize);
	void setOColumnSize(size_t oColumnSize);
	shared_ptr<Matrix> getInput();
	shared_ptr<Matrix> getOutput();
	string getType();
	size_t getIRowSize();
	size_t getIColumnSize();
	size_t getORowSize();
	size_t getOColumnSize();

};


#endif