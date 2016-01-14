#include "util/XMLInput.h"

XMLInput::XMLInput(string file, int id) :Input(file,id){
	parser = shared_ptr<XMLDataParser>(new XMLDataParser(file));
}

shared_ptr<Sample> XMLInput::getNextSample(){
	return parser->getNextSample();
}


size_t XMLInput::getSampleNum(){
	return parser->getSampleNum();
}

bool XMLInput::isEnd(){
	return parser->isEnd();
}
