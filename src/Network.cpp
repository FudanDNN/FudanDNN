#include "Network.h"

size_t Network::addLinearLayer(size_t visualUnit, size_t hiddenUnit, size_t init_scheme, size_t solver_type,
	double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate,double dropoutRate)
{
	shared_ptr<LinearLayer> linearLayer;
	shared_ptr<Solver> solver = solverFactory->createSolver(solver_type, regularizationRate,
		weightLearningRate, biasLearningRate, momentumRate);
	linearLayer = shared_ptr<LinearLayer>(new LinearLayer(visualUnit, hiddenUnit, init_scheme,solver));
	shared_ptr<LayerNode> node(new LayerNode(currentId, linearLayer));
	idMap.insert(Node_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t Network::addNonlinearLayer(size_t visualUnit, size_t type, double s, double lb, double ub, double prec, double ic)
{
	shared_ptr<Function> function = this->functionFactory->createFunction(type, s, lb, ub, prec, ic);
	shared_ptr<NonlinearLayer> nonlinear(new NonlinearLayer(function, type, visualUnit));
	shared_ptr<LayerNode> node(new LayerNode(currentId, nonlinear));
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
	initialized = true;
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

void Network::trainingForward()
{
	for (shared_ptr<LayerNode> node : nodes)
	{
		size_t id = node->getId();
		shared_ptr<Layer> layer = node->getLayer();
		vector<shared_ptr<Matrix>> visualValue;
		for (shared_ptr<LayerNode> pred : node->getPrevNode())
		{
			if (visualValue.size() == 0){
				visualValue = pred->getLayer()->getHiddenValue();
			}
			else{
				visualValue[0] = visualValue[0]->mergeRow(pred->getLayer()->getHiddenValue()[0]);
			}
		}

		if (node->getInputCell() != nullptr)
		{
			shared_ptr<Matrix> input = node->getInputCell()->getInputMat();
			if (visualValue.size() == 0){
				visualValue.push_back(input);
			}
			else{
				visualValue[0] = visualValue[0]->mergeRow(input);
			}
		}
		layer->setVisualValue(visualValue);
		layer->calculate();

	}

	shared_ptr<Layer> lastLayer = nodes.at(nodes.size() - 1)->getLayer();
	finalValue = lastLayer->getHiddenValue();

}

void Network::testingForward()
{

}

void Network::backward()
{
	shared_ptr<Layer> lastLayer = nodes.at(nodes.size() - 1)->getLayer();
	lastLayer->addHiddenGradient(finalGradient);
	for (int i = nodes.size() - 1; i >= 0; i--)
	{
		shared_ptr<LayerNode> node = nodes.at(i);
		size_t id = node->getId();

		shared_ptr<Layer> layer = node->getLayer();

		layer->gradient();

		size_t preLength = 0;
		size_t currLength = 0;

		for (shared_ptr<LayerNode> pred : node->getPrevNode())
		{
			shared_ptr<Layer> predLayer = pred->getLayer();
			currLength = predLayer->getHiddenValue()[0]->getRowSize();
			vector<shared_ptr<Matrix>> tempVec;
			tempVec.push_back(layer->getVisualGradient()[0]->submatrix(preLength, preLength + currLength, 0, 1));
			predLayer->addHiddenGradient(tempVec);
			preLength += currLength;
		}



	}
}

size_t Network::addInput(size_t inputId)
{
	shared_ptr<InputCell> cell(new InputCell(inputId));
	inputCells.push_back(cell);
	cellMap.insert(Cell_Pair(inputId, cell));
	return inputId;
}

size_t Network::addInputEdge(size_t inputId, size_t layerId)
{
	shared_ptr<LayerNode> outNode = idMap.find(layerId)->second;
	outNode->setInputCell(cellMap.find(inputId)->second);
	return 0;
}

shared_ptr<Matrix> Network::getGradientForInput(size_t inputId)
{
	return cellMap.find(inputId)->second->getGradientMat();
}

void Network::setInputMat(size_t inputId, shared_ptr<Matrix> inputMat)
{
	cellMap.find(inputId)->second->setInputMat(inputMat);
}

vector<shared_ptr<Matrix>> Network::getFinalValue()
{
	return this->finalValue;
}

void Network::setFinalGradient(vector<shared_ptr<Matrix>> finalGradient)
{
	this->finalGradient = finalGradient;
}

void Network::update()
{
	for (shared_ptr<LayerNode> node : nodes){
		node->getLayer()->update();
	}
}
