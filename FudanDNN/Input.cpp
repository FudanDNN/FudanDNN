#include "Input.h"

Input::Input(string xmlFile, int id){
	parser = shared_ptr<XMLDataParser>(new XMLDataParser(xmlFile));
	this->xmlFile = xmlFile;
	this->id = id;
}

shared_ptr<Sample> Input::getNextSample(){
	return parser->getNextSample();
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

size_t Input::getInputLength(){
	return 1;
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