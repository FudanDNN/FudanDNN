#include "util/Input.h"

Input::Input(string file,size_t id){
	this->file = file;
	this->id = id;
}

size_t Input::getId(){
	return this->id;
}

void Input::setGradient(shared_ptr<Matrix> gradient){
	this->gradient = gradient;
}

void Input::setId(size_t id)
{
	this->id = id;
}

size_t Input::getIdInNetwork(){
	return this->idInNetwork;
}

void Input::setIdInNetwork(size_t idInNetwork){
	this->idInNetwork = idInNetwork;
}