#include "hyperparametercontrol.h"
#include "include/Model.h"
int NetworkHyperParameterControl::parseInformation(fudandnn::Model & model){
    model.setBatchSize(batchSize);
    model.setCriteria(criteria, outputSize);
    model.setTrainingTimes(trainingTimes);
    return 0;
}
int InputLayerHyperParameterControl::parseInformation(fudandnn::Model & model){
    return model.addInput(fudandnn::INPUT::XMLINPUT, this->file);
}
int LinearLayerHyperParameterControl::parseInformation(fudandnn::Model & model){
    return model.addLinearLayerToNetwork(visualUnitCount, hiddenUnitCount, 1, initScheme, solver, 0.0);
}
int NonlinearHyperParameterControl::parseInformation(fudandnn::Model & model){
    return model.addNonlinearLayerToNetwork(this->visualUnitCount, type, 1, -3, 3, 0.001, 0.0001);
}
int ConvolutionLayerHyperParameterControl::parseInformation(fudandnn::Model & model){
    return model.addConvolutionLayerToNetwork(rowSize, columnSize, kRowSize, kColumnSize,
        channelSize, featureMapSize, stride, initScheme, solver, 0.0);
}
int MaxPoolingHyperParameterControl::parseInformation(fudandnn::Model & model){
    return model.addMaxPoolingLayerToNetwork(rowSize, columnSize, kRowSize, kColumnSize, channelSize, featureMapSize, stride, 0);
}
int ConcatLayerHyperParameterControl::parseInformation(fudandnn::Model &model){
    return model.addConcatLayerToNetwork(rowSize, columnSize, channelSize);
}
