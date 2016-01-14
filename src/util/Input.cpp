#include "util/Input.h"

Input::Input(string file, int id){
	this->file = file;
	this->id = id;
}

size_t Input::getId(){
	return this->id;
}

void Input::setGradient(shared_ptr<Matrix> gradient){
	this->gradient = gradient;
}
