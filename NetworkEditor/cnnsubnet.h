#pragma once
#include <vector>
#include "cnnunitbase.h"
#include "hyperparametercontrol.h"
class CNNSubnet
{
private:
	std::vector<CNNUnitBase* > unitsList;
	CNNSubnetHyperParameterControl *control;
	int id;
public:
	CNNSubnet(int id);
	~CNNSubnet();
	int getId() const{ return id; }
	void registerUnit(CNNUnitBase*);
	void unregisterUnit(CNNUnitBase*);
	const std::vector<CNNUnitBase*>& getUnitsList(){ return unitsList; };
	CNNSubnetHyperParameterControl* getHyperParameterControl(){ return this->control; }
	void setHyperParameterControl(CNNSubnetHyperParameterControl* control){ this->control = control; }
};

