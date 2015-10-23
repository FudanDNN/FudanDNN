#include "networkunitpool.h"
namespace NetworkUnitPool{
	int idCount = 0;
	std::vector<NetworkUnitBase *> networkUnits;
	int cnnSubnetIdCount = 0;
	std::vector<CNNSubnet*> cnnSubnets;

	void unregisterUnit(NetworkUnitBase * unit){
		if (unit == nullptr) return;
		//clean link
		for (auto iter = unit->getInputUnitList().begin(); iter != unit->getInputUnitList().end(); ++iter){
			(*iter)->removeOutputUnit(unit);
		}
		for (auto iter = unit->getOutputUnitList().begin(); iter != unit->getOutputUnitList().end(); ++iter){
			(*iter)->removeInputUnit(unit);
		}
		//clean from cnn subnet if it is a cnn unit
		if (unit->isCNNUnit(unit->getTypeId())){
			CNNUnitBase* cnnUnit = dynamic_cast<CNNUnitBase*> (unit);
			int subnetId=cnnUnit->getSubnetId();
			for (auto iter = cnnSubnets.begin(); iter != cnnSubnets.end(); ++iter){
				if ((*iter)->getId() == subnetId){
					(*iter)->unregisterUnit(cnnUnit);
					break;
				}
			}
		}
		//remove from pool
		for (auto iter = networkUnits.begin(); iter != networkUnits.end(); ++iter){
			if (*iter == unit){
				networkUnits.erase(iter);
				break;
			}
		}

		delete unit;
	}

	void registerUnit(NetworkUnitBase* unit){
		if (unit == nullptr) return;
		// check if registered
		for (auto iter = networkUnits.cbegin(); iter != networkUnits.cend(); ++iter){
			if (*iter == unit){
				return;
			}
		}
		networkUnits.push_back(unit);
	}

	void addLink(NetworkUnitBase* start, NetworkUnitBase *end){
		if (start == nullptr || end == nullptr) return;
		start->addOutputUnit(end);
		end->addInputUnit(start);
	}

	NetworkUnitBase * createNetworkUnit(DiagramItem* diagramItem, NetworkUnitBase::NetworkUnitType typeId){
		NetworkUnitBase *t=nullptr;
		switch (typeId){
			case NetworkUnitBase::INPUT_UNIT:
				t = new NetworkUnitBase(diagramItem, typeId);
				t->setTitle("Input Layer");
				t->setHyperParameterControl(new InputLayerHyperParameterControl());
				break;
			case NetworkUnitBase::LINEAR_UNIT:
				t = new NetworkUnitBase(diagramItem, typeId);
				t->setTitle("Linear Layer");
				t->setHyperParameterControl(new LinearLayerHyperParameterControl());
				break;
			case NetworkUnitBase::NONLINEAR_UNIT:
				t = new NetworkUnitBase(diagramItem, typeId);
                t->setHyperParameterControl(new NonLinearLayerHyperParameterControl());
				t->setTitle("Nonlinear Layer");
				break;
			case NetworkUnitBase::CNN_INPUT_UNIT:
				t = new CNNUnitBase(diagramItem, typeId);
                t->setHyperParameterControl(new CNNIOHyperParameterControl(0));
				t->setTitle("CNN Input");
				break;
			case NetworkUnitBase::CNN_OUTPUT_UNIT:
				t = new CNNUnitBase(diagramItem, typeId);
                t->setHyperParameterControl(new CNNIOHyperParameterControl(0));
				t->setTitle("CNN Output");
				break;
			case NetworkUnitBase::CNN_CONVOLUTIONAL_UNIT:
				t = new CNNUnitBase(diagramItem, typeId);
                t->setHyperParameterControl(new CNNConvolutionLayerHyperParameterControl(0));
				t->setTitle("CNN Convolutional Layer");
				break;
			case NetworkUnitBase::CNN_MAXPOOLING_UNIT:
				t = new CNNUnitBase(diagramItem, typeId);
                t->setHyperParameterControl(new CNNMaxPoolingLayerHyperParameterControl(0));
				t->setTitle("CNN Max Pooling Layer");
				break;
            case NetworkUnitBase::CNN_NONLINEAR_UNIT:
                t = new CNNUnitBase(diagramItem, typeId);
                t->setHyperParameterControl(new CNNNonLinearLayerHyperParameterControl(0));
                t->setTitle("CNN Nonlinear Layer");
                break;
		}
		registerUnit(t);
		return t;
	}
	void outputNetworkGraph(std::ostream& out){
		for (auto iter = networkUnits.begin(); iter != networkUnits.end(); ++iter){
			NetworkUnitBase* item = *iter;
			out << item->getUnitId() << " Type=" << item->getTypeId() << std::endl;
		}
		for (auto iter = networkUnits.begin(); iter != networkUnits.end(); ++iter){
			NetworkUnitBase* item = *iter;
			for (auto iter2 = item->getOutputUnitList().begin(); iter2 != item->getOutputUnitList().end(); ++iter2){
				NetworkUnitBase* toItem = *iter2;
				out << item->getUnitId() << " " << toItem->getUnitId() << std::endl;
			}
		}
	}
	std::vector<int> sliceLayers(){
		int unitCount = networkUnits.size();
		std::vector<int> ret(unitCount);
		std::vector<int> inCount(unitCount);
		std::map<int, int> idToPos;
		for (int i = 0; i < unitCount; ++i){
			NetworkUnitBase* unit = networkUnits[i];
			inCount[i] = unit->getInputUnitList().size();;
			idToPos[unit->getUnitId()] = i;
		}
		std::queue<int> uQ;
		for (int i = 0; i < unitCount; ++i){
			if (inCount[i] == 0) uQ.push(i);
		}
		while (!uQ.empty()){
			int now = uQ.front();
			uQ.pop();
			NetworkUnitBase* unit = networkUnits[now];
			for (auto iter = unit->getOutputUnitList().begin(); iter != unit->getOutputUnitList().end(); iter++){
				int to = idToPos[(*iter)->getUnitId()];
				if (--inCount[to] == 0) {
					uQ.push(to);
					ret[to] = ret[now] + 1;
				}
			}
		}
		return ret;
	}
	CNNSubnet * createSubnet(){
		CNNSubnet *ret = new CNNSubnet(++cnnSubnetIdCount);
        ret->setHyperParameterControl(new CNNSubnetHyperParameterControl());
        ret->getHyperParameterControl()->setCnnId(cnnSubnetIdCount);
		cnnSubnets.push_back(ret);
		return ret;
	}

}