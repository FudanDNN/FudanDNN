#include "ConvolutionalNetworkLayer.h"
ConvolutionalNetworkLayer::ConvolutionalNetworkLayer(){

}
void ConvolutionalNetworkLayer::compute(){
	if (!sorted){
		topoSort();
	}

	shared_ptr<AbstractMatrix> v = this->visualValue;
	for (shared_ptr<ComponentNode> node : nodes)
	{
		int id = node->getId();

		shared_ptr<AbstractComponent> component = node->getComponent();

		vector<shared_ptr<AbstractMatrix>> vValue;
		for (shared_ptr<ComponentNode> pred : node->getBeforeNode())
		{
			vector<shared_ptr<AbstractMatrix>> temp = pred->getComponent()->getHiddenValue();
			for (shared_ptr<AbstractMatrix> tempMatix : temp){
				vValue.push_back(tempMatix);
			}
		}

		if (node->getBeforeNode().size() == 0)
		{
			vValue.push_back(v);
		}

		component->setVisualValue(vValue);
		component->calculate();
	}
	vector<shared_ptr<AbstractMatrix>> hValue;
	v = shared_ptr<AbstractMatrix>(new Matrix(0, 1));
	for (shared_ptr<ComponentNode> node : nodes){
		if (node->getNextNode().size() == 0){
			vector<shared_ptr<AbstractMatrix>> temp = node->getComponent()->getHiddenValue();
			for (shared_ptr<AbstractMatrix> tempMatrix : temp){
				for (size_t i = 0; i < tempMatrix->getColumnSize(); i++){
					v->mergeRow(tempMatrix->m2vByColumn());
				}
			}
		}
	}
	this->hiddenValue = v;
}

void ConvolutionalNetworkLayer::calculate(){
	if (!sorted){
		topoSort();
	}
	//shared_ptr<AbstractMatrix> v = this->visualValue;
	for (shared_ptr<ComponentNode> node : nodes)
	{
		int id = node->getId();

		shared_ptr<AbstractComponent> component = node->getComponent();
		//cout << "1----" << &component->getHiddenValue() << "----" << &node << "----" << &nodes << endl;
		vector<shared_ptr<AbstractMatrix>> vValue;
		for (shared_ptr<ComponentNode> pred : node->getBeforeNode())
		{
			vector<shared_ptr<AbstractMatrix>> temp = pred->getComponent()->getHiddenValue();
			for (shared_ptr<AbstractMatrix> tempMatix : temp){
				vValue.push_back(tempMatix);
			}
		}

		if (node->getBeforeNode().size() == 0)
		{
			vValue.push_back(visualValue);
		}

		component->setVisualValue(vValue);
		component->calculate();

	}

	vector<shared_ptr<AbstractMatrix>> hValue;
	shared_ptr<AbstractMatrix> v = shared_ptr<AbstractMatrix>(new Matrix(0, 1));
	v->initializeValue(0, 0);
	for (shared_ptr<ComponentNode> node : nodes){
		//cout << "2----" << &(node->getComponent()->getHiddenValue()) << "----" << &node << "----" << &nodes << endl;
		if (node->getNextNode().size() == 0){
			vector<shared_ptr<AbstractMatrix>> temp = node->getComponent()->getHiddenValue();
			for (shared_ptr<AbstractMatrix> tempMatrix : temp){
				v = v->mergeRow(tempMatrix->m2vByColumn());
			}
		}
	}
	this->hiddenValue = v;
	//cout << "---" << endl;
	//visualValue->print();
	//cout << "---" << endl;
	//hiddenValue->print();
	this->hiddenUnit = hiddenValue->getRowSize();
}

void ConvolutionalNetworkLayer::update(){
	if (!sorted){
		topoSort();
	}
	for (shared_ptr<ComponentNode> node : nodes){
		shared_ptr<AbstractComponent> component = node->getComponent();
		component->update();
		component->getNum();
	}
}

