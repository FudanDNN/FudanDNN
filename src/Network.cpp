#include "Network.h"

size_t Network::addLinearLayer(size_t visualUnit, size_t hiddenUnit, size_t init_scheme, size_t solver_type,
	double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate,double dropoutRate)
{
	shared_ptr<LinearLayer> linearLayer;
	linearLayer = shared_ptr<LinearLayer>(new LinearLayer(visualUnit, hiddenUnit, init_scheme,
		solverFactory->createSolver(solver_type, regularizationRate,
		weightLearningRate, biasLearningRate, momentumRate)));
	shared_ptr<LayerNode> node(new LayerNode(currentId, linearLayer));
	idMap.insert(Node_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t Network::addNonlinearLayer(size_t visualUnit, size_t type)
{
	shared_ptr<NonlinearLayer> nonlinearLayer;
	shared_ptr<NonlinearFactory> factory(new NonlinearFactory());
	nonlinearLayer = factory->createNonlinear(type, visualUnit);
	shared_ptr<LayerNode> node(new LayerNode(currentId, nonlinearLayer));
	idMap.insert(Node_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t Network::addEdge(size_t inID, size_t outID)
{
	shared_ptr<LayerNode> inNode = idMap.find(inID)->second;
	shared_ptr<LayerNode> outNode = idMap.find(outID)->second;
	inNode->addNextNode(outNode);
	outNode->addPrevNode(inNode);
	return 0;
}

void Network::removeNode(size_t id)
{
	shared_ptr<LayerNode> node = idMap.find(id)->second;
	auto itr = nodes.cbegin();
	while (itr != nodes.end()){
		if ((*itr) == node) break;
	}
	nodes.erase(itr);
}
void Network::removeEdge(size_t inID, size_t outID)
{

}

void Network::init()
{
	for (int i = 0; i < nodes.size(); i++){
		shared_ptr<Layer> networkLayer = (nodes[i]->getLayer());
		if (networkLayer != nullptr)
			networkLayer->init();
	}
}

void Network::topoSort()
{
	for (shared_ptr<LayerNode> node : nodes)
	{
		vector<shared_ptr<LayerNode>> prevNodes = node->getPrevNode();
		this->inDegreeMap.insert(InDegree_Pair(node->getId(), prevNodes.size()));
	}

	size_t i = 0;
	size_t size = nodes.size();
	while (i < size)
	{
		size_t inDegree_i = inDegreeMap.find(nodes[i]->getId())->second;
		if (inDegree_i == 0)
		{
			for (shared_ptr<LayerNode> succ : nodes[i]->getNextNode())
			{
				size_t succId = succ->getId();
				size_t in_d = inDegreeMap.find(succId)->second - 1;
				inDegreeMap.erase(succId);
				inDegreeMap.insert(InDegree_Pair(succId, in_d));
			}
			i++;
			continue;
		}

		size_t j = i + 1;
		while (j < size)
		{
			size_t inDegree_j = inDegreeMap.find(nodes[j]->getId())->second;
			if (inDegree_j == 0)
			{
				break;
			}
			j++;
		}

		shared_ptr<LayerNode> tmp = nodes[i];
		nodes[i] = nodes[j];
		nodes[j] = tmp;

	}

	sorted = true;
}

