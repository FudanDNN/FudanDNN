#include "NonLinearLayer.h"
NonLinearLayer::NonLinearLayer(){
	
}

void NonLinearLayer::readSelf(string filename)
{
	ifstream fin(filename, ios::in);

	fin >> visualUnit;
	hiddenUnit = visualUnit;

	fin.close();
}

void NonLinearLayer::writeSelf(string filename)
{
	ofstream fout(filename, ios::out);

	fout << visualUnit << endl;

	fout.close();
}
