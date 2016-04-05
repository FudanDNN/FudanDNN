#ifndef HYPERPARAMETERCONTROL_H

#define HYPERPARAMETERCONTROL_H
#include <string>
#include <vector>
#include <memory>
using std::shared_ptr;
#include "include/layer/Solver.h"
#include "Constant.h"
namespace fudandnn{
    class Model;
}
/* hyperparameter控制类，是一个纯虚类，根据具体模型决定*/
class HyperParameterControl{
public :
    virtual void outputData(std::ostream &) {
        throw new std::exception("Not Implemented yet!");
    }
    virtual void inputData(std::ostream &){
        throw new std::exception("Not Implemented yet!");
    }
    virtual int parseInformation(fudandnn::Model & model){
        throw new std::exception("Not Implemented yet!");
    }
	HyperParameterControl(){};
};
class HasSolver{
public:
    virtual void setSolver(shared_ptr<Solver> solver) =0 ;
};
/* the hyperparameter of the whole networks */
class NetworkHyperParameterControl : public HyperParameterControl{
private: 
    fudandnn::SOLVER solver;
    fudandnn::CRITERIA criteria;
    double regularizationRate;
    double learningRate;
    double momentumRate;
    size_t trainingTimes;
    size_t batchSize;
    size_t outputSize;
public:
    NetworkHyperParameterControl() :
        solver(fudandnn::SGD),
        criteria(fudandnn::CRITERIA::MSE),
        regularizationRate(0.0),
        learningRate(0.01),
        momentumRate(0.0),trainingTimes(500), batchSize(1),outputSize(0){
    }
    void setSolver(fudandnn::SOLVER solver){this->solver = solver;}
    void setCriteria(fudandnn::CRITERIA criteria){this->criteria = criteria;}
    void setRegularizationRate(double v){ this->learningRate = v; }
    void setLearningRate(double v){ this->learningRate = v; }
    void setMomentumRate(double v){ this->momentumRate = v; }
    void setTrainingTimes(size_t v) { this->trainingTimes = v; }
    void setBatchSize(size_t v){ this->batchSize = v; }
    void setOutputSize(size_t v){ this->outputSize = v; }
    fudandnn::SOLVER getSolver(){ return this->solver; }
    fudandnn::CRITERIA getCriteria(){ return this->criteria; }
    double getRegularizationRate(){ return this->regularizationRate; }
    double getLearningRate(){ return this->learningRate; }
    double getMomentumRate(){ return this->momentumRate; }
    size_t getTrainingTimes(){ return this->trainingTimes; }
    size_t getBatchSize() { return this->batchSize; }
    size_t getOutputSize() { return this->outputSize; }
    int parseInformation(fudandnn::Model & model) override;
};
/* 输入层参数 */
class InputLayerHyperParameterControl : public HyperParameterControl{
private:
	std::string file;
public:
    InputLayerHyperParameterControl(): HyperParameterControl(){

	}
	virtual void setFile(std::string file){ this->file = file; }
	std::string getFile(){ return file; }
    virtual int parseInformation(fudandnn::Model & model) override;
};
/* 线性层参数 */
class LinearLayerHyperParameterControl : public HyperParameterControl, public HasSolver{
private:
	size_t visualUnitCount, hiddenUnitCount, initScheme;
    shared_ptr<Solver> solver;
public:
	LinearLayerHyperParameterControl(): LinearLayerHyperParameterControl(0,0,0){

	}
	LinearLayerHyperParameterControl(size_t visualUnitCount, size_t hiddenUnitCount, size_t initScheme) :
        visualUnitCount(visualUnitCount), hiddenUnitCount(hiddenUnitCount), initScheme(initScheme), HyperParameterControl()
	{

	}
	void setVisualUnitCount(size_t visualUnitCount){
		this->visualUnitCount = visualUnitCount;
	}
	void setHiddenUnitCount(size_t hiddenUnitCount){
		this->hiddenUnitCount = hiddenUnitCount;
	}
	void setInitScheme(size_t initScheme){
		this->initScheme = initScheme;
	}
	size_t getVisualUnitCount(){ return this->visualUnitCount; }
	size_t getHiddenUnitCount(){ return this->hiddenUnitCount; }
	size_t getInitScheme(){ return this->initScheme; }
    virtual int parseInformation(fudandnn::Model & model) override;
    void setSolver(shared_ptr<Solver> s) override{
        this->solver = s;
    }
};

class NonlinearHyperParameterControl :public HyperParameterControl{
private:
    size_t type, visualUnitCount;
public:
    NonlinearHyperParameterControl() : NonlinearHyperParameterControl(0, 0){}
    NonlinearHyperParameterControl(size_t type, size_t visualUnitCount) : type(type), visualUnitCount(visualUnitCount),HyperParameterControl(){}
    void setType(size_t type){this->type = type;}
    void setVisualUnitCount(size_t visualUnitCount){ this->visualUnitCount = visualUnitCount; }
    size_t getType(){ return type; }
    size_t getVisualUnitCount(){ return visualUnitCount; }
    int parseInformation(Model & model) override; 
};

