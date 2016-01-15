#ifndef __FUDANDNN_NETWORK_H
#define __FUDANDNN_NETWORK_H
#include "util\utils.h"
#include "layer\Layer.h"
#include "layer\SolverFactory.h"
#include "layer\LinearLayer.h"
#include "layer\LayerNode.h"
#include "util\Input.h"
#include "layer\NonlinearLayer.h"
#include "functions\FunctionFactory.h"

typedef pair <size_t, shared_ptr<LayerNode>> Node_Pair;
typedef pair <size_t, size_t> InDegree_Pair;
typedef pair <size_t, shared_ptr<Input>> Input_Pair;
typedef pair <size_t, shared_ptr<Matrix>> Matrix_Pair;

class Network{
public:
	Network()
	{
		solverFactory = shared_ptr<SolverFactory>(new SolverFactory());
		functionFactory = shared_ptr<FunctionFactory>(new FunctionFactory());
	};

	size_t addLinearLayer(size_t visualUnit, size_t hiddenUnit, size_t init_scheme, size_t solver_type,double regularizationRate, 
		double weightLearningRate, double biasLearningRate, double momentumRate,double dropoutLayer);
	size_t addNonlinearLayer(size_t visualUnit, size_t type, double s, double lb, double ub, double prec, double ic);
	size_t addEdge(size_t inID, size_t outID);

	void removeNode(size_t id);
	void removeEdge(size_t inID, size_t outID);

	void init();
	void topoSort();

	void trainingForward();
	void testingForward();
	void backward();
	void update();

	size_t addInput(shared_ptr<Input> input);
	size_t addInputEdge(size_t inputId, size_t layerId);

	shared_ptr<Matrix> getGradientForInput(size_t inputId);

	vector<shared_ptr<Matrix>> getFinalValue();
	void setFinalGradient(vector<shared_ptr<Matrix>> finalGradient);

protected:
	shared_ptr<SolverFactory> solverFactory;
	shared_ptr<FunctionFactory> functionFactory;

	// whether the graph is sorted
	bool sorted;
	//whether the network is inited;
	bool initialized;

	size_t currentInputId = 0;

	//store all the layer nodes
	vector<shared_ptr<LayerNode>> nodes;
	//store the id, assign to new layer
	size_t currentId = 0;
	//map id to layer node
	map<size_t, shared_ptr<LayerNode>> idMap;
	//map id to the indegree of layer
	map<size_t, size_t> inDegreeMap;
	

	//value for the network's result
	vector<shared_ptr<Matrix>> finalValue; 
	//gradient for the network entrance
	vector<shared_ptr<Matrix>> finalGradient;

	//inputs for the whole network
	vector<shared_ptr<Input>> inputs;
	//map id to input
	map<size_t, shared_ptr<Input>> inputMap;
	//gradient for inputs
	map<size_t, shared_ptr<Matrix>> inputGradientMap;
};

#endif