#ifndef __FUDANDNN_XMLTASK_H
#define __FUDANDNN_XMLTASK_H

#include "Task.h"
#include "XMLDataParser.h"

class XMLTask : public Task{
public:
	XMLTask(string file, int id);
	shared_ptr<Sample> getNextSample();
	size_t getSampleNum();
	bool isEnd();
protected:
	shared_ptr<XMLDataParser> parser;
};
#endif