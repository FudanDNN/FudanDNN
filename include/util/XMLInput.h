#ifndef __FUDANDNN_XMLTASK_H
#define __FUDANDNN_XMLTASK_H

#include "Input.h"
#include "XMLDataParser.h"

class XMLInput : public Input{
public:
	XMLInput(string file, int id);
	shared_ptr<Sample> getNextSample();
	size_t getSampleNum();
	bool isEnd();
protected:
	size_t index;
	shared_ptr<XMLDataParser> parser;
	vector<shared_ptr<Sample>> samples;
};
#endif