class ConvolutionLayerHyperParameterControl : public HyperParameterControl, public HasSolver{
private:
    size_t rowSize, columnSize, kRowSize, kColumnSize, channelSize, featureMapSize, stride, initScheme;
    shared_ptr<Solver> solver;
public:
    ConvolutionLayerHyperParameterControl() : ConvolutionLayerHyperParameterControl(0, 0, 0, 0, 0, 0, 0, fudandnn::initializationScheme::RANDOM){}
    ConvolutionLayerHyperParameterControl(
        size_t rowSize, size_t columnSize,
        size_t kRowSize, size_t kColumnSize,
        size_t channelSize, size_t featureMapSize,
        size_t stride, size_t initScheme) 
        :rowSize(rowSize), columnSize(columnSize), kRowSize(kRowSize), kColumnSize(kColumnSize),
        channelSize(channelSize), featureMapSize(featureMapSize), stride(stride), initScheme(initScheme)
    {}
    size_t getRowSize(){ return rowSize; }
    size_t getColumnSize(){ return columnSize; }
    size_t getKRowSize(){ return kRowSize; }
    size_t getKColumnSize(){ return kColumnSize; }
    size_t getChannelSize(){ return channelSize; }
    size_t getFeatureMapSize(){ return featureMapSize; }
    size_t getStride(){ return stride; }
    size_t getInitScheme(){ return initScheme; }
    void setRowSize(size_t v){ rowSize = v; }
    void setColumnSize(size_t v){ columnSize = v; }
    void setKRowSize(size_t v){ kRowSize = v; }
    void setKColumnSize(size_t v){ kColumnSize = v; }
    void setChannelSize(size_t v){ channelSize = v; }
    void setFeatureMapSize(size_t v){ featureMapSize = v; }
    void setStride(size_t v){ stride = v; }
    void setInitScheme(size_t v){ initScheme = v; }
    void setSolver(shared_ptr<Solver> s) override{
        this->solver = s;
    }
    int parseInformation(Model & model) override;
};

class MaxPoolingHyperParameterControl : public HyperParameterControl{
private:
    size_t rowSize, columnSize, kRowSize, kColumnSize, channelSize, featureMapSize,stride;
public:
    MaxPoolingHyperParameterControl() : MaxPoolingHyperParameterControl(0, 0, 0, 0, 0, 0, 0){}
    MaxPoolingHyperParameterControl(
        size_t rowSize, size_t columnSize,
        size_t kRowSize, size_t kColumnSize,
        size_t channelSize, size_t featureMapSize, size_t stride)
        :rowSize(rowSize), columnSize(columnSize), kRowSize(kRowSize), kColumnSize(kColumnSize),
        channelSize(channelSize), featureMapSize(featureMapSize), stride(stride)
    {}
    size_t getRowSize(){ return rowSize; }
    size_t getColumnSize(){ return columnSize; }
    size_t getKRowSize(){ return kRowSize; }
    size_t getKColumnSize(){ return kColumnSize; }
    size_t getChannelSize(){ return channelSize; }
    size_t getFeatureMapSize(){ return featureMapSize; }
    size_t getStride(){ return stride; }
    void setRowSize(size_t v){ rowSize = v; }
    void setColumnSize(size_t v){ columnSize = v; }
    void setKRowSize(size_t v){ kRowSize = v; }
    void setKColumnSize(size_t v){ kColumnSize = v; }
    void setChannelSize(size_t v){ channelSize = v; }
    void setFeatureMapSize(size_t v){ featureMapSize = v; }
    void setStride(size_t v){ stride = v; }
    int parseInformation(Model & model) override;
};

class ConcatLayerHyperParameterControl : public HyperParameterControl{
private:
    size_t rowSize, columnSize, channelSize;
public:
    ConcatLayerHyperParameterControl() : ConcatLayerHyperParameterControl(0, 0, 0){}
    ConcatLayerHyperParameterControl(
        size_t rowSize, size_t columnSize,
        size_t channelSize)
        :rowSize(rowSize), columnSize(columnSize), channelSize(channelSize)
    {}
    size_t getRowSize(){ return rowSize; }
    size_t getColumnSize(){ return columnSize; }
    size_t getChannelSize(){ return channelSize; }
    void setRowSize(size_t v){ rowSize = v; }
    void setColumnSize(size_t v){ columnSize = v; }
    void setChannelSize(size_t v){ channelSize = v; }
    int parseInformation(Model & model) override;
};
#endif