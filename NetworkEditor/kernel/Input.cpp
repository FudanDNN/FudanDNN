#include "Input.h"

Input::Input(string xmlFile, int id){
	parser = shared_ptr<XMLDataParser>(new XMLDataParser(xmlFile));
	this->xmlFile = xmlFile;
	this->id = id;
}

shared_ptr<Sample> Input::getNextSample(){
	return parser->getNextSample();
}
shared_ptr<TrainingSample> Input::getNextTrainingSample(){
	return parser->getNextTrainingSample();
}

size_t Input::getId(){
	return this->id;
}

void Input::setGradient(shared_ptr<AbstractMatrix> gradient){

}

bool Input::isEnd(){
	return parser->isEnd();
}

size_t Input::getSampleNum(){
	return parser->getSampleNum();
}

size_t Input::getInputRow(){
	return parser->getIRowSize();
}

void Input::setNodeId(size_t nodeId){
	this->nodeId = nodeId;
}

size_t Input::getNodeId(){
	return this->nodeId;
}

void Input::backToHead(){
	parser->backToHead();
}