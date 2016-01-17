#include "layer/NonlinearLayer.h"

NonlinearLayer::NonlinearLayer(shared_ptr<Function> function, size_t type, size_t rowSize, size_t columnSize, size_t featureMap)
{
	this->fun = function;
	this->type = type;
	this->visualRow = this->hiddenRow = rowSize;
	this->visualColumn = this->hiddenColumn = columnSize;
	this->visualSize = this->hiddenSize = featureMap;
}
NonlinearLayer::NonlinearLayer(shared_ptr<Function> function, size_t type, size_t rowSize):
NonlinearLayer(function, type,rowSize,1,1)
{

}
string NonlinearLayer::getNetworkName()
{
	switch (type){
	case SIGMOID:
		return "sigmoid";
	}
}

void NonlinearLayer::readSelf(string)
{}

void NonlinearLayer::writeSelf(string)
{}

void NonlinearLayer::compute()
{
	for (size_t f = 0; f < visualSize; f++){
		shared_ptr<Matrix> temp(new Matrix(visualRow, visualColumn));
		for (size_t i = 0; i < visualRow; i++){
			for (size_t j = 0; j < visualColumn; j++){
				temp->setValue(i, j, fun->getValue(visualValue[f]->getValue(i, j)));
			}
		}
		hiddenValue.push_back(temp);
	}

}

void NonlinearLayer::calculate()
{
	for (size_t f = 0; f < visualSize; f++){
		shared_ptr<Matrix> temp(new Matrix(visualRow, visualColumn));
		for (size_t i = 0; i < visualRow; i++){
			for (size_t j = 0; j < visualColumn; j++){
				temp->setValue(i, j, fun->getValue(visualValue[f]->getValue(i, j)));
			}
		}
		hiddenValue.push_back(temp);
	}
	/*cout << "nonlinearLayer:" << this->getVisualRow() << endl;
	cout << "visualValue" << endl;
	visualValue[0]->print();
	cout << "hiddenValue" << endl;
	hiddenValue[0]->print();*/
}

void NonlinearLayer::gradient()
{
	for (size_t f = 0; f < visualSize; f++){
		shared_ptr<Matrix> temp(new Matrix(visualRow, visualColumn));
		for (size_t i = 0; i < visualRow; i++){
			for (size_t j = 0; j < visualColumn; j++){
				temp->setValue(i, j, hiddenGradient[f]->getValue(i, j) * fun->getDerivate(visualValue[f]->getValue(i, j)));
			}
		}
		this->visualGradient.push_back(temp);
	}
	hiddenGradient.clear();

	cout << "nonlinearLayer:" << this->getVisualRow() << endl;
	cout << "hiddenGradient" << endl;
	hiddenGradient[0]->print();
	cout << "visualGradient" << endl;
	visualGradient[0]->print();
}