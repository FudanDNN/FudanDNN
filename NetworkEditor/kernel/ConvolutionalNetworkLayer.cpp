#include "ConvolutionalNetworkLayer.h"
ConvolutionalNetworkLayer::ConvolutionalNetworkLayer(){
	this->visualUnit = -1;
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

size_t ConvolutionalNetworkLayer::addComponent(shared_ptr<ComponentNode> node){
    node->setId(currentId);
	idMap.insert(Component_Pair(currentId, node));
	nodes.push_back(node);
	currentId++;
	return node->getId();
}

void ConvolutionalNetworkLayer::calculate(){
	/*cout << nodes.size() << endl;
	this->visualValue->print();*/
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
	v->setAllValue(0);
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
	if (nodes.size() == 0){
		this->hiddenValue = this->visualValue->m2vByColumn();
	}
	//this->hiddenValue = this->visualValue->m2vByColumn();
	this->hiddenUnit = hiddenValue->getRowSize();
	//cout << "---" << endl;
	//visualValue->print();
	/*cout << "---" <<hiddenUnit<< endl;
	hiddenValue->print();*/
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
	/*cout << "----" << endl;
	this->visualGradient->print();*/
	this->hiddenGradient->setAllValue(0);
}

void ConvolutionalNetworkLayer::init(){
	if (this->initialized == true){
		return;
	}
	for (int i = 0; i < nodes.size(); i++){
		nodes[i]->getComponent()->initialization();
	}
	initialized = true;
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
size_t ConvolutionalNetworkLayer::addCNN2DComponentToCNN(size_t kernelSize, size_t stride,
    size_t featureMap_Num, size_t num, size_t visualRow, size_t visualColumn, size_t scheme,
    double regularizationRate, double kernelLearningRate, double biasLearningRate, double momentumRate){
    shared_ptr<CNN2DComponent> max(new CNN2DComponent(kernelSize, stride, featureMap_Num, num,
        visualRow, visualColumn, scheme));
    max->setRegularizationRate(regularizationRate);
    max->setBiasLearningRate(biasLearningRate);
    max->setKernelLearningRate(kernelLearningRate);
    max->setMomentumRate(momentumRate);
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
	return "ConvolutionalNetworkLayer";
}
int ConvolutionalNetworkLayer::checkCNN(){
	size_t temp = 0;
	for (shared_ptr<ComponentNode> node : nodes)
	{
		if (node->getBeforeNode().size() == 0){
			this->vRow = node->getComponent()->getVisualRow();
			this->vColumn = node->getComponent()->getVisualColumn();
		}
		if (node->getNextNode().size() == 0){
			temp += node->getComponent()->calculateHiddenSize();
		}
	}
	this->hiddenUnit = temp;
	return CORRECT;
}
void ConvolutionalNetworkLayer::writeSelf(string directory){
	_mkdir(directory.c_str());

	ofstream fout(directory + "/CouvolutionalNetworkLayer.net", ios::out);

	fout << nodes.size();
	fout << '\n';

	for (size_t i = 0; i < nodes.size(); i++)
	{
		fout << nodes.at(i)->getComponent()->getComponentName() << '\n';
		fout << nodes.at(i)->getId() << endl;
		shared_ptr<AbstractComponent> layer = nodes.at(i)->getComponent();
		char *temp = new char[3];
		sprintf_s(temp, 3 * sizeof(char), "%d", i);
		string s(temp);
		layer->writeSelf(directory + "/" + layer->getComponentName() + s);
	}

	for (size_t i = 0; i < nodes.size(); i++)
	{
		fout << nodes.at(i)->getNextNode().size() << endl;
		for (size_t j = 0; j < nodes.at(i)->getNextNode().size(); j++)
		{
			fout << nodes.at(i)->getNextNode().at(j)->getId() << ' ';
		}
		fout << '\n';
	}

	fout.close();

}

void ConvolutionalNetworkLayer::readSelf(string directory){
	ifstream fin(directory + "/CouvolutionalNetworkLayer.net", ios::in);

	int size;
	fin >> size;
	for (size_t i = 0; i < size; i++)
	{

		string name_s;
		fin >> name_s;
		int id;
		fin >> id;
		const char *name = name_s.c_str();
		char *temp = new char[3];
		sprintf_s(temp, 3 * sizeof(char), "%d", i);
		string s(temp);

		shared_ptr<ComponentNode> node;

		if (strcmp(name, "CNN1D") == 0)
		{
			shared_ptr<AbstractComponent> component(new CNN1DComponent());
			component->readSelf(directory + "/" + name + s);
			node = shared_ptr<ComponentNode>(new ComponentNode(id, component));
		}
		else if (strcmp(name, "CNN2D") == 0)
		{
			shared_ptr<AbstractComponent> component(new CNN2DComponent());
			component->readSelf(directory + "/" + name + s);
			node = shared_ptr<ComponentNode>(new ComponentNode(id, component));
		}
		else if (strcmp(name, "MaxPooling") == 0)
		{
			shared_ptr<AbstractComponent> component(new MaxPoolingLayer());
			component->readSelf(directory + "/" + name + s);
			node = shared_ptr<ComponentNode>(new ComponentNode(id, component));
		}
		else if (strcmp(name, "NonLinear") == 0)
		{
			shared_ptr<AbstractComponent> component(new NonLinearComponent());
			component->readSelf(directory + "/" + name + s);
			node = shared_ptr<ComponentNode>(new ComponentNode(id, component));
		}
		idMap.insert(Component_Pair(id, node));
		nodes.push_back(node);

	}

	for (size_t i = 0; i < size; i++)
	{
		int nextNum;
		fin >> nextNum;
		for (size_t j = 0; j < nextNum; j++)
		{
			int id;
			fin >> id;
			//cout << nodes[i]->getId() << endl;
			this->addEdgeInCNN(nodes[i]->getId(), id);
		}
	}
	this->initialized = true;
	fin.close();
}

void ConvolutionalNetworkLayer::setParameterForCNN1DComponent(size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn, size_t kernelSize,
	size_t stride, size_t featureMapNum, double regularizationRate, double kernelLearningRate, double biasLearningRate, double momentumRate){
	shared_ptr<ComponentNode> node = idMap.find(id)->second;
	shared_ptr<CNN1DComponent> cnn1d = dynamic_pointer_cast<CNN1DComponent>(node->getComponent());
	cnn1d->setNum(num);
	cnn1d->setVisualRow(visualRow);
	cnn1d->setVisualColumn(visualColumn);
	cnn1d->setHiddenRow(hiddenRow);
	cnn1d->setHiddenColumn(hiddenColumn);
	cnn1d->setKernelSize(kernelSize);
	cnn1d->setStride(stride);
	cnn1d->setFeatureMap(featureMapNum);
	cnn1d->setRegularizationRate(regularizationRate);
	cnn1d->setKernelLearningRate(kernelLearningRate);
	cnn1d->setBiasLearningRate(biasLearningRate);
	cnn1d->setMomentumRate(momentumRate);
}

void ConvolutionalNetworkLayer::setParameterForCNN2DComponent(size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn, size_t kernelSize,
	size_t stride, size_t featureMapNum, double regularizationRate, double kernelLearningRate, double biasLearningRate, double momentumRate){
	shared_ptr<ComponentNode> node = idMap.find(id)->second;
	shared_ptr<CNN2DComponent> cnn2d = dynamic_pointer_cast<CNN2DComponent>(node->getComponent());
	cnn2d->setNum(num);
	cnn2d->setVisualRow(visualRow);
	cnn2d->setVisualColumn(visualColumn);
	cnn2d->setHiddenRow(hiddenRow);
	cnn2d->setHiddenColumn(hiddenColumn);
	cnn2d->setKernelSize(kernelSize);
	cnn2d->setStride(stride);
	cnn2d->setFeatureMap(featureMapNum);
	cnn2d->setRegularizationRate(regularizationRate);
	cnn2d->setKernelLearningRate(kernelLearningRate);
	cnn2d->setBiasLearningRate(biasLearningRate);
	cnn2d->setMomentumRate(momentumRate);
}

void ConvolutionalNetworkLayer::setParameterForMaxPoolingComponent(size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn,
	size_t stride, size_t poolingSize){
	shared_ptr<ComponentNode> node = idMap.find(id)->second;
	shared_ptr<MaxPoolingLayer> maxPooling = dynamic_pointer_cast<MaxPoolingLayer>(node->getComponent());
	maxPooling->setNum(num);
	maxPooling->setVisualRow(visualRow);
	maxPooling->setVisualColumn(visualColumn);
	maxPooling->setHiddenRow(hiddenRow);
	maxPooling->setHiddenColumn(hiddenColumn);
	maxPooling->setStride(stride);
	maxPooling->setPoolingSize(poolingSize);
}

void ConvolutionalNetworkLayer::setParameterForNonLinearComponent(size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn){
	shared_ptr<ComponentNode> node = idMap.find(id)->second;
	shared_ptr<NonLinearComponent> nonLinear= dynamic_pointer_cast<NonLinearComponent>(node->getComponent());
	nonLinear->setNum(num);
	nonLinear->setVisualRow(visualRow);
	nonLinear->setVisualColumn(visualColumn);
	nonLinear->setHiddenRow(hiddenRow);
	nonLinear->setHiddenColumn(hiddenColumn);
}
vector<shared_ptr<ComponentNode>> ConvolutionalNetworkLayer::getAllComponents(){
	if (!sorted){
		topoSort();
	}
	return this->nodes;
}