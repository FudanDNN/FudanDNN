#include "Model.h"
Model::Model()
{
	this->network = shared_ptr<Network>(new Network());
}

size_t Model::addLinearLayerToNetwork(size_t visualUnit, size_t hiddenUnit, size_t num, size_t init_scheme, size_t solver_type,
	double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate, double dropoutLayer)
{
	return this->network->addLinearLayer(visualUnit, hiddenUnit, num, init_scheme, solver_type,
		regularizationRate, weightLearningRate, biasLearningRate, momentumRate, dropoutLayer);
}

size_t Model::addConvolutionLayerToNetwork(size_t rowSize, size_t columnSize, size_t krowSize, size_t kcolumnSize, size_t visualSize,
	size_t hiddenSize, size_t stride, size_t initScheme, size_t solver_type, double regularizationRate,
	double weightLearningRate, double biasLearningRate, double momentumRate, double dropoutLayer)
{
	return this->network->addConvolutionLayer(rowSize, columnSize, krowSize, kcolumnSize, visualSize,
		hiddenSize, stride, initScheme, solver_type, regularizationRate,
		weightLearningRate, biasLearningRate, momentumRate, dropoutLayer);
}

size_t Model::addMaxPoolingLayerToNetwork(size_t rowSize, size_t columnSize, size_t krowSize, size_t kcolumnSize, size_t visualSize,
	size_t hiddenSize, size_t stride, double dropoutRate){
	return this->network->addMaxPoolingLayer(rowSize, columnSize, krowSize, kcolumnSize, visualSize,
		hiddenSize, stride, dropoutRate);
}

size_t Model::addConcatLayerToNetwork(size_t visualRow, size_t visualColumn, size_t visualSize){
	return this->network->addConcatLayer(visualRow, visualColumn, visualSize);
}

size_t Model::addNonlinearLayerToNetwork(size_t visualUnit, size_t type, double s, double lb, double ub, double prec, double ic)
{
	return this->network->addNonlinearLayer(visualUnit, type, s, lb, ub, prec, ic);
}

size_t Model::addEdgeToNetwork(size_t inID, size_t outID)
{
	return this->network->addEdge(inID, outID);
}


void Model::removeNodeInNetwork(size_t id)
{
	this->network->removeNode(id);
}

size_t Model::addInput(size_t type, string fileName)
{
	shared_ptr<Input> input;
	switch (type){
	case XMLINPUT:
		input = shared_ptr<XMLInput>(new XMLInput(fileName, currentInputId));
	}
	inputMap.insert(Input_Pair(currentInputId, input));
	currentInputId++;
	this->inputs.push_back(input);

	return currentInputId - 1;
}

size_t Model::linkInputToNetwork(size_t inputId,size_t layerId)
{

	shared_ptr<Input> input = this->inputMap.find(inputId)->second;
	network->addInput(input->getId());
	network->addInputEdge(inputId, layerId);

	return 0;
}

void Model::setCriteria(size_t type, size_t unit)
{
	switch (type)
	{
	case MSE:
		this->criteria = shared_ptr<Mse>(new Mse(unit));
		break;
	case SOFTMAX:
		this->criteria = shared_ptr<Softmax>(new Softmax(unit));
		break;
	}
}

void Model::run()
{
	network->topoSort();
	int sampleNum = inputs[0]->getSampleNum();
	int totalTimes = this->trainingTimes * sampleNum;
	int correctNum = 0;
	double error = 0;
	for (int i = 0; i < totalTimes; i++){
		shared_ptr<Matrix> output;
		for (shared_ptr<Input> input : inputs){
			shared_ptr<Sample> sample = input->getNextSample();
			output = sample->getOutput();
			network->setInputMat(input->getId(), sample->getInput());
		}
		network->trainingForward();

		this->criteria->setPredictValue(network->getFinalValue()[0]);
		this->criteria->setExpectedValue(output);

		double singleError = this->criteria->computeError();
		error += singleError;

		this->criteria->gradient();

		if (this->criteria->getPredictType() == this->criteria->getExpectedType()){
			correctNum++;
		}
		if (i % sampleNum == 0){
			cout << "times:" << i / sampleNum << 
				"error:" << error << "correctRate:" << ((double)correctNum) / sampleNum << endl;
			error = 0;
			correctNum = 0;
		}

		vector<shared_ptr<Matrix>> tempVec;
		tempVec.push_back(this->criteria->getPredictGradient());
		network->setFinalGradient(tempVec);

		network->backward();

		if (i%batchSize == 0){
			network->update();
		}
	}
}

void Model::setTrainingTimes(size_t trainingTimes)
{
	this->trainingTimes = trainingTimes;
}

void Model::setBatchSize(size_t batchSize)
{
	this->batchSize = batchSize;
}