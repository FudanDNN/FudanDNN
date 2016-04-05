#include "networkunitbase.h"
#include "networkunitpool.h"
NetworkUnitBase::NetworkUnitBase(DiagramItem* diagramItem, NetworkUnitType typeId)
	:unitId(NetworkUnitPool::idCount++), icon(nullptr), title("Network Unit"), diagramItem(diagramItem), typeId(typeId), hyperParameterControl(nullptr)
{
    diagramItem->networkUnitPtr = this;
}
NetworkUnitBase::~NetworkUnitBase(){

}

void NetworkUnitBase::addInputUnit(NetworkUnitBase *unit){
    //check if existed
    for (auto iter=this->inputUnit.begin();iter!=this->inputUnit.end();++iter){
        if (*iter == unit) return ;
    }
    this->inputUnit.push_back(unit);
}

void NetworkUnitBase::addOutputUnit(NetworkUnitBase *unit){
    //check if existed
    for (auto iter=this->outputUnit.begin();iter!=this->outputUnit.end();++iter){
        if (*iter == unit) return ;
    }
    this->outputUnit.push_back(unit);
}

void NetworkUnitBase::removeInputUnit(NetworkUnitBase *unit){
	for (auto iter = this->inputUnit.begin(); iter != this->inputUnit.end(); ++iter){
		if (*iter == unit) {
			inputUnit.erase(iter);
			return;
		}
	}
}

void NetworkUnitBase::removeOutputUnit(NetworkUnitBase *unit){
	for (auto iter = this->outputUnit.begin(); iter != this->outputUnit.end(); ++iter){
		if (*iter == unit) {
			outputUnit.erase(iter);
			return;
		}
	}
}

