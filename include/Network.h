#ifndef __FUDANDNN_NETWORK_H
#define __FUDANDNN_NETWORK_H
#include "util\utils.h"
#include "layer\Layer.h"
#include "layer\NonlinearFactory.h"
#include "layer\SolverFactory.h"
#include "layer\LinearLayer.h"
#include "layer\LayerNode.h"

typedef pair <size_t, shared_ptr<LayerNode>> Node_Pair;
typedef pair <size_t, size_t> InDegree_Pair;

class Network{
public:
	Network()
	{
		nonlinearFactory = shared_ptr<NonlinearFactory>(new NonlinearFactory());
		solverFactory = shared_ptr<SolverFactory>(new SolverFactory());
	};

	size_t addLinearLayer(size_t visualUnit, size_t hiddenUnit, size_t init_scheme, size_t solver_type,double regularizationRate, 
		double weightLearningRate, double biasLearningRate, double momentumRate,double dropoutLayer);
	size_t addNonlinearLayer(size_t visualUnit, size_t type);
	size_t addEdge(size_t inID, size_t outID);

	void removeNode(size_t id);
	void removeEdge(size_t inID, size_t outID);

	void init();
	void topoSort();






protected:
	shared_ptr<NonlinearFactory> nonlinearFactory;
	shared_ptr<SolverFactory> solverFactory;
	//store all the layer nodes
	vector<shared_ptr<LayerNode>> nodes;
	//store the id, assign to new layer
	size_t currentId = 0;
	//map id to layer node
	map<size_t, shared_ptr<LayerNode>> idMap;
	//map id to the indegree of layer
	map<size_t, size_t> inDegreeMap;
	// whether the graph is sorted
	bool sorted;
	//value for the network entrance
	vector<shared_ptr<Matrix>> beginValue;
	//value for the network's result
	vector<shared_ptr<Matrix>> finalValue;
	//gradient for the network entrance
	vector<shared_ptr<Matrix>> beginGradient;
	//gradient for the network entrance
	vector<shared_ptr<Matrix>> finalGradient;

};

#endif