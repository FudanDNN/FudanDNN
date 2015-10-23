#include "cnnunitbase.h"


CNNUnitBase::CNNUnitBase(DiagramItem *ditem, NetworkUnitType typeId):
	NetworkUnitBase(ditem, typeId), bindCNNSubnetId(-1)
{
}


CNNUnitBase::~CNNUnitBase()
{
}

bool CNNUnitBase::isAvailbleInput(NetworkUnitBase* node){
	if (this->getTypeId() == NetworkUnitType::CNN_OUTPUT_UNIT) {
		return (dynamic_cast<CNNUnitBase*>(node) == nullptr);
	}
	CNNUnitBase *target = dynamic_cast<CNNUnitBase*>(node);
	if (target != nullptr){
		return (target->getSubnetId() == this->bindCNNSubnetId);
	}
	else {
		return false;
	}
}

bool CNNUnitBase::isAvailbleOutput(NetworkUnitBase* node){
	if (this->getTypeId() == NetworkUnitType::CNN_INPUT_UNIT) {
		return (dynamic_cast<CNNUnitBase*>(node) == nullptr);
	}
	CNNUnitBase *target = dynamic_cast<CNNUnitBase*>(node);
	if (target != nullptr){
		return (target->getSubnetId() == this->bindCNNSubnetId);
	}
	else {
		return false;
	}
}
