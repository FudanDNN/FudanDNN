#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include "workingthread.h"
#include "diagramscene.h"
#include "hyperparameterwidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
	void basicUnitBtnGroupClicked(int);
	void deleteItem();
	void useMoveCursor();
	void outputNetworkAction();
	/* 根据参数整理items的位置，使得整个图形变得好看*/
	void resetItemsPosition();
	void saveFileAction();
	void openFileAction();
	void onItemSelected(QGraphicsItem *);
    void doAlert();
    void onItemUnSelected(QGraphicsItem *);
private slots:
    void startTraining();
    void startPredict();
    void abortWorkingThread();
    void joinWorkingThread();
    void saveNetwork();
    void loadNetwork();
    void clearNetwork();
    void togglePauseNetwork();
private:
    WorkingThread* workingThread;
    Ui::MainWindow *ui;
    DiagramScene *scene;
	QToolButton *inputUnitBtn;
	QToolButton *linearLayerUnitBtn;
	QToolButton *nonlinearLayerUnitBtn;
	QToolButton *lineUnitBtn;
	QToolButton *moveCursorBtn;
	QToolButton *cnnSubnetBtn;
	QToolButton *cnnConvolutionalUnitBtn;
	QToolButton *cnnMaxPoolingUnitBtn;
    QToolButton *cnnNonlinearLayerUnitBtn;
    QToolButton *sigmoidUnitBtn;
	QMenu *itemMenu;
	HyperParameterWidget *currentHyperParameterWidget;
	/*
	 用于产生一个网络单元按钮的工厂方法
	 未来应该修改成接受一个NetworkUnitBase或者类似物的方法
	*/
	QToolButton *createUnitBtn(QString const & title);
	/*
	创建工具栏：可选的网络单元选框
	*/
	void createUnitBox();
	/*
	创建场景中放置的那些块的右键菜单
	*/
	void createItemMenu();
	/*
	创建默认场景的方法
	*/
	void createScene();
	/*
	连接菜单按钮与对应的handler的方法
	*/
	void connectActions();
	/*
	构造settingBox的方法
	*/
	void createSettingBox();
	/* 构造超参数控制界面的方法 */
    void createWorkingThread();
	HyperParameterWidget* createHyperParameterWidget(HyperParameterControl *);
    void rebuildNetwork(shared_ptr<MasterControl> master);
	
};

#endif // MAINWINDOW_H
