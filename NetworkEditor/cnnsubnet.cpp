#include "cnnsubnet.h"


CNNSubnet::CNNSubnet(int id) : id(id), control(nullptr)
{
}


CNNSubnet::~CNNSubnet()
{
}

void CNNSubnet::registerUnit(CNNUnitBase* node){
	if (node->bindCNNSubnetId != -1) return;
	for (auto iter = unitsList.cbegin(); iter != unitsList.cend(); ++iter){
		if (*iter == node) return;
	}
	
	this->unitsList.push_back(node);
	node->bindCNNSubnetId = this->id;
}

void CNNSubnet::unregisterUnit(CNNUnitBase* node){
	if (node->bindCNNSubnetId != this->id ) return;
	node->bindCNNSubnetId = -1;
	for (auto iter = unitsList.begin(); iter != unitsList.end(); ++iter){
		if (*iter == node){
			unitsList.erase(iter);
			return;
		}
	}
}
