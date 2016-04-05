#ifndef NETWORKUNITPOOL_H

#define NETWORKUNITPOOL
#include <vector>
#include <memory>
#include "networkunitbase.h"
namespace fudandnn{
    class Model;
}
class DiagramItem;
/*
NetworkUnitPool负责维护整个程序运行过程中产生的所有网络单元的映射类
一个NetworkUnit在产生后应该在这里注册，然后这里负责最终清楚这个类占据的空间
*/
namespace NetworkUnitPool{
	extern int idCount;
	extern std::vector<NetworkUnitBase *> networkUnits;
	/* 注册一个单元对象 */
	void registerUnit(NetworkUnitBase *);
	/* 解除一个单元对象的注册*/
	void unregisterUnit(NetworkUnitBase *);
	/* 连接两个网络单元，第一个的输出作为第二个的输入*/
	void addLink(NetworkUnitBase* start, NetworkUnitBase* end);
	/* 获得一个已经注册的单元对象的工厂方法*/
	NetworkUnitBase * createNetworkUnit(DiagramItem*, NetworkUnitBase::NetworkUnitType);
	/* 输出网络图状态，用于调试*/
	void outputNetworkGraph(std::ostream& out);
	/* 将网络分层。输出是一个int数组，表示networkUnits的每一项应该处在第几层*/
	std::vector<int > sliceLayers();
    void parseInformation(fudandnn::Model &, std::shared_ptr<Solver>);
	/*
	TODO
	添加与后端代码交互的方法
	*/
}

#endif