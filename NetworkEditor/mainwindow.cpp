#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "diagramitem.h"
#include "diagramscene.h"
#include "networkunitbase.h"
#include"networkunitpool.h"
#include "arrow.h"
#include <QtGui>
#include <QFileDialog>
#include <qmessagebox.h>
#include "hyperparameterwidget.h"
#include <fstream>
#include "cnnsubnetcontainer.h"
#include "workingthread.h"
#include "kernel/MasterControl.h"
#include "trainjob.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	currentHyperParameterWidget(nullptr)
{
    createWorkingThread();
	ui->setupUi(this);
    this->ui->networkOutputText->setFont(QFont("Times New Roman"));
	createUnitBox();
	createSettingBox();
	createItemMenu();
	createScene();
	connectActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::createWorkingThread(){
    workingThread = new TrainJob();
    connect(workingThread, SIGNAL(alert(QString )), this, SLOT(doAlert(QString)));
    connect(workingThread, SIGNAL(finished()), this, SLOT(joinWorkingThread()));
}
QToolButton * MainWindow::createUnitBtn(QString const & title){
	QToolButton *retBtn = new QToolButton;
	retBtn->setText(title);
	retBtn->setCheckable(true);
	retBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
	return retBtn;
}
void MainWindow::createUnitBox(){
	//clean old pages
	this->ui->unitToolBox->removeItem(0);
	//create basic unit box
	QWidget *basicUnitBox = new QWidget(this);
	QVBoxLayout *basicUnitBoxLayout = new QVBoxLayout(basicUnitBox);
	QButtonGroup *basicUnitBtnGroup = new QButtonGroup;
	basicUnitBox->setLayout(basicUnitBoxLayout);
	
	lineUnitBtn = createUnitBtn(QString("Line"));
	moveCursorBtn = createUnitBtn(QString("Move"));
	inputUnitBtn = createUnitBtn(QString("Input Layer"));
	linearLayerUnitBtn = createUnitBtn(QString("Linear Layer"));
	nonlinearLayerUnitBtn = createUnitBtn(QString("Nonlinear Layer"));
	cnnSubnetBtn = createUnitBtn(QString("CNN Subnet"));
	cnnConvolutionalUnitBtn = createUnitBtn(QString("CNN Convolutuonal Layer"));
	cnnMaxPoolingUnitBtn = createUnitBtn(QString("CNN MaxPooling Layer"));
    cnnNonlinearLayerUnitBtn = createUnitBtn(QString("CNN Nonlinear Layer"));
    QToolButton *btn1 = createUnitBtn(QString("Auto Encoder"));
    QToolButton *btn2 = createUnitBtn(QString("RBM"));
	//add to btngroup
	basicUnitBtnGroup->addButton(moveCursorBtn, 1);
	basicUnitBtnGroup->addButton(lineUnitBtn, 2);
	basicUnitBtnGroup->addButton(inputUnitBtn, 3);
	basicUnitBtnGroup->addButton(linearLayerUnitBtn, 4);
	basicUnitBtnGroup->addButton(nonlinearLayerUnitBtn, 5);
	basicUnitBtnGroup->addButton(cnnSubnetBtn, 6);
	basicUnitBtnGroup->addButton(cnnConvolutionalUnitBtn, 7);
	basicUnitBtnGroup->addButton(cnnMaxPoolingUnitBtn, 8);
    basicUnitBtnGroup->addButton(cnnNonlinearLayerUnitBtn, 9);
    basicUnitBtnGroup->addButton(btn1, 10);
    basicUnitBtnGroup->addButton(btn2, 11);
	//bind clicked signal
	connect(basicUnitBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(basicUnitBtnGroupClicked(int)));
	//add to layout
	foreach(QAbstractButton *button, basicUnitBtnGroup->buttons()){
		basicUnitBoxLayout->addWidget(button);
	}
	basicUnitBoxLayout->addStretch();

	moveCursorBtn->setChecked(true);
	this->ui->unitToolBox->addItem(basicUnitBox, tr("Basic units"));
	
}
void MainWindow::connectActions(){
	connect(this->ui->actionOutputGraph, SIGNAL(triggered()), this, SLOT(outputNetworkAction()));
	connect(this->ui->actionResetItemsPosition, SIGNAL(triggered()), this, SLOT(resetItemsPosition()));
    connect(this->ui->actionSaveNetwork, SIGNAL(triggered()), this, SLOT(saveNetwork()));
	connect(this->ui->actionOpenNetwork, SIGNAL(triggered()), this, SLOT(loadNetwork()));
    connect(this->ui->actionResetNetwork, SIGNAL(triggered()), this, SLOT(clearNetwork()));
}

void MainWindow::basicUnitBtnGroupClicked(int btnId){
	switch (btnId){
		case 1:
			this->scene->setMode(DiagramScene::MoveItem);
			break;
		case 2:
			this->scene->setMode(DiagramScene::InsertLine);
			break;
		case 3: //input layer
			this->scene->setMode(DiagramScene::InsertItem);
			this->scene->setUnitTypeToInsert (NetworkUnitBase::NetworkUnitType::INPUT_UNIT);
			break;
		case 4: //linear layer
			this->scene->setMode(DiagramScene::InsertItem);
			this->scene->setUnitTypeToInsert(NetworkUnitBase::NetworkUnitType::LINEAR_UNIT);
			break; 
		case 5: //nonlinear layer
			this->scene->setMode(DiagramScene::InsertItem);
			this->scene->setUnitTypeToInsert(NetworkUnitBase::NetworkUnitType::NONLINEAR_UNIT);
			break;
		case 6: //cnn subnet
			this->scene->setMode(DiagramScene::InsertCNNSubnet);
			break;
		case 7: //cnn convolutional layer
			this->scene->setMode(DiagramScene::InsertItem);
			this->scene->setUnitTypeToInsert(NetworkUnitBase::NetworkUnitType::CNN_CONVOLUTIONAL_UNIT);
			break;
		case 8: //cnn pooling layer
			this->scene->setMode(DiagramScene::InsertItem);
			this->scene->setUnitTypeToInsert(NetworkUnitBase::NetworkUnitType::CNN_MAXPOOLING_UNIT);
			break;
        case 9: //cnn nonlinear layer
            this->scene->setMode(DiagramScene::InsertItem);
            this->scene->setUnitTypeToInsert(NetworkUnitBase::NetworkUnitType::CNN_NONLINEAR_UNIT);
            break;
	};
}

void MainWindow::createItemMenu(){
	this->itemMenu = menuBar()->addMenu(tr("Items"));
	QAction *deleteItemAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"),this);
	connect(deleteItemAction, SIGNAL(triggered()), this, SLOT(deleteItem()));
	this->itemMenu->addAction(deleteItemAction);
}

void MainWindow::deleteItem(){
	QList<QGraphicsItem *> selectedItems= this->scene->selectedItems();
	DiagramItem *ditem;
	//check cnn input & output
	foreach (QGraphicsItem* boxItem, selectedItems){
		ditem = dynamic_cast<DiagramItem*>(boxItem);
		if (ditem == NULL) continue;
		if (ditem->getNetworkUnitPtr()->getTypeId() == NetworkUnitBase::CNN_INPUT_UNIT ||
			ditem->getNetworkUnitPtr()->getTypeId() == NetworkUnitBase::CNN_OUTPUT_UNIT){
			int result =QMessageBox::warning(this, "Delete confirm",
				"Do you really want to delete CNN Input or CNN Output node?\nThe operation will delete the whole CNN Subnet.",
				QMessageBox::StandardButton::Cancel,
				QMessageBox::StandardButton::Yes);
			if (result == QMessageBox::Cancel) return;
		}
	}
	std::vector<int> subnetToRemove;
	//actually removing
	foreach(QGraphicsItem* boxItem, selectedItems){
		ditem = dynamic_cast<DiagramItem*>(boxItem);
		if (ditem != NULL){
			if (ditem->getNetworkUnitPtr()->getTypeId() == NetworkUnitBase::CNN_INPUT_UNIT ||
				ditem->getNetworkUnitPtr()->getTypeId() == NetworkUnitBase::CNN_OUTPUT_UNIT){
				subnetToRemove.push_back((dynamic_cast<CNNUnitBase *>(ditem->getNetworkUnitPtr()))->getSubnetId());
				continue;
			}
			scene->removeNetworkUnit(ditem);
            continue;
		}
		//check arrows
		Arrow* arrow = dynamic_cast<Arrow*>(boxItem);
		if (arrow != NULL) {
			DiagramItem *startItem = arrow->startItem();
			DiagramItem *endItem = arrow->endItem();
			//remove arrow from DiagramItem objects
			startItem->removeArrow(arrow);
			endItem->removeArrow(arrow);

			//remove link between two network units
			startItem->getNetworkUnitPtr()->removeOutputUnit(endItem->getNetworkUnitPtr());
			endItem->getNetworkUnitPtr()->removeInputUnit(startItem->getNetworkUnitPtr());

			//remove arrow from scene
			scene->removeItem(arrow);
			delete arrow;
            continue;
		}
	}
	for (auto iterId = subnetToRemove.cbegin(); iterId != subnetToRemove.cend(); ++iterId){
		int id = *iterId;
		scene->removeSubnet(id);
	}
}

void MainWindow::useMoveCursor(){
	this->moveCursorBtn->setChecked(true);
	this->scene->setMode(DiagramScene::MoveItem);
}

void MainWindow::createScene(){
	this->scene = new DiagramScene(this->itemMenu, this);
	this->scene->setSceneRect(QRectF(0, 0, 5000, 5000));
	connect(this->scene, SIGNAL(itemInserted(QGraphicsItem *)), this, SLOT(useMoveCursor()));
	ui->graphicsView->setScene(this->scene);
	ui->graphicsView->scale(1.0, 1.0);
	ui->graphicsView->centerOn(0, 0);
	connect(scene, SIGNAL(itemSelected(QGraphicsItem *)), this, SLOT(onItemSelected(QGraphicsItem*)));
    connect(scene, SIGNAL(itemUnSelected(QGraphicsItem *)), this, SLOT(onItemUnSelected(QGraphicsItem*)));
}

void MainWindow::outputNetworkAction(){
	std::ofstream fout("D:/outputfile.txt");
	NetworkUnitPool::outputNetworkGraph(fout);
	fout.close();
}

void MainWindow::resetItemsPosition(){
	int boxWidth = DiagramItem::WIDTH + scene->getItemPadding() * 2;
	int boxHeight = DiagramItem::HEIGHT + scene->getItemPadding() * 2;
	int boxCenterX = scene->getItemPadding() + DiagramItem::WIDTH / 2;
	int boxCenterY = scene->getItemPadding() + DiagramItem::HEIGHT / 2;
	// O(N) method
	const int windowLeftPadding = 60;
	std::vector<int> unitLayerMarks = NetworkUnitPool::sliceLayers();
	int layerCount = -1;
	for (auto iter = unitLayerMarks.cbegin(); iter != unitLayerMarks.cend(); ++iter){
		layerCount = (*iter > layerCount) ? *iter : layerCount;
	}
	layerCount++;
	
	std::vector< std::vector<NetworkUnitBase*>> units(layerCount);
	for (int i = 0; i != unitLayerMarks.size(); ++i){
		int layer = unitLayerMarks[i];
		units[layer].push_back(NetworkUnitPool::networkUnits[i]);
	}
	int xPos = boxCenterX + windowLeftPadding, yPos = boxCenterY;
	for (int i = 0; i != units.size(); ++i){
		std::vector<NetworkUnitBase*> &unitsInLayer = units[i];
		for (int j = 0; j != unitsInLayer.size(); ++j){
			DiagramItem *ditem = unitsInLayer[j]->getDiagramItem();
			ditem->setPos(xPos, yPos);
			xPos += boxWidth;
		}
		yPos += boxHeight;
		xPos = boxCenterX + windowLeftPadding;
	}
	this->scene->resizeSubnets();
}
void MainWindow::saveFileAction(){
	QFileDialog fileDialog(this);
	fileDialog.setWindowTitle("Save to file");
	fileDialog.setFileMode(QFileDialog::AnyFile);
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);
	fileDialog.setNameFilter("FudanDNN description files (*.dnn);;All Files (*.*)");
	if (fileDialog.exec() == QFileDialog::Accepted)
	{
		//TODO: output to file
		QString file = fileDialog.selectedFiles()[0];
		std::ofstream fout(file.toStdString().c_str());
		fout << "HAHAHA" << std::endl;
		fout.close();
	}
}

