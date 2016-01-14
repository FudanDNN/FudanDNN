#include "layer\LinearLayer.h"

LinearLayer::LinearLayer(size_t visualUnit, size_t hiddenUnit, size_t initScheme,shared_ptr<Solver> solver)
{
	this->visualColumn = 1;
	this->visualRow = visualUnit;
	this->hiddenColumn = 1;
	this->hiddenRow = hiddenRow;
	this->solver = solver;
	this->initialization(initScheme);
}

void LinearLayer::initialization(int init_scheme)
{

}

void LinearLayer::compute()
{

}

void LinearLayer::calculate()
{

}

void LinearLayer::gradient()
{

}

void LinearLayer::writeSelf(string filename)
{

}

void LinearLayer::readSelf(string filename)
{

}
string LinearLayer::getNetworkName(){
	return "LinearLayer";
}