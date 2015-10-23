#ifndef HYPERPARAMETERCONTROL_H

#define HYPERPARAMETERCONTROL_H
#include <string>
#include <vector>
#include <kernel/MasterControl.h>
/* hyperparameter控制类，是一个纯虚类，根据具体模型决定*/
class HyperParameterControl{
public :
    virtual void outputData(std::ostream &) {}
    virtual void inputData(std::ostream &){}
    virtual void parseData(MasterControl &) {}
    shared_ptr<NetworkNode> associatedNode;
    void setAssociatedNode(shared_ptr<NetworkNode> node){
        associatedNode = node;
    }
    shared_ptr<NetworkNode> getAssociatedNode(){
        return associatedNode;
    }
    HyperParameterControl(){
        associatedNode = nullptr;
    };
};
/* 输入层参数 */
class InputLayerHyperParameterControl : public HyperParameterControl{
private:
	std::string file;
public:
    InputLayerHyperParameterControl() :HyperParameterControl(){

	}
	virtual void setFile(std::string file){ this->file = file; }
	std::string getFile(){ return file; }
    void outputData(std::ostream &) override{}
    void inputData(std::ostream &) override{}
    void parseData(MasterControl &) override{}
};

/* 线性层参数 */
class LinearLayerHyperParameterControl : public HyperParameterControl{
private:
    size_t visualUnitCount, hiddenUnitCount;
    double regularizationRate = 0;
    double weightLearningRate = 0.01;
    double biasLearningRate = 0.01;
    double momentumRate = 0.5;
    AbstractNetworkLayer::initializationScheme initScheme;
public:
	LinearLayerHyperParameterControl(): LinearLayerHyperParameterControl(0,0,AbstractNetworkLayer::RANDOM){

	}
    LinearLayerHyperParameterControl(size_t visualUnitCount, size_t hiddenUnitCount, AbstractNetworkLayer::initializationScheme initScheme) :
        HyperParameterControl(),visualUnitCount(visualUnitCount), hiddenUnitCount(hiddenUnitCount), initScheme(initScheme)
	{}
	void setVisualUnitCount(size_t visualUnitCount){
		this->visualUnitCount = visualUnitCount;
	}
	void setHiddenUnitCount(size_t hiddenUnitCount){
		this->hiddenUnitCount = hiddenUnitCount;
	}
    void setInitScheme(AbstractNetworkLayer::initializationScheme initScheme){
		this->initScheme = initScheme;
	}
    void setRegularizationRate(double d){ this->regularizationRate = d; }
    void setWeightLearningRate(double d){ this->weightLearningRate = d; }
    void setBiasLearningRate(double d){ this->biasLearningRate = d; }
    void setMomentumRate(double d){ this->momentumRate = d; }

	size_t getVisualUnitCount(){ return this->visualUnitCount; }
	size_t getHiddenUnitCount(){ return this->hiddenUnitCount; }
	AbstractNetworkLayer::initializationScheme getInitScheme(){ return this->initScheme; }
    double getRegularizationRate(){ return this->regularizationRate; }
    double getWeightLearningRate(){ return this->weightLearningRate; }
    double getBiasLearningRate(){ return this->biasLearningRate; }
    double getMomentumRate(){ return this->momentumRate; }

	void outputData(std::ostream &) override{}
	void inputData(std::ostream &) override{}
	void parseData(MasterControl &) override{}
};

/* NonLinear 层参数*/
class NonLinearLayerHyperParameterControl : public HyperParameterControl{
private:
    size_t size;
    NonLinearFactory::NONLINEAR type;
public:
    NonLinearLayerHyperParameterControl() : NonLinearLayerHyperParameterControl(0,NonLinearFactory::SIGMOID){}
    NonLinearLayerHyperParameterControl(size_t size, NonLinearFactory::NONLINEAR type) :HyperParameterControl(), size(size), type(type){}
    virtual void setSize(int s){ size = s; }
    virtual size_t getSize(){ return this->size; }
    virtual void setType(NonLinearFactory::NONLINEAR t){ type = t; }
    virtual NonLinearFactory::NONLINEAR getType(){ return type; }
    void outputData(std::ostream &) override{}
    void inputData(std::ostream &) override{}
    void parseData(MasterControl &) override{}
};

/* CNN子网络参数 */
class CNNSubnetHyperParameterControl : public HyperParameterControl{
private:
	size_t cnnId;
public:
    CNNSubnetHyperParameterControl() :HyperParameterControl(){}
	void setCnnId(size_t id){
		this->cnnId = id;
	}
	size_t getCnnId(){ return cnnId;  }
	void outputData(std::ostream &) override{}
	void inputData(std::ostream &) override{}
	void parseData(MasterControl &) override{}
};
/* CNN Units Abstract Class*/
class CNNUnitHyperParameterControl :public HyperParameterControl{
private:
    shared_ptr<ComponentNode> associatedCNNNode;
public:
    CNNUnitHyperParameterControl() : HyperParameterControl(){
        associatedCNNNode = nullptr;
    }
    virtual size_t getCnnId() = 0;
    virtual void setCnnId(size_t)=0;
    void setAssociatedCNNNode(shared_ptr<ComponentNode> node){
        associatedCNNNode = node;
    }
    shared_ptr<ComponentNode> getAssociatedCNNNode(){
        return associatedCNNNode;
    }
};

class CNNIOHyperParameterControl :public CNNUnitHyperParameterControl{
private:
    size_t cnnId;
public :
    CNNIOHyperParameterControl(size_t cnnId) :CNNUnitHyperParameterControl(), cnnId(cnnId){}
    size_t getCnnId(){ return cnnId; }
    void setCnnId(size_t id){ cnnId = id; }
};

