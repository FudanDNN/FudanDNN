#ifndef NETWORKUNITBASE_H
#define NETWORKUNITBASE_H

#include "diagramitem.h"
#include <vector> 
#include <ostream>
#include <queue>
#include "hyperparametercontrol.h"
class NetworkUnitBase
{
public:
	enum NetworkUnitType {NULL_UNIT, INPUT_UNIT, LINEAR_UNIT, NONLINEAR_UNIT, CONVOLUTION_UNIT, MAXPOOLING_UNIT, CONCAT_UNIT};
    NetworkUnitBase(DiagramItem *,NetworkUnitType typeId);
    ~NetworkUnitBase();
    void addInputUnit(NetworkUnitBase *);
    void addOutputUnit(NetworkUnitBase *);
    void removeInputUnit(NetworkUnitBase*);
    void removeOutputUnit(NetworkUnitBase*);
    void setIcon(QPixmap *p) {this->icon=p;}
    QPixmap* getIcon() const{return this->icon;}
    int getUnitId() const {return this->unitId;}
    QString getTitle() const {return this->title;}
    void setTitle(QString const &s){this->title=s;}
    DiagramItem* getDiagramItem() const{return this->diagramItem;}
	const std::vector<NetworkUnitBase*> & getInputUnitList() const { return this->inputUnit; };
	const std::vector <NetworkUnitBase*> & getOutputUnitList() const {return this->outputUnit;};
	NetworkUnitType getTypeId() const { return this->typeId; }
	void setTypeId(NetworkUnitType typeId) { this->typeId = typeId; }
	virtual void readParameters(std::istream &){}
	virtual void writeParameters(std::ostream &){}
	virtual bool isAvailbleInput(NetworkUnitBase* ){ return true; }
	virtual bool isAvailbleOutput(NetworkUnitBase* ){ return true; }
	void setHyperParameterControl(HyperParameterControl* hpc){ this->hyperParameterControl = hpc; }
	HyperParameterControl* getHyperParameterControl() { return hyperParameterControl; }
private:
    const int unitId;
	NetworkUnitType typeId;
    QPixmap *icon;
    QString title;
	std::vector<NetworkUnitBase*> inputUnit;
	std::vector<NetworkUnitBase*> outputUnit;
    DiagramItem *diagramItem;
	HyperParameterControl *hyperParameterControl;
};

#endif // NETWORKUNITBASE_H
