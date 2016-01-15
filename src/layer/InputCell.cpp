#include "layer\InputCell.h"
InputCell::InputCell(size_t id){
	this->id = id;
}

void InputCell::setInputMat(shared_ptr<Matrix> inputMat){
	this->inputMat = inputMat;
}

void InputCell::setGradientMat(shared_ptr<Matrix> gradientMat){
	this->gradientMat = gradientMat;
}

shared_ptr<Matrix> InputCell::getInputMat(){
	return this->inputMat;
}

shared_ptr<Matrix> InputCell::getGradientMat(){
	return this->gradientMat;
}

size_t InputCell::getId(){
	return this->id;
}