void ConvolutionalNetworkLayer::gradient(){
	if (!sorted){
		topoSort();
	}
	//shared_ptr<AbstractMatrix> m = this->hiddenGradient;
	//hiddenGradient->print();
	size_t hiddenUnitNum = this->hiddenUnit;
	size_t index = hiddenUnitNum;
	for (int j = nodes.size() - 1; j >= 0; j--){
		shared_ptr<ComponentNode> node = nodes[j];
		shared_ptr<AbstractComponent> component = node->getComponent();

		if (node->getNextNode().size() == 0){
			size_t vectorSize = component->getHiddenValue().size();
			size_t mRow = component->getHiddenValue()[0]->getRowSize();
			size_t mColumn = component->getHiddenValue()[0]->getColumnSize();
			size_t square = mRow*mColumn;

			shared_ptr<AbstractMatrix> gradientMatrix = hiddenGradient->submatrix(index - square*vectorSize, index, 0, 1);
			vector<shared_ptr<AbstractMatrix>> gradient;

			for (size_t i = 0; i < vectorSize; i++){
				shared_ptr<AbstractMatrix> matrix = gradientMatrix->submatrix(i*square, (i + 1)*square, 0, 1);
				gradient.push_back(matrix->v2mByColomn(mRow));
			}
			component->setHiddenGradient(gradient);
			component->gradient();
			index -= vectorSize*square;
		}
		//cout << component->getVisualGradient().size() << j <<" "<<nodes.size() << endl;
		vector<shared_ptr<AbstractMatrix>> vGradient = component->getVisualGradient();
		size_t predIndex = 0;
		for (shared_ptr<ComponentNode> pred : node->getBeforeNode())
		{
			shared_ptr<AbstractComponent> predComponent = pred->getComponent();
			size_t predSize = predComponent->getHiddenValue().size();
			vector<shared_ptr<AbstractMatrix>> predGradient;
			//cout << vGradient.size() << predSize << endl;
			for (int i = 0; i < predSize; i++){
				predGradient.push_back(vGradient[predIndex + i]);
			}
			/*for (int x = 0; x < predGradient.size(); x++){
			predGradient[x]->print();
			}*/
			predComponent->setHiddenGradient(predGradient);
			predComponent->gradient();
			//cout << predComponent->getNum() << endl;
			/*for (int x = 0; x < predComponent->getVisualGradient().size(); x++){
			predComponent->getVisualGradient()[x]->print();
			}*/
			predIndex += predSize;
		}
		if (node->getBeforeNode().size() == 0){
			/*for (int x = 0; x < component->getVisualGradient().size(); x++){
			component->getVisualGradient()[x]->print();
			}*/
			this->visualGradient = component->getVisualGradient()[0];
		}
	}
	//visualGradient->print();
	this->hiddenGradient->initializeValue(0, 0);
}

size_t ConvolutionalNetworkLayer::addCNN2DComponentToCNN(size_t kernelSize, size_t stride,
	size_t featureMap_Num, size_t num, size_t visualRow, size_t visualColumn, size_t scheme){
	shared_ptr<CNN2DComponent> max(new CNN2DComponent(kernelSize, stride, featureMap_Num, num,
		visualRow, visualColumn, scheme));
	shared_ptr<ComponentNode> node(new ComponentNode(currentId, max));
	idMap.insert(Component_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t ConvolutionalNetworkLayer::addMaxPoolingToCNN(size_t poolingSize,
	size_t stride, size_t visualRow, size_t visualColumn, size_t num){
	shared_ptr<MaxPoolingLayer> max(new MaxPoolingLayer(poolingSize, stride, visualRow, visualColumn, num));
	shared_ptr<ComponentNode> node(new ComponentNode(currentId, max));
	idMap.insert(Component_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t ConvolutionalNetworkLayer::addNonLinearToCNN(int visualRow, int visualColumn, int num, size_t type){
	shared_ptr<NonLinearComponent> nonLinear(new NonLinearComponent(visualRow, visualColumn, num, type));
	shared_ptr<ComponentNode> node(new ComponentNode(currentId, nonLinear));
	idMap.insert(Component_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t ConvolutionalNetworkLayer::addEdgeInCNN(size_t inId, size_t outId){
	shared_ptr<ComponentNode> inNode = idMap.find(inId)->second;
	shared_ptr<ComponentNode> outNode = idMap.find(outId)->second;
	inNode->addNextNode(outNode);
	outNode->addBeforeNode(inNode);
	return 0;
}

void ConvolutionalNetworkLayer::topoSort(){
	for (shared_ptr<ComponentNode> node : nodes)
	{
		vector<shared_ptr<ComponentNode>> beforeNodes = node->getBeforeNode();
		this->inDegreeMap.insert(InDegree_Pair(node->getId(), beforeNodes.size()));
	}

	size_t i = 0;
	size_t size = nodes.size();
	while (i < size)
	{
		size_t inDegree_i = inDegreeMap.find(nodes[i]->getId())->second;
		if (inDegree_i == 0)
		{
			for (shared_ptr<ComponentNode> succ : nodes[i]->getNextNode())
			{
				int succId = succ->getId();
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
			size_t inDegree_j = inDegreeMap.find(nodes[i]->getId())->second;
			if (inDegree_j == 0)
			{
				break;
			}
			j++;
		}

		shared_ptr<ComponentNode> tmp = nodes[i];
		nodes[i] = nodes[j];
		nodes[j] = tmp;

	}
	sorted = true;
}
string ConvolutionalNetworkLayer::getNetworkName(){
	return "ContractiveNetworkLayer";
}

void ConvolutionalNetworkLayer::writeSelf(string filename){

}

void ConvolutionalNetworkLayer::readSelf(string filename){

}