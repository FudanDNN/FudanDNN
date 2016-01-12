#include "util/Sample.h"

Sample::Sample(shared_ptr<Matrix> input, shared_ptr<Matrix> output, string type,
	size_t iRowSize, size_t iColumnSize, size_t oRowSize, size_t oColumnSize){
	this->input = input;
	this->output = output;
	this->type = type;
	this->iRowSize = iRowSize;
	this->iColumnSize = iColumnSize;
	this->oRowSize = oRowSize;
	this->oColumnSize = oColumnSize;
}

void Sample::setInput(shared_ptr<Matrix> input){
	this->input = input;
}

void Sample::setOutput(shared_ptr<Matrix> output){
	this->output = output;
}

void Sample::setType(string type){
	this->type = type;
}

void Sample::setIRowSize(size_t iRowSize){
	this->iRowSize = iRowSize;
}

void Sample::setIColumnSize(size_t iColumnSize){
	this->iColumnSize = iColumnSize;
}

void Sample::setORowSize(size_t oRowSize){
	this->oRowSize = oRowSize;
}

void Sample::setOColumnSize(size_t oColumnSize){
	this->oColumnSize = oColumnSize;
}

shared_ptr<Matrix> Sample::getInput(){
	return this->input;
}

shared_ptr<Matrix> Sample::getOutput(){
	return this->output;
}

string Sample::getType(){
	return this->type;
}

size_t Sample::getIRowSize(){
	return this->iRowSize;
}

size_t Sample::getIColumnSize(){
	return this->iColumnSize;
}

size_t Sample::getORowSize(){
	return this->oRowSize;
}

size_t Sample::getOColumnSize(){
	return this->oColumnSize;
}