void MainWindow::openFileAction(){
	QFileDialog fileDialog(this);
	fileDialog.setWindowTitle("Open file");
	fileDialog.setFileMode(QFileDialog::AnyFile);
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setNameFilter("FudanDNN description files (*.dnn);;All Files (*.*)");
	if (fileDialog.exec() == QFileDialog::Accepted)
	{
		//TODO: input from file
	}

}

void MainWindow::createSettingBox(){
    connect(this->ui->startTrainingBtn, SIGNAL(clicked()), this, SLOT(startTraining()));
    connect(this->ui->abortTrainingBtn, SIGNAL(clicked()), this, SLOT(abortWorkingThread()));
    connect(this->ui->startPredictingBtn, SIGNAL(clicked()), this, SLOT(startPredict()));
    connect(this->ui->pauseTrainingBtn, SIGNAL(clicked()), this, SLOT(togglePauseNetwork()));
}

void MainWindow::onItemSelected(QGraphicsItem *graphicsItem){
	DiagramItem *diagramItem = dynamic_cast<DiagramItem*> (graphicsItem);
	if (diagramItem != nullptr){
		HyperParameterWidget *newWidget= createHyperParameterWidget(diagramItem->getNetworkUnitPtr()->getHyperParameterControl());
		ui->settingBox->insertItem(0, newWidget, "Layer Setting");
		if (currentHyperParameterWidget != nullptr) delete currentHyperParameterWidget;
		currentHyperParameterWidget = newWidget;
        ui->settingBox->setCurrentIndex(0);
	}
	CNNSubnetContainer* cnnSubnetContainer = dynamic_cast<CNNSubnetContainer*> (graphicsItem);
	if (cnnSubnetContainer != nullptr){
		HyperParameterWidget *newWidget = createHyperParameterWidget(cnnSubnetContainer->getCNNSubnet()->getHyperParameterControl());
		ui->settingBox->insertItem(0, newWidget, "Layer Setting");
		if (currentHyperParameterWidget != nullptr) delete currentHyperParameterWidget;
		currentHyperParameterWidget = newWidget;
        ui->settingBox->setCurrentIndex(0);
	}
}
void MainWindow::onItemUnSelected(QGraphicsItem *graphicsItem){
    if (ui->settingBox->itemText(0) == "Layer Setting"){ 
        ui->settingBox->removeItem(0); 
        delete currentHyperParameterWidget;
        currentHyperParameterWidget = nullptr;
    }
}