/* CNN Convolution Layer*/
class CNNConvolutionLayerHyperParameterControl : public CNNUnitHyperParameterControl{
private:
    size_t kernelSize;
    size_t stride;
    size_t featureMap_Num;
    size_t num;
    size_t visualRow;
    size_t visualColumn;
    double regularizationRate = 0.0001;
    double kernelLearningRate = 0.01;
    double biasLearningRate = 0.01;
    double momentumRate = 0.5;
    AbstractNetworkLayer::initializationScheme scheme;
    size_t cnnId;
public:
    CNNConvolutionLayerHyperParameterControl(size_t kernelSize, size_t stride, size_t featureMapNum, size_t num,
        size_t visualRow, size_t visualColumn, AbstractNetworkLayer::initializationScheme initScheme, size_t cnnId)
        :CNNUnitHyperParameterControl(), kernelSize(kernelSize), stride(stride), featureMap_Num(featureMapNum), num(num), visualRow(visualRow), visualColumn(visualColumn), scheme(initScheme), cnnId(cnnId)
    {

    }
    CNNConvolutionLayerHyperParameterControl(size_t cnnId) 
        :CNNConvolutionLayerHyperParameterControl(0,0,0,0,0,0,AbstractNetworkLayer::RANDOM,cnnId){}
    void setKernelSize(size_t s){kernelSize = s;}
    void setStride(size_t s){ stride = s; }
    void setFeatureMapNum(size_t s){ featureMap_Num = s; }
    void setNum(size_t s){ num = s; }
    void setVisualRow(size_t s){ visualRow = s; }
    void setVisualColumn(size_t s){ visualColumn = s; }
    void setInitScheme(AbstractNetworkLayer::initializationScheme s){scheme = s;}
    void setCnnId(size_t cnnId) override {this->cnnId = cnnId;}
    void setRegularizationRate(double d){ this->regularizationRate = d; }
    void setKernelLearningRate(double d){ this->kernelLearningRate = d; }
    void setBiasLearningRate(double d){ this->biasLearningRate = d; }
    void setMomentumRate(double d){ this->momentumRate = d; }
    double getRegularizationRate(){ return this->regularizationRate; }
    double getKernelLearningRate(){ return this->kernelLearningRate; }
    double getBiasLearningRate(){ return this->biasLearningRate; }
    double getMomentumRate(){ return this->momentumRate; }
    size_t getKernelSize(){ return kernelSize; }
    size_t getStride(){ return stride; }
    size_t getFeatureMapNum(){ return featureMap_Num; }
    size_t getNum(){ return num; }
    size_t getVisualRow(){ return visualRow; }
    size_t getVisualColumn(){return visualColumn;}
    AbstractNetworkLayer::initializationScheme getInitScheme(){ return scheme; }
    size_t getCnnId() override { return cnnId; }

};

/* CNN MaxPooling Layer*/
class CNNMaxPoolingLayerHyperParameterControl : public CNNUnitHyperParameterControl{
private:
    size_t poolingSize;
    size_t stride;
    size_t num;
    size_t visualRow;
    size_t visualColumn;
    size_t cnnId;
public:
    CNNMaxPoolingLayerHyperParameterControl(size_t poolingSize, size_t stride, size_t num,
        size_t visualRow, size_t visualColumn, size_t cnnId)
        :CNNUnitHyperParameterControl(),poolingSize(poolingSize), stride(stride), num(num), visualRow(visualRow), visualColumn(visualColumn), cnnId(cnnId)
    {

    }
    CNNMaxPoolingLayerHyperParameterControl(size_t cnnId)
        :CNNMaxPoolingLayerHyperParameterControl( 0, 0, 0, 0, 0, cnnId){}
    void setPoolingSize(size_t s){ poolingSize = s; }
    void setStride(size_t s){ stride = s; }
    void setNum(size_t s){ num = s; }
    void setVisualRow(size_t s){ visualRow = s; }
    void setVisualColumn(size_t s){ visualColumn = s; }
    void setCnnId(size_t cnnId) override { this->cnnId = cnnId; }

    size_t getPoolingSize(){ return poolingSize; }
    size_t getStride(){ return stride; }
    size_t getNum(){ return num; }
    size_t getVisualRow(){ return visualRow; }
    size_t getVisualColumn(){ return visualColumn; }
    size_t getCnnId() override { return cnnId; }
};

/* CNN NonLinear Layer*/
class CNNNonLinearLayerHyperParameterControl : public CNNUnitHyperParameterControl{
private:
    size_t num;
    size_t visualRow;
    size_t visualColumn;
    NonLinearFactory::NONLINEAR type;
    size_t cnnId;
public:
    CNNNonLinearLayerHyperParameterControl(size_t num, size_t visualRow, size_t visualColumn, NonLinearFactory::NONLINEAR type, size_t cnnId)
        :CNNUnitHyperParameterControl(),num(num), visualRow(visualRow), visualColumn(visualColumn), type(type), cnnId(cnnId)
    {
    }
    CNNNonLinearLayerHyperParameterControl(size_t cnnId)
        :CNNNonLinearLayerHyperParameterControl(0, 0, 0, NonLinearFactory::SIGMOID , cnnId){}
    void setNum(size_t s){ num = s; }
    void setVisualRow(size_t s){ visualRow = s; }
    void setVisualColumn(size_t s){ visualColumn = s; }
    void setType(NonLinearFactory::NONLINEAR s){ type = s; }
    void setCnnId(size_t cnnId) override { this->cnnId = cnnId; }

    size_t getNum(){ return num; }
    size_t getVisualRow(){ return visualRow; }
    size_t getVisualColumn(){ return visualColumn; }
    NonLinearFactory::NONLINEAR getType(){ return type; }
    size_t getCnnId() override { return cnnId; }

};
#endif