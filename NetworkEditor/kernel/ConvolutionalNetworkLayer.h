#ifndef __FUDANDNN_CONVOLUTIONALNETWORKLAYER_H
#define __FUDANDNN_CONVOLUTIONALNETWORKLAYER_H
#include "utils.h"
#include "AbstractNetworkLayer.h"
#include "CNN2DComponent.h"
#include "CNN1DComponent.h"
#include "ComponentNode.h"
#include "MaxPoolingLayer.h"
#include "NonLinearComponent.h"
#include <direct.h>

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
	void init() override;
	int checkCNN();
	size_t addCNN2DComponentToCNN(size_t kernelSize, size_t stride, size_t featureMap_Num,
		size_t num, size_t visualRow, size_t visualColumn, size_t scheme);
    size_t ConvolutionalNetworkLayer::addCNN2DComponentToCNN(size_t kernelSize, size_t stride,
        size_t featureMap_Num, size_t num, size_t visualRow, size_t visualColumn, size_t scheme,
        double regularizationRate, double kernelLearningRate, double biasLearningRate, double momentumRate);
	size_t addMaxPoolingToCNN(size_t poolingSize, size_t stride,
		size_t visualRow, size_t visualColumn, size_t num);
	size_t addNonLinearToCNN(int visualRow,int visualColumn,int num, size_t type);
	size_t addEdgeInCNN(size_t inId, size_t outId);
	size_t addComponent(shared_ptr<ComponentNode> node);
	void topoSort();
	void writeSelf(string);
	void readSelf(string);
	void setParameterForCNN1DComponent(size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn, size_t kernelSize,
		size_t stride, size_t featureMapNum, double regularizationRate, double kernelLearningRate, double biasLearningRate, double momentumRate);
	void setParameterForCNN2DComponent(size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn, size_t kernelSize,
		size_t stride, size_t featureMapNum, double regularizationRate, double kernelLearningRate, double biasLearningRate, double momentumRate);
	void setParameterForMaxPoolingComponent(size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn,
		size_t stride, size_t poolingSize);
	void setParameterForNonLinearComponent(size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn);
	vector<shared_ptr<ComponentNode>> getAllComponents();
private:
	vector<shared_ptr<ComponentNode>> nodes;
	size_t currentId=0;
	map<int, shared_ptr<ComponentNode>> idMap;
	map<int, size_t> inDegreeMap;
	bool sorted = false;
	size_t vRow;
	size_t vColumn;
};
#endif