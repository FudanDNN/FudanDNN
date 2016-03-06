#include "util/XMLInput.h"

XMLInput::XMLInput(string file, int id) :Input(file,id){
	parser = shared_ptr<XMLDataParser>(new XMLDataParser(file));
	this->samples = parser->getAllSamples();
	index = -1;
}

shared_ptr<Sample> XMLInput::getNextSample(){
	index++;
	index = index % 120;
	return samples.at(index);
}


size_t XMLInput::getSampleNum(){
	return this->samples.size();
}

bool XMLInput::isEnd(){
	return index==(this->samples.size()-1);
}
