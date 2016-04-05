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
#include "include/Model.h"
#include "workingthread.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	currentHyperParameterWidget(nullptr)
{
	ui->setupUi(this);
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
QToolButton * MainWindow::createUnitBtn(QString const & title){
	QToolButton *retBtn = new QToolButton;
	retBtn->setText(title);
	retBtn->setCheckable(true);
	retBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
	return retBtn;
}
void MainWindow::addButtonToBasicUnitBtnGroup(QToolButton *& btn, const char * str, int id){
    btn = createUnitBtn(QString(str));
    basicUnitBtnGroup->addButton(btn, id);
}
void MainWindow::createUnitBox(){
	//clean old pages
	this->ui->unitToolBox->removeItem(0);
	//create basic unit box
	QWidget *basicUnitBox = new QWidget(this);
	QVBoxLayout *basicUnitBoxLayout = new QVBoxLayout(basicUnitBox);
    basicUnitBtnGroup = new QButtonGroup;
	basicUnitBox->setLayout(basicUnitBoxLayout);

	//add to btngroup
    addButtonToBasicUnitBtnGroup(moveCursorBtn, "Move", 1);
    addButtonToBasicUnitBtnGroup(lineUnitBtn, "Line", 2);
    addButtonToBasicUnitBtnGroup(inputUnitBtn, "Input Layer", 3);
    addButtonToBasicUnitBtnGroup(linearLayerUnitBtn, "Linear Layer", 4);
    addButtonToBasicUnitBtnGroup(nonlinearLayerUnitBtn, "Nonlinear Layer", 5);
    addButtonToBasicUnitBtnGroup(convolutionLayerUnitBtn, "Convolution Layer", 6);
    addButtonToBasicUnitBtnGroup(maxPoolingLayerUnitBtn, "Max Pooling Layer", 7);
    addButtonToBasicUnitBtnGroup(concatLayerUnitBtn, "Concat Layer", 8);
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
	connect(this->ui->actionSave_file, SIGNAL(triggered()), this, SLOT(saveFileAction()));
	connect(this->ui->actionOpen_file, SIGNAL(triggered()), this, SLOT(openFileAction()));
    connect(this->ui->actionTrain, SIGNAL(triggered()), this, SLOT(trainAction()));
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
        case 6: //convolution layer
            this->scene->setMode(DiagramScene::InsertItem);
            this->scene->setUnitTypeToInsert(NetworkUnitBase::NetworkUnitType::CONVOLUTION_UNIT);
            break;
        case 7: //max pooling
            this->scene->setMode(DiagramScene::InsertItem);
            this->scene->setUnitTypeToInsert(NetworkUnitBase::NetworkUnitType::MAXPOOLING_UNIT);
            break;
        case 8:
            this->scene->setMode(DiagramScene::InsertItem);
            this->scene->setUnitTypeToInsert(NetworkUnitBase::NetworkUnitType::CONCAT_UNIT);
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
	//actually removing
	foreach(QGraphicsItem* boxItem, selectedItems){
		ditem = dynamic_cast<DiagramItem*>(boxItem);
		//check arrows
		Arrow* arrow = dynamic_cast<Arrow*>(boxItem);
		if (arrow != nullptr) {
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
        else {
            scene->removeNetworkUnit(ditem);
        }
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
    connect(scene, SIGNAL(itemUnSelected(QGraphicsItem *)), this, SLOT(onItemUnSelected(QGraphicsItem *)));
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
void MainWindow::onAlert(QString msg){
    this->ui->networkOutputText->setText(this->ui->networkOutputText->toPlainText() + msg);
}

void MainWindow::trainAction(){
    TrainThread *thread = new TrainThread();
    fudandnn::Model & model = thread->getModel();
    SolverFactory sf;
    shared_ptr<Solver> solver(sf.createSolver(
        networkHyperParameterControl->getSolver(),
        networkHyperParameterControl->getRegularizationRate(),
        networkHyperParameterControl->getLearningRate(),
        networkHyperParameterControl->getLearningRate(),
        networkHyperParameterControl->getMomentumRate())
        );
    thread->setSolver(solver);
    this->networkHyperParameterControl->parseInformation(model);
    connect(thread, SIGNAL(alert(QString)), this, SLOT(onAlert(QString)));
    thread->start();
}

void MainWindow::createSettingBox(){
    this->networkHyperParameterControl = new NetworkHyperParameterControl();
    this->networkSettingWidget = new NetworkHyperParameterWidget(this->networkHyperParameterControl,this);
    this->ui->settingBox->removeItem(0);
    this->ui->settingBox->insertItem(0, this->networkSettingWidget, "Network Settings");
    this->ui->settingBox->setCurrentIndex(0);
    //ui->settingBox->removeItem(0);
	//Test
	//InputLayerHyperParameterWidget* ilhpw = new InputLayerHyperParameterWidget(nullptr, 0);
	//ui->settingBox->addItem(ilhpw, "Input Layer Setting");
}

void MainWindow::onItemSelected(QGraphicsItem *graphicsItem){
	DiagramItem *diagramItem = dynamic_cast<DiagramItem*> (graphicsItem);
	if (diagramItem != nullptr){
		HyperParameterWidget *newWidget= createHyperParameterWidget(diagramItem->getNetworkUnitPtr()->getHyperParameterControl());
        if (ui->settingBox->itemText(0) == "Layer Setting"){
            ui->settingBox->removeItem(0);
            delete currentHyperParameterWidget;
            currentHyperParameterWidget = nullptr;
        }
		ui->settingBox->insertItem(0, newWidget, "Layer Setting");
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
    if (dynamic_cast<NonlinearHyperParameterControl*>(control) != nullptr){
        return new NonlinearHyperParameterWidget(dynamic_cast<NonlinearHyperParameterControl*>(control), 0);
    }
    if (dynamic_cast<ConvolutionLayerHyperParameterControl*>(control) != nullptr){
        return new ConvolutionLayerHyperParameterWidget(dynamic_cast<ConvolutionLayerHyperParameterControl*>(control), 0);
    }
    if (dynamic_cast<MaxPoolingHyperParameterControl*>(control) != nullptr){
        return new MaxPoolingHyperParameterWidget(dynamic_cast<MaxPoolingHyperParameterControl*>(control), 0);
    }
    if (dynamic_cast<ConcatLayerHyperParameterControl*>(control) != nullptr){
        return new ConcatLayerHyperParameterWidget(dynamic_cast<ConcatLayerHyperParameterControl*> (control),0);
    }
	return nullptr;
}