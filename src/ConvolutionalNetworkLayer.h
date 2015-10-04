#ifndef __FUDANDNN_CONVOLUTIONALNETWORKLAYER_H
#define __FUDANDNN_CONVOLUTIONALNETWORKLAYER_H
#include "utils.h"
#include "AbstractNetworkLayer.h"
#include "CNN2DComponent.h"
#include "ComponentNode.h"
#include "MaxPoolingLayer.h"
#include "NonLinearComponent.h"

using namespace std;
typedef pair <int, shared_ptr<ComponentNode>> Component_Pair;
typedef pair <size_t, size_t> InDegree_Pair;

class ConvolutionalNetworkLayer : public AbstractNetworkLayer{
public:
	ConvolutionalNetworkLayer();
	string getNetworkName() override;
	void compute()override;
	void calculate()override;
	void gradient()override;
	void update() override;
	size_t addCNN2DComponentToCNN(size_t kernelSize, size_t stride, size_t featureMap_Num,
		size_t num, size_t visualRow, size_t visualColumn, size_t scheme);
	size_t addMaxPoolingToCNN(size_t poolingSize, size_t stride,
		size_t visualRow, size_t visualColumn, size_t num);
	size_t addNonLinearToCNN(int visualRow,int visualColumn,int num, size_t type);
	size_t addEdgeInCNN(size_t inId, size_t outId);
	void topoSort();
private:
	vector<shared_ptr<ComponentNode>> nodes;
	int currentId;
	map<int, shared_ptr<ComponentNode>> idMap;
	map<int, size_t> inDegreeMap;
	bool sorted = false;
};
#endif