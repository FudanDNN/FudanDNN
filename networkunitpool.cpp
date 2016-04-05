#include "networkunitpool.h"
#include "include/Model.h"
#include "hyperparametercontrol.h"
#include <set>
#include <map>
namespace NetworkUnitPool{
	int idCount = 0;
	std::vector<NetworkUnitBase *> networkUnits;
	void unregisterUnit(NetworkUnitBase * unit){
		if (unit == nullptr) return;
		//clean link
		for (auto iter = unit->getInputUnitList().begin(); iter != unit->getInputUnitList().end(); ++iter){
			(*iter)->removeOutputUnit(unit);
		}
		for (auto iter = unit->getOutputUnitList().begin(); iter != unit->getOutputUnitList().end(); ++iter){
			(*iter)->removeInputUnit(unit);
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
				t->setTitle("Linear");
				t->setHyperParameterControl(new LinearLayerHyperParameterControl());
				break;
			case NetworkUnitBase::NONLINEAR_UNIT:
				t = new NetworkUnitBase(diagramItem, typeId);
				t->setTitle("Nonlinear");
                t->setHyperParameterControl(new NonlinearHyperParameterControl());
				break;
            case NetworkUnitBase::CONVOLUTION_UNIT:
                t = new NetworkUnitBase(diagramItem, typeId);
                t->setTitle("Convolution");
                t->setHyperParameterControl(new ConvolutionLayerHyperParameterControl());
                break;
            case NetworkUnitBase::MAXPOOLING_UNIT:
                t = new NetworkUnitBase(diagramItem, typeId);
                t->setTitle("Max Pooling");
                t->setHyperParameterControl(new MaxPoolingHyperParameterControl());
                break;
            case NetworkUnitBase::CONCAT_UNIT:
                t = new NetworkUnitBase(diagramItem, typeId);
                t->setTitle("Concat");
                t->setHyperParameterControl(new ConcatLayerHyperParameterControl());
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

    void parseInformation(fudandnn::Model & model, shared_ptr<Solver> solver){
        std::map<int, int> idMap;
        std::set<int> inputNodes;
        for (auto iter = networkUnits.begin(); iter != networkUnits.end(); ++iter){
            NetworkUnitBase* item = *iter;
            HasSolver *hasSolverUnit = dynamic_cast<HasSolver*>(item->getHyperParameterControl());
            if (hasSolverUnit != nullptr){
                hasSolverUnit->setSolver(solver);
            }
            if (dynamic_cast<InputLayerHyperParameterControl*>(item->getHyperParameterControl())){
                inputNodes.insert(item->getUnitId());
            }
            idMap[item->getUnitId()] = item->getHyperParameterControl()->parseInformation(model);
        }
        for (auto iter = networkUnits.begin(); iter != networkUnits.end(); ++iter){
            NetworkUnitBase* item = *iter;
            for (auto iter2 = item->getOutputUnitList().begin(); iter2 != item->getOutputUnitList().end(); ++iter2){
                NetworkUnitBase* toItem = *iter2;
                int inId = idMap[item->getUnitId()];
                int outId = idMap[toItem->getUnitId()];
                if (inputNodes.find(inId) == inputNodes.end()){
                    model.addEdgeToNetwork(inId, outId);
                }
                else{
                    model.linkInputToNetwork(inId, outId);
                }
            }
        }
    }
}