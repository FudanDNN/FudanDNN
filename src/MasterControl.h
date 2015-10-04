#ifndef __FUDANDNN_MASTERCONTROL_H
#define __FUDANDNN_MASTERCONTROL_H
#include "utils.h"
#include "AbstractLayer.h"
#include "NetworkNode.h"
#include "RBM.h"
#include "ConvolutionalNetworkLayer.h"
#include "AutoEncoder.h"
#include "SparseAutoEncoder.h"
#include "DenoiseAutoEncoder.h"
#include "ContractiveAutoEncoder.h"
#include "Sigmoid.h"
#include "Type.h"
#include "MSE.h"
#include "NonLinearLayer.h"
#include "LinearLayer.h"
#include "NonLinearFactory.h"
#include "XMLDataParser.h"
#include "Criteria.h"
#include "Input.h"
#include "HardShrink.h"
#include "HardTanH.h"
#include "LogSigmoid.h"
#include "PReLU.h"
#include "ReLU.h"
#include "SoftShrink.h"

typedef pair <size_t, shared_ptr<NetworkNode>> Node_Pair;
typedef pair <size_t, shared_ptr<Input>> Input_Pair;
typedef pair <size_t, shared_ptr<XMLDataParser>> Parser_Pair;

class MasterControl
{
public:
	size_t addRBM(size_t visualUnit, size_t hiddenUnit, size_t initScheme, size_t type, bool sparse,
		size_t batchSize, size_t preTrainingTimes, size_t nonLinearType);
	size_t addAutoEncoder(size_t visualUnit, size_t hiddenUnit, size_t initScheme,
		size_t batchSize, size_t preTrainingTimes, size_t nonLinearType);
	size_t addSparseAutoEncoder(size_t visualUnit, size_t hiddenUnit, size_t init_scheme,
		size_t nonlinearType, size_t sparseRate, size_t sparsePercent, size_t preTrainingTimes, size_t batchSize);
	size_t addContractiveAutoEncoder(size_t visualUnit, size_t hiddenUnit, size_t init_scheme,
		size_t nonlinearType, size_t jacobiPenalty, size_t preTrainingTimes, size_t batchSize);
	size_t addDenoiseAutoEncoder(size_t visualUnit, size_t hiddenUnit, size_t init_scheme, size_t nonlinearType
		, size_t clearRate, size_t preTrainingTimes, size_t batchSize);
	size_t addCNN(size_t dimension, size_t kernelSize, size_t stride, size_t featureMapNum, size_t type);
	size_t addLINEAR(size_t visualUnit, size_t hiddenUnit, size_t init_scheme);
	size_t addNONLINEAR(size_t visualUnit, size_t type);
	size_t addEdge(size_t inID, size_t outID);
	size_t addCNN();
	size_t addCNN2DComponentToCNN(size_t kernelSize, size_t stride, size_t featureMap_Num,
		size_t num, size_t visualRow, size_t visualColumn, size_t scheme, size_t cnnId);
	size_t addMaxPoolingComponentToCNN(size_t poolingSize, size_t stride, size_t visualRow,
		size_t visualColumn, size_t num, size_t cnnId);
	size_t addNonLinearToCNN(int visualRow, int visualColumn, size_t num, size_t type, size_t cnnId);
	size_t addEdgeInCNN(size_t inId, size_t outId, size_t cnnId);
	size_t addInputEdge(size_t inputID, size_t outID);
	size_t addInput(string file);
	size_t setCriteria(size_t type, size_t unitNum);
	bool checkNetwork();
	void run();
	void preTraining();
	void caculateFromTop(shared_ptr<NetworkNode> node, shared_ptr<AbstractMatrix> matrix);
	bool saveToConfig(string path);
	void topoSort();
	void setTrainingTimes(size_t trainingTimes);
	void setBatchSize(size_t batchSize);
	void writeSelf(string);
	void readSelf(string);

private:
	vector<shared_ptr<NetworkNode>> nodes;
	vector<shared_ptr<Input>> inputs;
	size_t currentId;
	map<size_t, shared_ptr<Input>> inputMap;
	map<size_t, shared_ptr<NetworkNode>> idMap;
	map<size_t, size_t> inDegreeMap;
	map<size_t, shared_ptr<XMLDataParser>> dataFileMap;
	shared_ptr<Criteria> criteria;
	size_t trainingTimes;
	size_t batchSize;

};
#endif