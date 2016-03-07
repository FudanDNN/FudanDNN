#include "util/XMLInput.h"

XMLInput::XMLInput(string file, int id) :Input(file,id){
	parser = shared_ptr<XMLDataParser>(new XMLDataParser(file));
	cout << "read file to memory..." << endl;
	this->samples = parser->getAllSamples();
	cout << "read file done" << endl;
	index = -1;
}

shared_ptr<Sample> XMLInput::getNextSample(){
	index++;
	index = index % this->samples.size();
	//cout << index << " " << getSampleNum() << endl;
	return samples.at(index);
}


size_t XMLInput::getSampleNum(){
	return this->samples.size();
}

bool XMLInput::isEnd(){
	return index==(this->samples.size()-1);
}
