#pragma once
#include "utils.h"
#include "AbstractMatrix.h"
#include "Sample.h"
#include "XMLDataParser.h"

class Input{
public:
	Input(string xmlFile,int id);
	virtual shared_ptr<Sample> getNextSample();
	size_t getId();
	void setGradient(shared_ptr<AbstractMatrix> gradient);
	size_t getInputLength();
	void setNodeId(size_t id);
	size_t getNodeId();
	void backToHead();
	size_t getSampleNum();
	bool isEnd();

protected:
	string xmlFile;
	size_t id;
	shared_ptr<XMLDataParser> parser;
	size_t nodeId;

};