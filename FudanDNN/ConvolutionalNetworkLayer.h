#include "AbstractNetworkLayer.h"
#include "CNN2DComponent.h"
#include "ComponentNode.h"
#include "MaxPoolingLayer.h"
#include <string>
#include <hash_map>

using namespace std;
typedef pair <int, shared_ptr<ComponentNode>> Component_Pair;
typedef pair <size_t, size_t> InDegree_Pair;
#pragma once
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
		size_t visualRow, size_t visualColumn);
	size_t addEdgeInCNN(size_t inId, size_t outId);
	void topoSort();
private:
	vector<shared_ptr<ComponentNode>> nodes;
	int currentId;
	hash_map<int, shared_ptr<ComponentNode>> idMap;
	hash_map<int, size_t> inDegreeMap;
	bool sorted = false;
};