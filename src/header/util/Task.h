#ifndef __FUDANDNN_INPUT_H
#define __FUDANDNN_INPUT_H
#include "utils.h"
#include "Matrix.h"
#include "Sample.h"

class Task{
public:
	Task(string file, int id);
	virtual shared_ptr<Sample> getNextSample(){ return nullptr; };
	virtual void update(){};
	virtual size_t getSampleNum(){ return 0; };
	virtual bool isEnd(){ return false; };
	size_t getId();
	void setGradient(shared_ptr<Matrix> gradient);

protected:
	string file;
	size_t id;
	shared_ptr<Sample> currentSample;
	shared_ptr<Matrix> gradient;

};
#endif