HyperParameterWidget* MainWindow::createHyperParameterWidget(HyperParameterControl *control){
	if (control == nullptr) return nullptr;
	if (dynamic_cast<InputLayerHyperParameterControl*>(control) != nullptr){
		return new InputLayerHyperParameterWidget(dynamic_cast<InputLayerHyperParameterControl*>(control), 0);
	}
	if (dynamic_cast<LinearLayerHyperParameterControl*>(control) != nullptr){
		return new LinearLayerHyperParameterWidget(dynamic_cast<LinearLayerHyperParameterControl*>(control), 0);
	}
    if (dynamic_cast<NonLinearLayerHyperParameterControl*>(control) != nullptr){
        return new NonLinearLayerHyperParameterWidget(dynamic_cast<NonLinearLayerHyperParameterControl*>(control), 0);
    }
    if (dynamic_cast<CNNSubnetHyperParameterControl*>(control) != nullptr){
        return new CNNSubnetHyperParameterWidget(dynamic_cast<CNNSubnetHyperParameterControl*>(control), 0);
    }
    if (dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(control) != nullptr){
        return new CNNConvolutionLayerHyperParameterWidget(dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(control), 0);
    }
    if (dynamic_cast<CNNMaxPoolingLayerHyperParameterControl *>(control) != nullptr){
        return new CNNMaxPoolingLayerHyperParameterWidget(dynamic_cast<CNNMaxPoolingLayerHyperParameterControl*>(control), this);
    }
    if (dynamic_cast<CNNNonLinearLayerHyperParameterControl *>(control) != nullptr){
        return new CNNNonLinearLayerHyperParameterWidget(dynamic_cast<CNNNonLinearLayerHyperParameterControl*>(control), this);
    }
    if (dynamic_cast<CNNIOHyperParameterControl*> (control) != nullptr){
        return new CNNIOHyperParameterWidget(dynamic_cast<CNNIOHyperParameterControl*>(control),this);
    }
	return nullptr;
}

