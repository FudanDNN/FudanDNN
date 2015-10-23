#pragma once
#include "networkunitbase.h"
class CNNSubnet;
class CNNUnitBase :
	public NetworkUnitBase
{
	friend CNNSubnet;
private:
	int bindCNNSubnetId;
public:
	virtual bool isAvailbleInput(NetworkUnitBase*) override;
	virtual bool isAvailbleOutput(NetworkUnitBase*) override;
	/*
		获取该CNN节点绑定在哪个CNN子网络上，如果为-1表示没有绑定，应当被立即释放或者绑定。
	*/
	int getSubnetId() const {
		return bindCNNSubnetId;
	}
	CNNUnitBase(DiagramItem *, NetworkUnitType typeId);
	~CNNUnitBase();
};

