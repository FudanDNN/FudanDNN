#include"../header/util/XMLTask.h"

XMLTask::XMLTask(string file, int id) :Task(file,id){
	parser = shared_ptr<XMLDataParser>(new XMLDataParser(file));
}

shared_ptr<Sample> XMLTask::getNextSample(){
	return parser->getNextSample();
}


size_t XMLTask::getSampleNum(){
	return parser->getSampleNum();
}

bool XMLTask::isEnd(){
	return parser->isEnd();
}