/* slots for working thread*/
void MainWindow::doAlert(QString msg){
    
    this->ui->networkOutputText->setText(this->ui->networkOutputText->toPlainText() +msg);
    ui->networkOutputText->moveCursor(QTextCursor::End);
}
void MainWindow::joinWorkingThread(){
    workingThread->wait();
    ui->startTrainingBtn->setEnabled(true);
    ui->startPredictingBtn->setEnabled(true);
}
void MainWindow::startPredict(){
    TrainJob* trainJob = dynamic_cast<TrainJob*>(workingThread);
    trainJob->setBatchSize((size_t)(ui->networkBatchSizeSpinBox->value()));
    trainJob->setTrainingTimes((size_t)(ui->networkTrainingTimesSpinBox->value()));
    trainJob->setCriteria((size_t)(ui->networkCriteriaComboBox->currentIndex()));
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle("Save to file");
    fileDialog.setFileMode(QFileDialog::FileMode::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("All Files (*.*)");
    if (fileDialog.exec() == QFileDialog::Accepted){
        trainJob->setJobType(WorkingThreadJobType::PREDICT_JOB);
        trainJob->setPredictFileName(fileDialog.selectedFiles()[0].toStdString());
        this->ui->startPredictingBtn->setEnabled(false);
        this->ui->startTrainingBtn->setEnabled(false);
        trainJob->start();
    }
}
void MainWindow::abortWorkingThread(){
    if (workingThread != nullptr){
        if (workingThread->isRunning()){
            int result = QMessageBox::warning(this, "Abort Working", "Abort the running job is a dangerous operation!\nAre you sure?", QMessageBox::Cancel, QMessageBox::Yes);
            if (result == QMessageBox::Yes){
                workingThread->abort();
            }
        }
    }
}
void MainWindow::startTraining(){
    if (workingThread != nullptr){
        if (workingThread->isRunning()){
            int result = QMessageBox::critical(this, "In working!",
                "Fudan DNN is working!",
                QMessageBox::StandardButton::Cancel);
            return;
        }
    }
    //prepare ui
    ui->networkOutputText->setPlainText("");
    for (size_t i = 0; i < ui->settingBox->count(); ++i){
        if (ui->settingBox->itemText(i) == "Network Output"){
            ui->settingBox->setCurrentIndex(i);
            break;
        }
    }
    
    //build thread object
    TrainJob *trainJob = dynamic_cast<TrainJob*> (workingThread);
    workingThread = trainJob;
    trainJob->setBatchSize((size_t)(ui->networkBatchSizeSpinBox->value()));
    trainJob->setTrainingTimes((size_t)(ui->networkTrainingTimesSpinBox->value()));
    trainJob->setCriteria((size_t)(ui->networkCriteriaComboBox->currentIndex()));
    //forbidden button
    ui->startTrainingBtn->setEnabled(false);
    //start job
    trainJob->setJobType(WorkingThreadJobType::TRAIN_JOB);
    workingThread->start();
}
void MainWindow::saveNetwork(){
    if (workingThread->isRunning()){
        int result = QMessageBox::warning(this, "FudanDNN", "Please wait the working stop!", QMessageBox::Cancel);
        return;
    }
    TrainJob* trainJob = dynamic_cast<TrainJob* >(workingThread);

    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle("Save to file");
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setNameFilter("All Files (*.*)");
    if (fileDialog.exec() == QFileDialog::Accepted)
    {
        //TODO: output to file
        QString file = fileDialog.directory().path();
        trainJob->getMasterControl()->writeSelf(file.toStdString());
    }
}
void MainWindow::loadNetwork(){
    if(workingThread->isRunning()){
        int result = QMessageBox::warning(this, "FudanDNN", "Please wait the working stop!", QMessageBox::Cancel);
        return;
    }
    TrainJob* trainJob = dynamic_cast<TrainJob* >(workingThread);

    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle("Open file");
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setNameFilter("All Files (*.*)");
    if (fileDialog.exec() == QFileDialog::Accepted)
    {
        QString file = fileDialog.directory().path();
        trainJob->getMasterControl()->readSelf(file.toStdString());
        rebuildNetwork(trainJob->getMasterControl());
    }
    this->ui->startPredictingBtn->setEnabled(true);
}
void MainWindow::rebuildNetwork(shared_ptr<MasterControl> master){
    vector<shared_ptr<NetworkNode>> nodes = master->getAllNodes();
    DiagramItem *item;
    map<size_t, DiagramItem*> idItemMap;
    map<DiagramItem*, DiagramItem*> cnnOutput2InputMap;
    map<size_t, CNNSubnetContainer*> idCNNMap;
    for (int i = 0; i < nodes.size(); ++i){
        if (nodes[i]->getLayer()->getNetworkName() == "ConvolutionalNetworkLayer"){
            CNNSubnetContainer * subnet = scene->addCNN(QPointF(50.0,50.0));
            idCNNMap[nodes[i]->getId()] = subnet;
            vector <CNNUnitBase *> cnnUnits = subnet->getCNNSubnet()->getUnitsList();
            CNNUnitBase* inputNode, *outputNode;
            for (int j = 0; j < cnnUnits.size(); ++j){
                if (cnnUnits[j]->getTypeId() == CNNUnitBase::CNN_INPUT_UNIT) inputNode = cnnUnits[j];
                if (cnnUnits[j]->getTypeId() == CNNUnitBase::CNN_OUTPUT_UNIT) outputNode = cnnUnits[j];
            }
            cnnOutput2InputMap[outputNode->getDiagramItem()] = inputNode->getDiagramItem();
            idItemMap[nodes[i]->getId()] = outputNode->getDiagramItem();
        }
    }
    for (int i = 0; i < nodes.size(); ++i){
        string networkTypeName = nodes[i]->getLayer()->getNetworkName();
        shared_ptr<AbstractLayer> layer = nodes[i]->getLayer();
        if (networkTypeName == "LinearLayer"){
            item=scene->addNetworkUnit(NetworkUnitBase::LINEAR_UNIT);
            idItemMap[nodes[i]->getId()] = item;
            LinearLayerHyperParameterControl *hpc = dynamic_cast<LinearLayerHyperParameterControl*>(item->getNetworkUnitPtr()->getHyperParameterControl());
            hpc->setVisualUnitCount(dynamic_pointer_cast<LinearLayer>(layer)->getVisualUnit());
            hpc->setHiddenUnitCount(dynamic_pointer_cast<LinearLayer>(layer)->getHiddenUnit());
            hpc->setAssociatedNode(nodes[i]);
        }
        if (networkTypeName == "Sigmoid"){
            item=scene->addNetworkUnit(NetworkUnitBase::NONLINEAR_UNIT);
            idItemMap[nodes[i]->getId()] = item;
            NonLinearLayerHyperParameterControl *hpc = dynamic_cast<NonLinearLayerHyperParameterControl*>(item->getNetworkUnitPtr()->getHyperParameterControl());
            hpc->setType(NonLinearFactory::SIGMOID);
            hpc->setSize(dynamic_pointer_cast<Sigmoid>(layer)->getSize());
            hpc->setAssociatedNode(nodes[i]);
        }
        if (networkTypeName == "ReLU"){
            item = scene->addNetworkUnit(NetworkUnitBase::NONLINEAR_UNIT);
            idItemMap[nodes[i]->getId()] = item;
            NonLinearLayerHyperParameterControl *hpc = dynamic_cast<NonLinearLayerHyperParameterControl*>(item->getNetworkUnitPtr()->getHyperParameterControl());
            hpc->setType(NonLinearFactory::RELU);
            hpc->setSize(dynamic_pointer_cast<ReLU>(layer)->getSize());
            hpc->setAssociatedNode(nodes[i]);
        }

        /*
        CODE REVIEW ATTENTION
        only j can appear in follow block!
        CARE i !
        */
        if (networkTypeName == "ConvolutionalNetworkLayer"){
            vector<shared_ptr<ComponentNode>> cnnNodes = dynamic_pointer_cast<ConvolutionalNetworkLayer>(nodes[i]->getLayer())->getAllComponents();
            CNNSubnetContainer * targetSubnet = idCNNMap[nodes[i]->getId()];
            map<size_t, DiagramItem*> idCNNItemMap;
            for (int j = 0; j < cnnNodes.size(); ++j){
                string componentName = cnnNodes[j]->getComponent()->getComponentName();
                shared_ptr<AbstractComponent> cnnComponent = cnnNodes[j]->getComponent();
                if (componentName == "CNN2D"){
                    item = scene->addNetworkUnit(NetworkUnitBase::CNN_CONVOLUTIONAL_UNIT);
                    idCNNItemMap[cnnNodes[j]->getId()] = item;
                    CNNConvolutionLayerHyperParameterControl *hpc = dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(item->getNetworkUnitPtr()->getHyperParameterControl());
                    hpc->setFeatureMapNum(dynamic_pointer_cast<CNN2DComponent>(cnnComponent)->getFeatureMapNum());
                    hpc->setKernelSize(dynamic_pointer_cast<CNN2DComponent>(cnnComponent)->getKernelSize());
                    hpc->setNum(dynamic_pointer_cast<CNN2DComponent>(cnnComponent)->getNum());
                    hpc->setStride(dynamic_pointer_cast<CNN2DComponent>(cnnComponent)->getStride());
                    hpc->setVisualColumn(dynamic_pointer_cast<CNN2DComponent>(cnnComponent)->getVisualColumn());
                    hpc->setVisualRow(dynamic_pointer_cast<CNN2DComponent>(cnnComponent)->getVisualRow());
                }
                if (componentName == "MaxPooling"){
                    item = scene->addNetworkUnit(NetworkUnitBase::CNN_MAXPOOLING_UNIT);
                    idCNNItemMap[cnnNodes[j]->getId()] = item;
                    CNNMaxPoolingLayerHyperParameterControl *hpc = dynamic_cast<CNNMaxPoolingLayerHyperParameterControl*>(item->getNetworkUnitPtr()->getHyperParameterControl());
                    hpc->setNum(dynamic_pointer_cast<MaxPoolingLayer>(cnnComponent)->getNum());
                    hpc->setPoolingSize(dynamic_pointer_cast<MaxPoolingLayer>(cnnComponent)->getPoolingSize());
                    hpc->setStride(dynamic_pointer_cast<MaxPoolingLayer>(cnnComponent)->getStride());
                    hpc->setVisualColumn(dynamic_pointer_cast<MaxPoolingLayer>(cnnComponent)->getVisualColumn());
                    hpc->setVisualRow(dynamic_pointer_cast<MaxPoolingLayer>(cnnComponent)->getVisualRow());
                }
                CNNUnitBase *ptr = dynamic_cast<CNNUnitBase*> (item->getNetworkUnitPtr());
                targetSubnet->getCNNSubnet()->registerUnit(ptr);
                dynamic_cast<CNNUnitHyperParameterControl*>(ptr->getHyperParameterControl())->setAssociatedCNNNode(cnnNodes[j]);
                dynamic_cast<CNNUnitHyperParameterControl*>(ptr->getHyperParameterControl())->setCnnId(targetSubnet->getCNNSubnet()->getId());
            }
            for (int j = 0; j < cnnNodes.size(); ++j){
                shared_ptr<AbstractComponent> cnnComponent = cnnNodes[j]->getComponent();
                vector<shared_ptr<ComponentNode>> nextNode = cnnNodes[j]->getNextNode();
                
                int nowId = cnnNodes[j]->getId();
                if (nextNode.size() == 0){
                    scene->addArrow(idCNNItemMap[nowId],idItemMap[nodes[i]->getId()]);
                }
                if (cnnNodes[j]->getBeforeNode().size() == 0){
                    scene->addArrow(cnnOutput2InputMap[idItemMap[nodes[i]->getId()]], idCNNItemMap[nowId]);
                }
                for (int k = 0; k < nextNode.size(); ++k){
                    scene->addArrow(idCNNItemMap[nowId], idCNNItemMap[nextNode[k]->getId()]);
                }                
            }
        }

    }
    for (int i = 0; i < nodes.size(); ++i){
        shared_ptr<AbstractLayer> layer = nodes[i]->getLayer();
        vector<shared_ptr<NetworkNode>> nextNode=nodes[i]->getNextNode();
        int nowId = nodes[i]->getId();
        for (int i = 0; i < nextNode.size(); ++i){
            if (nextNode[i]->getLayer()->getNetworkName() == "ConvolutionalNetworkLayer"){
                scene->addArrow(idItemMap[nowId], cnnOutput2InputMap[idItemMap[nodes[i]->getId()]]);
                continue;
            }
            scene->addArrow(idItemMap[nowId], idItemMap[nextNode[i]->getId()]);
        }
    }
    resetItemsPosition();
    if (master->getCriteria()->getType() == "MSE"){
        ui->networkCriteriaComboBox->setCurrentIndex(0);
    }
    if (master->getCriteria()->getType() == "Softmax"){
        ui->networkCriteriaComboBox->setCurrentIndex(1);
    }
}

void MainWindow::clearNetwork(){
    DiagramItem *ditem;
    std::vector<int> subnetToRemove;

    QList<QGraphicsItem*> boxItems;
    foreach (QGraphicsItem* boxItem, this->scene->items()){
        ditem = dynamic_cast<DiagramItem*>(boxItem);
        if (ditem != NULL){
            boxItems.append(ditem);
        }
    }
    
    //actually removing
    foreach(QGraphicsItem* boxItem, boxItems){
        ditem = dynamic_cast<DiagramItem*>(boxItem);
        if (ditem != NULL){
            if (ditem->getNetworkUnitPtr()->getTypeId() == NetworkUnitBase::CNN_INPUT_UNIT ||
                ditem->getNetworkUnitPtr()->getTypeId() == NetworkUnitBase::CNN_OUTPUT_UNIT){
                subnetToRemove.push_back((dynamic_cast<CNNUnitBase *>(ditem->getNetworkUnitPtr()))->getSubnetId());
                continue;
            }
            scene->removeNetworkUnit(ditem);
            continue;
        }
    }
    for (auto iterId = subnetToRemove.cbegin(); iterId != subnetToRemove.cend(); ++iterId){
        int id = *iterId;
        scene->removeSubnet(id);
    }
    dynamic_cast<TrainJob*>(workingThread)->resetMasterControl();
}

void MainWindow::togglePauseNetwork(){
    QString btnName=this->ui->pauseTrainingBtn->text();
    dynamic_cast<TrainJob*>(workingThread)->togglePauseFlag();
    if (btnName == "Pause") this->ui->pauseTrainingBtn->setText("Continue");
    else this->ui->pauseTrainingBtn->setText("Pause");
}