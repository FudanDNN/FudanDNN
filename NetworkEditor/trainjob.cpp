#include "trainjob.h"
#include "hyperparametercontrol.h"
#include <map>
using namespace NetworkUnitPool;
void TrainJob::run()  {
    shared_ptr<MasterControl> oldMaster = master;
    master = shared_ptr<MasterControl>(new MasterControl(this));

    std::vector<size_t> unitIds;
    std::vector<size_t> unitCNNIds;
    std::map<int,size_t> cnnIdsMap;//map cnn id form gui to master

    unitIds.resize(networkUnits.size());
    unitCNNIds.resize(networkUnits.size());
    InputLayerHyperParameterControl *ilhpc;
    LinearLayerHyperParameterControl *llhpc;
    NonLinearLayerHyperParameterControl *nllhpc;
    CNNConvolutionLayerHyperParameterControl *cnnclhpc;
    CNNMaxPoolingLayerHyperParameterControl *cnnmphpc;
    CNNNonLinearLayerHyperParameterControl *cnnnlhpc;

    CNNUnitHyperParameterControl* cnnuhpc;
    //find CNN
    for (int i = 0; i < cnnSubnets.size(); ++i){
        cnnIdsMap[cnnSubnets[i]->getHyperParameterControl()->getCnnId()]=master->addCNN();
    }
    //add nodes
    for (int i = 0; i < networkUnits.size(); ++i){
        if (networkUnits[i]->getHyperParameterControl()->getAssociatedNode() != nullptr){
            shared_ptr<NetworkNode> node = networkUnits[i]->getHyperParameterControl()->getAssociatedNode();
            node->clearEdges();
            unitIds[i] = master->addNode(node);
            continue;
        }
        if (networkUnits[i]->isCNNUnit()){
            if (dynamic_cast<CNNUnitHyperParameterControl*>(networkUnits[i]->getHyperParameterControl())->getAssociatedCNNNode() != nullptr){
                cnnuhpc = dynamic_cast<CNNUnitHyperParameterControl*>(networkUnits[i]->getHyperParameterControl());
                shared_ptr<ComponentNode> node = cnnuhpc->getAssociatedCNNNode();
                node->clearEdges();
                unitCNNIds[i] = cnnIdsMap[cnnuhpc->getCnnId()];
                unitIds[i]=master->addComponentInCNN(unitCNNIds[i], node);
                continue;
            }
        }
        switch (networkUnits[i]->getTypeId()){
            case NetworkUnitBase::INPUT_UNIT:
                ilhpc = dynamic_cast<InputLayerHyperParameterControl*>(networkUnits[i]->getHyperParameterControl());
                unitIds[i] = master->addInput(ilhpc->getFile());
                break;
            case NetworkUnitBase::LINEAR_UNIT:
                llhpc = dynamic_cast<LinearLayerHyperParameterControl*> (networkUnits[i]->getHyperParameterControl());
                unitIds[i] = master->addLINEAR(llhpc->getVisualUnitCount(), llhpc->getHiddenUnitCount(), llhpc->getInitScheme(),
                    llhpc->getRegularizationRate(),llhpc->getWeightLearningRate(),llhpc->getBiasLearningRate(),llhpc->getMomentumRate());
                break;
            case NetworkUnitBase::NONLINEAR_UNIT:
                nllhpc = dynamic_cast<NonLinearLayerHyperParameterControl*> (networkUnits[i]->getHyperParameterControl());
                unitIds[i] = master->addNONLINEAR(nllhpc->getSize(), nllhpc->getType());
                break;
            case NetworkUnitBase::CNN_INPUT_UNIT:
                cnnuhpc = dynamic_cast<CNNUnitHyperParameterControl*>(networkUnits[i]->getHyperParameterControl());
                unitIds[i] = cnnIdsMap[cnnuhpc->getCnnId()];
                break;
            case NetworkUnitBase::CNN_OUTPUT_UNIT:
                unitIds[i] = cnnIdsMap[dynamic_cast<CNNUnitHyperParameterControl*>(networkUnits[i]->getHyperParameterControl())->getCnnId()];
                break;
            case NetworkUnitBase::CNN_CONVOLUTIONAL_UNIT:
                cnnclhpc = dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(networkUnits[i]->getHyperParameterControl());
                unitCNNIds[i] = cnnIdsMap[cnnclhpc->getCnnId()];
                unitIds[i] = master->addCNN2DComponentToCNN(
                    cnnclhpc->getKernelSize(), cnnclhpc->getStride(), 
                    cnnclhpc->getFeatureMapNum(), cnnclhpc->getNum(), 
                    cnnclhpc->getVisualRow(), cnnclhpc->getVisualColumn(), 
                    cnnclhpc->getInitScheme(),cnnclhpc->getRegularizationRate(),cnnclhpc->getKernelLearningRate(),cnnclhpc->getBiasLearningRate(),cnnclhpc->getMomentumRate(), unitCNNIds[i]);
                break;
            case NetworkUnitBase::CNN_MAXPOOLING_UNIT:
                cnnmphpc = dynamic_cast<CNNMaxPoolingLayerHyperParameterControl*>(networkUnits[i]->getHyperParameterControl());
                unitCNNIds[i] = cnnIdsMap[cnnmphpc->getCnnId()];
                unitIds[i] = master->addMaxPoolingComponentToCNN(
                    cnnmphpc->getPoolingSize(), cnnmphpc->getStride(),
                     cnnmphpc->getVisualRow(), cnnmphpc->getVisualColumn(), 
                     cnnmphpc->getNum(), unitCNNIds[i]);
                break;
            case NetworkUnitBase::CNN_NONLINEAR_UNIT:
                cnnnlhpc = dynamic_cast<CNNNonLinearLayerHyperParameterControl*>(networkUnits[i]->getHyperParameterControl());
                unitCNNIds[i] = cnnIdsMap[cnnnlhpc->getCnnId()];
                unitIds[i] = master->addNonLinearToCNN(cnnclhpc->getVisualRow(),
                    cnnnlhpc->getVisualColumn(),cnnnlhpc->getNum(),
                    cnnnlhpc->getType(), unitCNNIds[i]);
                break;
        }
    }
    //get nodes
    std::vector<std::shared_ptr<NetworkNode>> nodesInMaster = master->getAllNodes();
    std::map<int, std::shared_ptr<NetworkNode>> nodesInMasterIdMap;
    std::map<int, std::shared_ptr<ConvolutionalNetworkLayer>> CNNSubnetIdMap;
    for (int i = 0; i < nodesInMaster.size(); ++i){
        if (nodesInMaster[i]->getLayer()->getNetworkName() == "ConvolutionalNetworkLayer"){
            CNNSubnetIdMap[nodesInMaster[i]->getId()] = 
                dynamic_pointer_cast<ConvolutionalNetworkLayer>(nodesInMaster[i]->getLayer());
        }
        nodesInMasterIdMap[nodesInMaster[i]->getId()] = nodesInMaster[i];
        
    }
    for (int i = 0; i < networkUnits.size(); ++i){
        if (networkUnits[i]->getTypeId()!=NetworkUnitBase::INPUT_UNIT){
            if (networkUnits[i]->isCNNUnit()){
                //TODO: associate cnn nodes
                if (networkUnits[i]->getTypeId() == NetworkUnitBase::CNN_INPUT_UNIT ||
                    networkUnits[i]->getTypeId() == NetworkUnitBase::CNN_OUTPUT_UNIT)
                    continue;
                vector<shared_ptr<ComponentNode>> cnnComponentNodes =
                    CNNSubnetIdMap[
                        cnnIdsMap[unitCNNIds[
                            dynamic_cast<CNNUnitHyperParameterControl*>(networkUnits[i]->getHyperParameterControl())
                                ->getCnnId()]
                    ]]->getAllComponents();
                for (int j = 0; j < cnnComponentNodes.size(); ++j){
                    if (cnnComponentNodes[j]->getId() == unitIds[i]){
                        dynamic_cast<CNNUnitHyperParameterControl*>(networkUnits[i]->getHyperParameterControl())
                            ->setAssociatedCNNNode(cnnComponentNodes[j]);
                        break;
                    }
                }
                continue;
            }
            networkUnits[i]->getDiagramItem()->getNetworkUnitPtr()
                ->getHyperParameterControl()->setAssociatedNode(nodesInMasterIdMap[unitIds[i]]);
        }
    }
    //add edges
    for (int i = 0; i < networkUnits.size(); ++i){
        //ignore output of cnn input unit
        if (networkUnits[i]->getTypeId() == NetworkUnitBase::CNN_INPUT_UNIT) continue;
        
        std::vector<NetworkUnitBase*> outputNodes = networkUnits[i]->getOutputUnitList();
        for (auto iter = outputNodes.cbegin(); iter != outputNodes.cend(); ++iter){
            //ignore input of cnn output unit
            if ((*iter)->getTypeId() == NetworkUnitBase::CNN_OUTPUT_UNIT) continue;
            
            for (int j = 0; j < networkUnits.size(); ++j){
                if (*iter == networkUnits[j]){
                    if (networkUnits[i]->getTypeId() == NetworkUnitBase::INPUT_UNIT){
                        master->addInputEdge(unitIds[i], unitIds[j]);
                    }
                    else if (networkUnits[i]->getTypeId() == NetworkUnitBase::CNN_OUTPUT_UNIT){
                        master->addEdge(unitIds[i], unitIds[j]);
                    }
                    else if (networkUnits[i]->isCNNUnit(networkUnits[i]->getTypeId())){
                        master->addEdgeInCNN(unitIds[i], unitIds[j], unitCNNIds[j]);
                    }
                    else{
                        master->addEdge(unitIds[i],unitIds[j]);
                    }
                }
            }
        }
    }
    master->setBatchSize(this->batchSize);
    master->setCriteria(this->criteria);
    master->setTrainingTimes(this->trainingTimes);
    int networkError= master->checkNetwork();
    if (networkError == NetworkError::CORRECT){
        switch (this->jobType){
        case WorkingThreadJobType::PREDICT_JOB:
            this->jobType = WorkingThreadJobType::TRAIN_JOB;
            if (networkError == NetworkError::CORRECT){
                master->predict(fileName);
            }
            break;
        case WorkingThreadJobType::TRAIN_JOB:
            master->run();
            break;
        }
    }
    else{
        switch (networkError){
            case NetworkError::NOINPUT:
                emitMessage("*ERROR* : No Input Layer!");
                break;
            case NetworkError::WRONGHIDDENNUM:
                emitMessage("*ERROR* : Wrong Hidden Layer Number!");
                break;
            case NetworkError::WRONGVISUALNUM:
                emitMessage("*ERROR* : Wrong Visual Layer Number!");
                break;
        }
    }
}
