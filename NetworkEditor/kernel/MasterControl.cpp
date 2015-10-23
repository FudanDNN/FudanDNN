#include "MasterControl.h"
#include "../trainjob.h"
using namespace std;

size_t MasterControl::addRBM(size_t visualUnit, size_t hiddenUnit, size_t initScheme,
	size_t type, bool sparse, size_t batchSize, size_t preTrainingTimes, size_t nonLinearType){
	shared_ptr<RBM> rbm;
	switch (type){
	case Type::RBM:
		rbm = shared_ptr<RBM>(new RBM(visualUnit, hiddenUnit, nonLinearType, sparse, initScheme));
		rbm->setSelfBatchSize(batchSize);
		break;
	default:
		rbm = shared_ptr<RBM>(new RBM(visualUnit, hiddenUnit, nonLinearType, sparse, initScheme));
		rbm->setSelfBatchSize(batchSize);
		break;
	}
	shared_ptr<NetworkNode> node(new NetworkNode(currentId, rbm));
	idMap.insert(Node_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t MasterControl::addAutoEncoder(size_t visualUnit, size_t hiddenUnit, size_t initScheme,
	size_t batchSize, size_t preTrainingTimes, size_t nonLinearType){
	shared_ptr<AutoEncoder> autoencoder;
	autoencoder = shared_ptr<AutoEncoder>(new AutoEncoder(visualUnit, hiddenUnit, initScheme, nonLinearType));
	autoencoder->setSelfBatchSize(batchSize);
	autoencoder->setPreTrainingTime(preTrainingTimes);
	shared_ptr<NetworkNode> node(new NetworkNode(currentId, autoencoder));
	idMap.insert(Node_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t MasterControl::addSparseAutoEncoder(size_t visualUnit, size_t hiddenUnit, size_t init_scheme,
	size_t nonlinearType, size_t sparseRate, size_t sparsePercent, size_t preTrainingTimes, size_t batchSize){
	shared_ptr<SparseAutoEncoder> autoencoder;
	autoencoder = shared_ptr<SparseAutoEncoder>(new SparseAutoEncoder(visualUnit, hiddenUnit, init_scheme,
		nonlinearType, sparseRate, sparsePercent));
	autoencoder->setSelfBatchSize(batchSize);
	autoencoder->setPreTrainingTime(preTrainingTimes);
	shared_ptr<NetworkNode> node(new NetworkNode(currentId, autoencoder));
	idMap.insert(Node_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t MasterControl::addContractiveAutoEncoder(size_t visualUnit, size_t hiddenUnit, size_t init_scheme,
	size_t nonlinearType, size_t jacobiPenalty, size_t preTrainingTimes, size_t batchSize){
	shared_ptr<ContractiveAutoEncoder> autoencoder;
	autoencoder = shared_ptr<ContractiveAutoEncoder>(new ContractiveAutoEncoder(visualUnit, hiddenUnit, init_scheme,
		nonlinearType, jacobiPenalty));
	autoencoder->setSelfBatchSize(batchSize);
	autoencoder->setPreTrainingTime(preTrainingTimes);
	shared_ptr<NetworkNode> node(new NetworkNode(currentId, autoencoder));
	idMap.insert(Node_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t MasterControl::addDenoiseAutoEncoder(size_t visualUnit, size_t hiddenUnit, size_t init_scheme,
	size_t nonlinearType, size_t clearRate, size_t preTrainingTimes, size_t batchSize){
	shared_ptr<DenoiseAutoEncoder> autoencoder;
	autoencoder = shared_ptr<DenoiseAutoEncoder>(new DenoiseAutoEncoder(visualUnit, hiddenUnit, init_scheme,
		nonlinearType, clearRate));
	autoencoder->setSelfBatchSize(batchSize);
	autoencoder->setPreTrainingTime(preTrainingTimes);
	shared_ptr<NetworkNode> node(new NetworkNode(currentId, autoencoder));
	idMap.insert(Node_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t MasterControl::addCNN(size_t dimension, size_t kernelSize, size_t stride,
	size_t featureMapNum, size_t type){

	return 0;
}

size_t MasterControl::addInput(string file){
	shared_ptr<Input> input(new Input(file, currentId));
	inputMap.insert(Input_Pair(currentId, input));
	currentId++;
	inputs.push_back(input);
	return input->getId();
}

size_t MasterControl::addLINEAR(size_t visualUnit, size_t hiddenUnit, size_t init_scheme){
	shared_ptr<LinearLayer> linearLayer;
	linearLayer = shared_ptr<LinearLayer>(new LinearLayer(visualUnit, hiddenUnit, init_scheme));
	shared_ptr<NetworkNode> node(new NetworkNode(currentId, linearLayer));
	idMap.insert(Node_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}
size_t MasterControl::addLINEAR(size_t visualUnit, size_t hiddenUnit, size_t init_scheme, 
    double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate){
    shared_ptr<LinearLayer> linearLayer;
    linearLayer = shared_ptr<LinearLayer>(new LinearLayer(visualUnit, hiddenUnit, init_scheme));
    linearLayer->setRegularizationRate(regularizationRate);
    linearLayer->setWeightLearningRate(weightLearningRate);
    linearLayer->setBiasLearningRate(biasLearningRate);
    linearLayer->setMomentumRate(momentumRate);
    shared_ptr<NetworkNode> node(new NetworkNode(currentId, linearLayer));
    idMap.insert(Node_Pair(currentId, node));
    currentId++;
    nodes.push_back(node);
    return node->getId();
}

size_t MasterControl::addNONLINEAR(size_t visualUnit, size_t type){
	shared_ptr<NonLinearLayer> nonlinearLayer;
	shared_ptr<NonLinearFactory> factory(new NonLinearFactory());
	nonlinearLayer = factory->createNonLinear(type, visualUnit);
	shared_ptr<NetworkNode> node(new NetworkNode(currentId, nonlinearLayer));
	idMap.insert(Node_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t MasterControl::addEdge(size_t inID, size_t outID){
	shared_ptr<NetworkNode> inNode = idMap.find(inID)->second;
	shared_ptr<NetworkNode> outNode = idMap.find(outID)->second;
	inNode->addNextNode(outNode);
	outNode->addBeforeNode(inNode);
	return 0;
}

size_t MasterControl::addInputEdge(size_t inputID, size_t outID){
	shared_ptr<NetworkNode> outNode = idMap.find(outID)->second;
	outNode->addInput(inputMap.find(inputID)->second);
	inputMap.find(inputID)->second->setNodeId(outID);
	return 0;
}

size_t MasterControl::addCNN(){
	shared_ptr<ConvolutionalNetworkLayer> cnn;
	cnn = shared_ptr<ConvolutionalNetworkLayer>(new ConvolutionalNetworkLayer());
	shared_ptr<NetworkNode> node(new NetworkNode(currentId, cnn));
	idMap.insert(Node_Pair(currentId, node));
	currentId++;
	nodes.push_back(node);
	return node->getId();
}

size_t MasterControl::addNode(shared_ptr<NetworkNode> node){
    node->setId(currentId);
	idMap.insert(Node_Pair(currentId, node));
	nodes.push_back(node);
	currentId++;
	return node->getId();
}

size_t MasterControl::addComponentInCNN(size_t cnn_id, shared_ptr<ComponentNode> component){
	shared_ptr<NetworkNode> node = idMap.find(cnn_id)->second;
	shared_ptr<ConvolutionalNetworkLayer> cnn = dynamic_pointer_cast<ConvolutionalNetworkLayer>(node->getLayer());
	return cnn->addComponent(component);
}

size_t MasterControl::addCNN2DComponentToCNN(size_t kernelSize, size_t stride, size_t featureMap_Num,
	size_t num, size_t visualRow, size_t visualColumn, size_t scheme,  size_t cnnId){
	shared_ptr<NetworkNode> node = idMap.find(cnnId)->second;
	shared_ptr<ConvolutionalNetworkLayer> cnn = dynamic_pointer_cast<ConvolutionalNetworkLayer>(node->getLayer());
	return cnn->addCNN2DComponentToCNN(kernelSize, stride, featureMap_Num, num,
		visualRow, visualColumn, scheme);
}
size_t MasterControl::addCNN2DComponentToCNN(size_t kernelSize, size_t stride, size_t featureMap_Num,
    size_t num, size_t visualRow, size_t visualColumn, size_t scheme, 
    double regualrizationRate, double kernelLearningRate, double biasLearningRate, 
    double momentumRate, size_t cnnId){
    shared_ptr<NetworkNode> node = idMap.find(cnnId)->second;
    shared_ptr<ConvolutionalNetworkLayer> cnn = dynamic_pointer_cast<ConvolutionalNetworkLayer>(node->getLayer());
    return cnn->addCNN2DComponentToCNN(kernelSize, stride, featureMap_Num, num,
        visualRow, visualColumn, scheme, regualrizationRate, kernelLearningRate, biasLearningRate, momentumRate);
}

size_t MasterControl::addMaxPoolingComponentToCNN(size_t poolingSize, size_t stride,
	size_t visualRow, size_t visualColumn, size_t num, size_t cnnId){
	shared_ptr<NetworkNode> node = idMap.find(cnnId)->second;
	shared_ptr<ConvolutionalNetworkLayer> cnn = dynamic_pointer_cast<ConvolutionalNetworkLayer>(node->getLayer());
	return cnn->addMaxPoolingToCNN(poolingSize, stride, visualRow, visualColumn, num);
}

size_t MasterControl::addNonLinearToCNN(int visualRow, int visualColumn, size_t num, size_t type, size_t cnnId){
	shared_ptr<NetworkNode> node = idMap.find(cnnId)->second;
	shared_ptr<ConvolutionalNetworkLayer> cnn = dynamic_pointer_cast<ConvolutionalNetworkLayer>(node->getLayer());
	return cnn->addNonLinearToCNN(visualRow, visualColumn, num, type);
}

size_t MasterControl::addEdgeInCNN(size_t inId, size_t outId, size_t cnnId){
	shared_ptr<NetworkNode> node = idMap.find(cnnId)->second;
	shared_ptr<ConvolutionalNetworkLayer> cnn = dynamic_pointer_cast<ConvolutionalNetworkLayer>(node->getLayer());
	return cnn->addEdgeInCNN(inId, outId);
}

vector<shared_ptr<NetworkNode>> MasterControl::getAllNodes(){
	if (!sorted){
		topoSort();
	}
	return this->nodes;
}

size_t MasterControl::setCriteria(size_t type){
	switch (type){
	case 0:
		this->criteria = shared_ptr<MSE>(new MSE());
		break;
	case 1:
		this->criteria = shared_ptr<Softmax>(new Softmax());
		break;
	}
	return 0;
}

void MasterControl::setTrainingTimes(size_t trainingTimes){
	this->trainingTimes = trainingTimes;
}

void MasterControl::setBatchSize(size_t batchSize)
{
	this->batchSize = batchSize;
}

void MasterControl::removeNode(size_t id){
	shared_ptr<NetworkNode> node = idMap.find(id)->second;
	auto itr = nodes.cbegin();
	while (itr != nodes.end()){
		if ((*itr) == node) break;
	}
	nodes.erase(itr);
}

void MasterControl::removeNodeInCNN(size_t id, size_t cnnId){

}

void MasterControl::removeEdge(size_t inID, size_t outID){

}

void MasterControl::removeEdgeInCNN(size_t inId, size_t outId, size_t cnnId){

}


void MasterControl::init(){
	for (int i = 0; i < nodes.size(); i++){
		shared_ptr<AbstractNetworkLayer> networkLayer = dynamic_pointer_cast<AbstractNetworkLayer>(nodes[i]->getLayer());
		if (networkLayer!=nullptr)
			networkLayer->init();
	}
}

void MasterControl::predict(string fileName){
    workingThread->emitMessage("Predicting...\n");
	ofstream fout(fileName);

	size_t sampleNum = inputs[0]->getSampleNum();
	for (size_t times = 0; times < sampleNum; times++)
	{

		for (shared_ptr<NetworkNode> node : nodes)
		{
			size_t id = node->getId();

			shared_ptr<AbstractLayer> layer = node->getLayer();

			shared_ptr<AbstractMatrix> visualValue = nullptr;
			for (shared_ptr<NetworkNode> pred : node->getBeforeNode())
			{
				if (visualValue == nullptr){
					visualValue = pred->getLayer()->getHiddenValue();
				}
				else{
					visualValue = visualValue->mergeRow(pred->getLayer()->getHiddenValue());
				}
			}

			if (node->getInput() != nullptr)
			{
				if (node->getInput()->isEnd())
				{

					for (shared_ptr<NetworkNode> node : nodes)
					{
						shared_ptr<AbstractLayer> layer = node->getLayer();
						shared_ptr<AbstractNetworkLayer> networkLayer = dynamic_pointer_cast<AbstractNetworkLayer>(layer);
						if (networkLayer)
							networkLayer->update();
					}

				}
				shared_ptr<TrainingSample> sample = node->getInput()->getNextTrainingSample();
				if (visualValue == nullptr){
					visualValue = sample->getInput();
				}
				else{
					visualValue = visualValue->mergeRow(sample->getInput());
				}
			}
			layer->setVisualValue(visualValue);
			layer->calculate();
		}

		shared_ptr<AbstractLayer> lastLayer = nodes.at(nodes.size() - 1)->getLayer();

		this->criteria->setPredictValue(lastLayer->getHiddenValue());

		shared_ptr<AbstractMatrix> ans = this->criteria->getAnswer();
		double max = -100;
		size_t maxIndex = 0;
		for (size_t i = 0; i < ans->getRowSize(); i++)
		{
			if (ans->getValue(i, 0)>max){
				maxIndex = i + 1;
				max = ans->getValue(i, 0);
			}
		}
		fout <<" maxIndex : " << maxIndex <<" ";
		for (size_t j = 0; j < ans->getColumnSize(); j++)
		{
			for (size_t i = 0; i < ans->getRowSize(); i++)
			{
				fout << ans->getValue(i, j) << ' ';
			}
			fout << endl;
		}

	}

	fout.close();
    workingThread->emitMessage("Finished predicting!\n");

}
void MasterControl::run()
{
	init();
	topoSort();
	//preTraining();
	size_t times = 0;
	size_t sampleNum = inputs[0]->getSampleNum();
	double error = 0;
	double norm2 = 0;
	int correctNum = 0;
	int totalTimes = trainingTimes*sampleNum;
	while (times < totalTimes)
	{
        while (dynamic_cast<TrainJob*>(workingThread)->getPauseFlag()){}
		times++;
		bool match = true;
		shared_ptr<AbstractMatrix> expectedOutput = nullptr;

		for (shared_ptr<NetworkNode> node : nodes)
		{
			size_t id = node->getId();

			shared_ptr<AbstractLayer> layer = node->getLayer();

			shared_ptr<AbstractMatrix> visualValue = nullptr;
			for (shared_ptr<NetworkNode> pred : node->getBeforeNode())
			{
				if (visualValue == nullptr){
					visualValue = pred->getLayer()->getHiddenValue();
				}
				else{
					visualValue = visualValue->mergeRow(pred->getLayer()->getHiddenValue());
				}
			}

			if (node->getInput() != nullptr)
			{
				if (node->getInput()->isEnd())
				{

					for (shared_ptr<NetworkNode> node : nodes)
					{
						shared_ptr<AbstractLayer> layer = node->getLayer();
						shared_ptr<AbstractNetworkLayer> networkLayer = dynamic_pointer_cast<AbstractNetworkLayer>(layer);
						if (networkLayer)
							networkLayer->update();
					}

				}
				shared_ptr<TrainingSample> sample = node->getInput()->getNextTrainingSample();
				if (visualValue == nullptr){
					visualValue = sample->getInput();
				}
				else{
					visualValue = visualValue->mergeRow(sample->getInput());
				}
				if (nullptr == expectedOutput)
				{
					expectedOutput = sample->getOutput();
				}
				else
				{
					if (!expectedOutput->equals(sample->getOutput()))
					{
						match = false;
						break;
					}
				}
			}
			layer->setVisualValue(visualValue);
			layer->calculate();
		}

		if (!match)
		{
			continue;
		}

		shared_ptr<AbstractLayer> lastLayer = nodes.at(nodes.size() - 1)->getLayer();

		this->criteria->setExpectedValue(expectedOutput);
		this->criteria->setPredictValue(lastLayer->getHiddenValue());
		double singleError = this->criteria->computeError();
		this->criteria->gradient();
		if (this->criteria->getPredictType() == this->criteria->getExpectedType()){
			correctNum++;
		}
		//criteria->getPredictValue()->print();
		error += singleError;
		//cout << "error:" << singleError << endl;
		norm2 += this->criteria->getPredictGradient()->norm2();
		if (times%sampleNum == 0){
            sprintf(outputBuffer, "times: %d error: %lf correctNum:%d\n", times, error, correctNum);
            workingThread->emitMessage(outputBuffer);
            //cout << "times: " << times << "   error: " << error << "   correctNum:" << correctNum << endl;
            error = 0;
			norm2 = 0;
			correctNum = 0;
		}
		//criteria->getPredictGradient()->print();
		//criteria->getPredictGradient()->print();
		lastLayer->setHiddenGradient(this->criteria->getPredictGradient());
		for (int i = nodes.size() - 1; i >= 0; i--)
		{
			shared_ptr<NetworkNode> node = nodes.at(i);
			size_t id = node->getId();

			shared_ptr<AbstractLayer> layer = node->getLayer();

			layer->gradient();

			size_t preLength = 0;
			size_t currLength = 0;

			for (shared_ptr<NetworkNode> pred : node->getBeforeNode())
			{
				shared_ptr<AbstractLayer> predLayer = pred->getLayer();
				currLength = predLayer->getHiddenValue()->getRowSize();
				predLayer->setHiddenGradient(layer->getVisualGradient()->submatrix(preLength, preLength + currLength, 0, 1));
				preLength += currLength;
			}

			if (node->getInput() != nullptr)
			{
				currLength = layer->getVisualGradient()->getRowSize();
				node->getInput()->setGradient(layer->getVisualGradient());
			}

		}

		if (times % batchSize == 0) {
			for (shared_ptr<NetworkNode> node : nodes)
			{
				shared_ptr<AbstractLayer> layer = node->getLayer();
				shared_ptr<AbstractNetworkLayer> networkLayer = dynamic_pointer_cast<AbstractNetworkLayer>(layer);
				if (networkLayer != nullptr)
				{
					networkLayer->update();
				}
			}
		}

	}


}

void MasterControl::preTraining()
{
	for (shared_ptr<Input> input : inputs){
		shared_ptr<NetworkNode> node = idMap.find(input->getNodeId())->second;
		shared_ptr<AbstractLayer> layer = node->getLayer();
		shared_ptr<RBM> rbm = dynamic_pointer_cast<RBM>(layer);
		shared_ptr<AutoEncoder> ae = dynamic_pointer_cast<AutoEncoder>(layer);
		if (rbm != NULL){
			for (size_t i = 0; i < rbm->getSelfTrainingTime(); i++)
			{
				shared_ptr<Sample> sample = nullptr;
				size_t index = 0;
				while (!input->isEnd())
				{
					sample = input->getNextSample();
					index++;
					rbm->singleTraining(sample->getInput());
					if (index % rbm->getSelfBatchSize() == 0)
						rbm->selfUpdate(false);
				}
				rbm->selfUpdate(true);
			}
		}
		else if (ae != NULL){
			for (size_t i = 0; i < ae->getPreTrainingTime(); i++)
			{
				shared_ptr<Sample> sample = nullptr;
				size_t index = 0;
				while (!input->isEnd())
				{
					sample = input->getNextSample();
					index++;
					ae->singleTraining(sample->getInput());
					if (index % ae->getSelfBatchSize() == 0)
						ae->selfUpdate(false);
				}
				ae->selfUpdate(true);
			}
		}
		else{
			continue;
		}
		vector<shared_ptr<NetworkNode>> nextNodes;
		while ((nextNodes = node->getNextNode()).size() != 0){
			shared_ptr<NetworkNode> nextNode = nextNodes[0];

			shared_ptr<AbstractLayer> layer = nextNode->getLayer();

			shared_ptr<RBM> rbm = dynamic_pointer_cast<RBM>(layer);
			shared_ptr<AutoEncoder> ae = dynamic_pointer_cast<AutoEncoder>(layer);

			if (rbm != NULL){
				shared_ptr<Sample> sample = nullptr;
				size_t index = 0;
				while (!input->isEnd())
				{
					sample = input->getNextSample();
					caculateFromTop(nextNode, sample->getInput());
					rbm->singleTraining(nextNode->getLayer()->getVisualValue());
					if (index % rbm->getSelfBatchSize() == 0)
						rbm->selfUpdate(false);
				}
			}
			else if (ae != NULL){
				shared_ptr<Sample> sample = nullptr;
				size_t index = 0;
				while (!input->isEnd())
				{
					sample = input->getNextSample();
					caculateFromTop(nextNode, sample->getInput());
					ae->singleTraining(nextNode->getLayer()->getVisualValue());
					if (index % ae->getSelfBatchSize() == 0)
						ae->selfUpdate(false);
				}
			}
			else{
				break;
			}
			node = node->getNextNode()[0];
		}
	}
}

void MasterControl::caculateFromTop(shared_ptr<NetworkNode> node, shared_ptr<AbstractMatrix> matrix){
	if (node->getInput() == nullptr){
		caculateFromTop(node->getBeforeNode()[0], matrix);
		shared_ptr<AbstractLayer> layer = node->getLayer();
		shared_ptr<AbstractLayer> previousLayer = node->getBeforeNode()[0]->getLayer();
		layer->setVisualValue(previousLayer->getHiddenValue());
		layer->calculate();
	}
	else{
		shared_ptr<AbstractLayer> layer = node->getLayer();
		layer->setVisualValue(matrix);
		layer->calculate();
	}
}

bool MasterControl::saveToConfig(string path){
	return true;
}

void MasterControl::topoSort()
{

	for (shared_ptr<NetworkNode> node : nodes)
	{
		vector<shared_ptr<NetworkNode>> beforeNodes = node->getBeforeNode();
		this->inDegreeMap.insert(InDegree_Pair(node->getId(), beforeNodes.size()));
	}

	size_t i = 0;
	size_t size = nodes.size();
	while (i < size)
	{
		size_t inDegree_i = inDegreeMap.find(nodes[i]->getId())->second;
		if (inDegree_i == 0)
		{
			for (shared_ptr<NetworkNode> succ : nodes[i]->getNextNode())
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

		shared_ptr<NetworkNode> tmp = nodes[i];
		nodes[i] = nodes[j];
		nodes[j] = tmp;

	}
	int criteriaSize = 0;
	for (int i = 0; i < nodes.size(); i++){
		if (nodes[i]->getNextNode().size() == 0)
			criteriaSize += nodes[i]->getLayer()->getHiddenUnit();
	}
	this->criteria->setVisualUnit(criteriaSize);
	sorted = true;
}

int MasterControl::checkNetwork()
{
	if (inputs.size() == 0){
		return NOINPUT;
	}
	if (criteria == nullptr){
		return NOCRITERIA;
	}
	for (shared_ptr<NetworkNode> node : nodes)
	{
		if (node->getLayer()->getNetworkName() == "ConvolutionalNetworkLayer"){
			int ret = dynamic_pointer_cast<ConvolutionalNetworkLayer>(node->getLayer())->checkCNN();
			if (ret!= CORRECT){
				return ret;
			}
		}
		else{
			shared_ptr<AbstractLayer> layer = node->getLayer();
			size_t predTotalInputSize = 0;
			for (shared_ptr<NetworkNode> pred : node->getBeforeNode())
			{
				shared_ptr<AbstractLayer> predLayer = pred->getLayer();
				predTotalInputSize += predLayer->getHiddenUnit();
			}
			if (node->getInput() != nullptr){
				predTotalInputSize += node->getInput()->getInputRow();
			}
			if (layer->getVisualUnit() != predTotalInputSize)
				return WRONGVISUALNUM;
		}
	}
	return CORRECT;
}

void MasterControl::writeSelf(string directory)
{
	_mkdir(directory.c_str());
	ofstream fout(directory + "/MasterControl.net", ios::out);

	fout << nodes.size();
	fout << '\n';
	for (size_t i = 0; i < nodes.size(); i++)
	{
		fout << nodes.at(i)->getLayer()->getNetworkName() << '\n';
		fout << nodes.at(i)->getId() << endl;
		shared_ptr<AbstractLayer> layer = nodes.at(i)->getLayer();
		char *temp = new char[3];
		sprintf_s(temp, 3 * sizeof(char), "%d", i);
		string s(temp);
		layer->writeSelf(directory + "/" + layer->getNetworkName() + s);
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

	fout << criteria->getType() << endl;
	criteria->writeSelf(directory + "/" + criteria->getType());

	fout.close();

}

void MasterControl::readSelf(string directory)
{

	ifstream fin(directory + "/MasterControl.net", ios::in);

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

		shared_ptr<NetworkNode> node;

		if (strcmp(name, "AutoEncoder") == 0)
		{
			shared_ptr<AbstractLayer> network(new AutoEncoder());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "RBM") == 0)
		{
			shared_ptr<AbstractLayer> network(new RBM());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "ConvolutionalNetworkLayer") == 0)
		{
			shared_ptr<AbstractLayer> network(new ConvolutionalNetworkLayer());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "LinearLayer") == 0)
		{
			shared_ptr<AbstractLayer> network(new LinearLayer());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "ContractiveAutoEncoder") == 0)
		{
			shared_ptr<AbstractLayer> network(new ContractiveAutoEncoder());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "DenoiseAutoEncoder") == 0)
		{
			shared_ptr<AbstractLayer> network(new DenoiseAutoEncoder());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "HardShrink") == 0)
		{
			shared_ptr<AbstractLayer> network(new HardShrink());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "HardTanH") == 0)
		{
			shared_ptr<AbstractLayer> network(new HardTanH());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "LogSigmoid") == 0)
		{
			shared_ptr<AbstractLayer> network(new LogSigmoid());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "PReLU") == 0)
		{
			shared_ptr<AbstractLayer> network(new PReLU());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "ReLU") == 0)
		{
			shared_ptr<AbstractLayer> network(new ReLU());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "Sigmoid") == 0)
		{
			shared_ptr<AbstractLayer> network(new Sigmoid());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "SoftShrink") == 0)
		{
			shared_ptr<AbstractLayer> network(new SoftShrink());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
		else if (strcmp(name, "SparseAutoEncoder") == 0)
		{
			shared_ptr<AbstractLayer> network(new SparseAutoEncoder());
			network->readSelf(directory + "/" + name + s);
			node = shared_ptr<NetworkNode>(new NetworkNode(id, network));
		}
        int k = node->getId();
		//idMap.insert(Node_Pair(id, node));
        idMap[id] = node;
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
			this->addEdge(nodes[i]->getId(), id);
		}
	}

	string cType_s;
	fin >> cType_s;
	const char *cType = cType_s.c_str();
	if (strcmp(cType, "MSE") == 0)
	{
		criteria = shared_ptr<MSE>(new MSE());
		criteria->readSelf(directory + "/" + cType);
	}
	else if (strcmp(cType, "Softmax") == 0)
	{
		criteria = shared_ptr<Softmax>(new Softmax());
		criteria->readSelf(directory + "/" + cType);
	}

	fin.close();

}

void MasterControl::setParameterForCNN1DComponent(size_t cnnId,size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn,
	size_t kernelSize, size_t stride, size_t featureMapNum, double regularizationRate, double kernelLearningRate, double biasLearningRate, double momentumRate)
{
	shared_ptr<NetworkNode> node = idMap.find(cnnId)->second;
	shared_ptr<ConvolutionalNetworkLayer> cnn = dynamic_pointer_cast<ConvolutionalNetworkLayer>(node->getLayer());
	cnn->setParameterForCNN1DComponent(id, num, visualRow, visualColumn, hiddenRow, hiddenColumn, kernelSize,
		stride, featureMapNum, regularizationRate, kernelLearningRate, biasLearningRate, momentumRate);
}

void MasterControl::setParameterForCNN2DComponent(size_t cnnId,size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn,
	size_t kernelSize, size_t stride, size_t featureMapNum, double regularizationRate, double kernelLearningRate, double biasLearningRate, double momentumRate)
{
	shared_ptr<NetworkNode> node = idMap.find(cnnId)->second;
	shared_ptr<ConvolutionalNetworkLayer> cnn = dynamic_pointer_cast<ConvolutionalNetworkLayer>(node->getLayer());
	cnn->setParameterForCNN2DComponent(id, num, visualRow, visualColumn, hiddenRow, hiddenColumn, kernelSize,
		stride, featureMapNum, regularizationRate, kernelLearningRate, biasLearningRate, momentumRate);
}

void MasterControl::setParameterForMaxPoolingComponent(size_t cnnId,size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn,
	size_t stride, size_t poolingSize)
{
	shared_ptr<NetworkNode> node = idMap.find(cnnId)->second;
	shared_ptr<ConvolutionalNetworkLayer> cnn = dynamic_pointer_cast<ConvolutionalNetworkLayer>(node->getLayer());
	cnn->setParameterForMaxPoolingComponent(id, num, visualRow, visualColumn, hiddenRow, hiddenColumn,
		stride, poolingSize);
}

void MasterControl::setParameterForNonLinearComponent(size_t cnnId,size_t id, size_t num, size_t visualRow, size_t visualColumn, size_t hiddenRow, size_t hiddenColumn)
{
	shared_ptr<NetworkNode> node = idMap.find(cnnId)->second;
	shared_ptr<ConvolutionalNetworkLayer> cnn = dynamic_pointer_cast<ConvolutionalNetworkLayer>(node->getLayer());
	cnn->setParameterForNonLinearComponent(id, num, visualRow, visualColumn, hiddenRow, hiddenColumn);
}

void MasterControl::setParameterForLinearLayer(size_t id, size_t visualUnit, size_t hiddenUnit, double regularizationRate, double weightLearningRate,
	double biasLearningRate, double dropoutRate, double momentumRate)
{
	shared_ptr<NetworkNode> node = idMap.find(id)->second;
	shared_ptr<LinearLayer> linear = dynamic_pointer_cast<LinearLayer>(node->getLayer());
	linear->setVisualUnit(visualUnit);
	linear->setHiddenUnit(hiddenUnit);
	linear->setRegularizationRate(regularizationRate);
	linear->setWeightLearningRate(weightLearningRate);
	linear->setBiasLearningRate(biasLearningRate);
	linear->setDropoutRate(dropoutRate);
	linear->setMomentumRate(momentumRate);
}

void MasterControl::setParameterForNonLinear(size_t id, size_t visualUnit)
{
	shared_ptr<NetworkNode> node = idMap.find(id)->second;
	shared_ptr<NonLinearLayer> nonlinear = dynamic_pointer_cast<NonLinearLayer>(node->getLayer());
	nonlinear->setVisualUnit(visualUnit);
	nonlinear->setHiddenUnit(visualUnit);
}

void MasterControl::setParameterForCriteria(size_t num)
{
	this->criteria->setVisualUnit(num);
}

/*int main(){
	MasterControl *master = new MasterControl();
	size_t cnnId = master->addCNN();
	//size_t kernelSize, size_t stride,size_t featureMapNum, size_t num, size_t visualRow, size_t visualColumn, size_t scheme
	size_t id1 = master->addCNN2DComponentToCNN(4, 4, 10, 1, 5, 5, 1, cnnId);

	size_t pooling1 = master->addMaxPoolingComponentToCNN(2, 2, 2, 2, 10, cnnId);

	size_t cnnEdge1 = master->addEdgeInCNN(id1, pooling1, cnnId);


	size_t inputId = master->addInput("dataset/digital_20.xml");
	master->addInputEdge(inputId, cnnId);

	size_t layer1 = master->addLINEAR(10, 30, 1);
	size_t non1 = master->addNONLINEAR(30, 1);
	size_t layer2 = master->addLINEAR(30, 10, 1);
	//size_t non2 = master->addNONLINEAR(10, 0);

	master->addEdge(cnnId, layer1);
	master->addEdge(layer1, non1);
	master->addEdge(non1, layer2);
	//master->addEdge(layer2, non2);

	master->setCriteria(1);
	master->setTrainingTimes(1);
	master->setBatchSize(1);
	master->run();
	master->writeSelf("CNN");
	shared_ptr<MasterControl> xxx(new MasterControl());
	xxx->readSelf("CNN");
	size_t newInput = xxx->addInput("dataset/digital_20.xml");
	xxx->addInputEdge(newInput, id1);
	xxx->setBatchSize(1);
	xxx->setTrainingTimes(10000);
	xxx->run();

	while (1);
}*/
/*int main(){
	shared_ptr<MasterControl> master(new MasterControl());
	size_t id1 = master->addLINEAR(13, 50, 0);
	size_t id2 = master->addNONLINEAR(50, NonLinearFactory::SIGMOID);
	size_t id3 = master->addLINEAR(50, 30, 0);
	size_t id4 = master->addNONLINEAR(30, NonLinearFactory::SIGMOID);
	master->addEdge(id1, id2);
	master->addEdge(id2, id3);
	master->addEdge(id3, id4);
	master->setBatchSize(1);
	size_t inputId = master->addInput("dataset/voice.xml");
	master->addInputEdge(inputId, id1);
	master->setCriteria(0);
	master->setTrainingTimes(10);
	master->run();
	master->writeSelf("temp");
	shared_ptr<MasterControl> xxx(new MasterControl());
	xxx->readSelf("temp");
	size_t newInput = xxx->addInput("dataset/voice.xml");
	xxx->addInputEdge(newInput, id1);
	xxx->setBatchSize(1);
	xxx->setTrainingTimes(10000);
	xxx->run();
	while (1);
}*/
/*
linear voice
*/
/*int main(){
shared_ptr<MasterControl> master (new MasterControl());
size_t id1 = master->addLINEAR(13,50,0);
size_t id2 = master->addNONLINEAR(50, NonLinearFactory::SIGMOID);
size_t id3 = master->addLINEAR(50,30,0);
size_t id4 = master->addNONLINEAR(30, NonLinearFactory::SIGMOID);
master->addEdge(id1, id2);
master->addEdge(id2, id3);
master->addEdge(id3, id4);
master->setBatchSize(1);
size_t inputId = master->addInput("dataset/voice.xml");
master->addInputEdge(inputId, id1);
master->setCriteria(0);
master->setTrainingTimes(1000000);
master->run();
while (1);
};*/
/*
linear sin
*/
/*int main(){
shared_ptr<MasterControl> master(new MasterControl());
size_t id1 = master->addLINEAR(1,10,0);
size_t id2 = master->addNONLINEAR(10, NonLinearFactory::SIGMOID);
size_t id3 = master->addLINEAR(10, 1,0);
size_t id4 = master->addNONLINEAR(1, NonLinearFactory::SIGMOID);
master->addEdge(id1, id2);
master->addEdge(id2, id3);
master->addEdge(id3, id4);
master->setBatchSize(1);
size_t inputId = master->addInput("dataset/regression.xml");
master->addInputEdge(inputId, id1);
master->setCriteria(0);
master->setTrainingTimes(42000);
master->run();
while (1);
};*/
/*
cnn mnist 32*32
*/
/*
int main(){
	MasterControl *master = new MasterControl();
	size_t cnnId = master->addCNN();
	//size_t kernelSize, size_t stride,size_t featureMapNum, size_t num, size_t visualRow, size_t visualColumn, size_t scheme
	size_t id1 = master->addCNN2DComponentToCNN(5, 1, 8, 1, 32, 32, 1, cnnId);

	size_t pooling1 = master->addMaxPoolingComponentToCNN(2, 2, 28, 28, 8, cnnId);

	size_t id2 = master->addCNN2DComponentToCNN(5, 1, 16, 8, 14, 14, 1, cnnId);

	size_t pooling2 = master->addMaxPoolingComponentToCNN(2, 2, 10, 10, 16, cnnId);


	size_t cnnEdge1 = master->addEdgeInCNN(id1, pooling1, cnnId);

	size_t cnnEdge3 = master->addEdgeInCNN(pooling1, id2, cnnId);

	size_t cnnEdge4 = master->addEdgeInCNN(id2, pooling2, cnnId);

	size_t inputId = master->addInput("dataset/temp_1_1000.xml");


	master->addInputEdge(inputId, cnnId);

	size_t layer1 = master->addLINEAR(400, 120, 1);
	size_t non1 = master->addNONLINEAR(120, 1);
	size_t layer2 = master->addLINEAR(120, 84, 1);
	size_t non2 = master->addNONLINEAR(84, 1);
	size_t layer3 = master->addLINEAR(84, 10, 1);
	//size_t non3 = master->addNONLINEAR(10, 0);

	master->addEdge(cnnId, layer1);
	master->addEdge(layer1, non1);
	master->addEdge(non1, layer2);
	master->addEdge(layer2, non2);
	master->addEdge(non2, layer3);
	//master->addEdge(layer3, non3);

	master->setCriteria(1);
	master->setTrainingTimes(10);
	master->setBatchSize(1);
	master->run();
	master->writeSelf("CNN");
	shared_ptr<MasterControl> xxx(new MasterControl());
	xxx->readSelf("CNN");
	size_t newInput = xxx->addInput("dataset/mnist32.xml");
	xxx->addInputEdge(newInput, id1);
	xxx->setBatchSize(1);
	xxx->setTrainingTimes(10000);
	xxx->run();
	while (1);
}*/
/*
cnn mnist 28*28
*/
/*int main(){
MasterControl *master = new MasterControl();
size_t cnnId = master->addCNN();
//size_t kernelSize, size_t stride,size_t featureMapNum, size_t num, size_t visualRow, size_t visualColumn, size_t scheme
size_t id1 = master->addCNN2DComponentToCNN(1, 1, 6, 1, 28, 28, 1, cnnId);
size_t nonLinear1 = master->addNonLinearToCNN(28, 28, 6, 1, cnnId);
size_t pooling1 = master->addMaxPoolingComponentToCNN(2, 2, 28, 28, 6, cnnId);

size_t id2 = master->addCNN2DComponentToCNN(5, 1, 16, 6, 14, 14, 1, cnnId);
size_t nonLinear2 = master->addNonLinearToCNN(10, 10, 16, 1, cnnId);
size_t pooling2 = master->addMaxPoolingComponentToCNN(2, 2, 10, 10, 16, cnnId);

size_t id3 = master->addCNN2DComponentToCNN(5, 1, 120, 16, 5, 5, 1, cnnId);
size_t nonLinear3 = master->addNonLinearToCNN(1, 1, 120, 1, cnnId);

size_t cnnEdge1 = master->addEdgeInCNN(id1, nonLinear1, cnnId);
size_t cnnEdge2 = master->addEdgeInCNN(nonLinear1, pooling1, cnnId);
size_t cnnEdge3 = master->addEdgeInCNN(pooling1, id2, cnnId);

size_t cnnEdge4 = master->addEdgeInCNN(id2, nonLinear2, cnnId);
size_t cnnEdge5 = master->addEdgeInCNN(nonLinear2, pooling2, cnnId);
size_t cnnEdge6 = master->addEdgeInCNN(pooling2, id3, cnnId);

size_t cnnEdge7 = master->addEdgeInCNN(id3, nonLinear3, cnnId);

size_t inputId = master->addInput("dataset/temp.xml");
master->addInputEdge(inputId, cnnId);

size_t layer1 = master->addLINEAR(120, 84, 0);
size_t non1 = master->addNONLINEAR(84, 0);
size_t layer2 = master->addLINEAR(84, 10, 0);
size_t non2 = master->addNONLINEAR(10, 0);

master->addEdge(cnnId, layer1);
master->addEdge(layer1, non1);
master->addEdge(non1, layer2);
master->addEdge(layer2, non2);

master->setCriteria(0);
master->setTrainingTimes(100000);
master->setBatchSize(1);
master->run();
while (1);
}*/
/*
cnn 5*5
*/
/*int main(){
MasterControl *master = new MasterControl();
size_t cnnId = master->addCNN();
//size_t kernelSize, size_t stride,size_t featureMapNum, size_t num, size_t visualRow, size_t visualColumn, size_t scheme
size_t id1 = master->addCNN2DComponentToCNN(4, 4, 10, 1, 5, 5, 1, cnnId);

size_t pooling1 = master->addMaxPoolingComponentToCNN(2, 2, 2, 2, 10, cnnId);

size_t cnnEdge1 = master->addEdgeInCNN(id1,pooling1, cnnId);


size_t inputId = master->addInput("dataset/digital_20.xml");
master->addInputEdge(inputId, cnnId);

size_t layer1 = master->addLINEAR(10, 30, 1);
size_t non1 = master->addNONLINEAR(30, 1);
size_t layer2 = master->addLINEAR(30, 10, 1);
//size_t non2 = master->addNONLINEAR(10, 0);

master->addEdge(cnnId, layer1);
master->addEdge(layer1, non1);
master->addEdge(non1, layer2);
//master->addEdge(layer2, non2);

master->setCriteria(1);
master->setTrainingTimes(100000);
master->setBatchSize(1);
master->run();
while (1);
}*/