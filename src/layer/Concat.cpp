#include "layer\ConcatLayer.h"

ConcatLayer::ConcatLayer(size_t visualRow, size_t visualColumn, size_t visualSize){
	this->visualRow = visualRow;
	this->visualColumn = visualColumn;
	this->visualSize = visualSize;
	this->hiddenSize = 1;
	this->hiddenColumn = 1;
	this->hiddenRow = visualRow*visualColumn*visualSize;
	instance = MatrixPool::getInstance();
	Layer::init();
}

string ConcatLayer::getNetworkName(){
	return "concat";
}

void ConcatLayer::compute(){
	hiddenValue[0]->setValues(0);
	size_t square = this->visualRow*this->visualColumn;
	shared_ptr<Matrix> temp(new Matrix(0, 1));
	for (int i = 0; i < visualSize; i++){
		temp = temp->mergeRow(visualValue[i]->reshape(square, 1));
	}
	hiddenValue[0]->addi(temp);
	hiddenGradient[0]->setValues(0);

}

void ConcatLayer::calculate(){
	hiddenValue[0]->setValues(0);
	size_t square = this->visualRow*this->visualColumn;
	shared_ptr<Matrix> temp(new Matrix(0, 1));
	for (int i = 0; i < visualSize; i++){
		temp = temp->mergeRow(visualValue[i]->reshape(square, 1));
	}
	hiddenValue[0]->addi(temp);
	hiddenGradient[0]->setValues(0);
}

void ConcatLayer::gradient(){
	size_t index = 0;
	size_t square = this->visualRow*this->visualColumn;
	for (int i = 0; i < visualSize; i++){
		visualGradient[i]->setValues(0);
		shared_ptr<Matrix> temp = hiddenGradient[0]->submatrix(index, index + square, 0, 1);
		//warning!!!!!!!!!!!!!!!!!!!!!!!!
		visualGradient[i] = temp->reshape(this->visualRow, this->visualColumn);
		visualValue[i]->setValues(0);
		index = index + square;
	}
}

void ConcatLayer::writeSelf(string){

}
void ConcatLayer::readSelf(string) {

}