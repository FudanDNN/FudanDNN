#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
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
    void onItemUnSelected(QGraphicsItem *);
    void trainAction();
    void onAlert(QString);
private:
    Ui::MainWindow *ui;
    DiagramScene *scene;
	QToolButton *inputUnitBtn;
	QToolButton *linearLayerUnitBtn;
	QToolButton *nonlinearLayerUnitBtn;
	QToolButton *lineUnitBtn;
	QToolButton *moveCursorBtn;
    QToolButton *convolutionLayerUnitBtn;
	QToolButton *maxPoolingLayerUnitBtn;
    QToolButton *concatLayerUnitBtn;
	QMenu *itemMenu;
    QButtonGroup *basicUnitBtnGroup = nullptr;
	HyperParameterWidget *currentHyperParameterWidget;
    NetworkHyperParameterWidget *networkSettingWidget;
    NetworkHyperParameterControl *networkHyperParameterControl;
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
	HyperParameterWidget* createHyperParameterWidget(HyperParameterControl *);
	
    void addButtonToBasicUnitBtnGroup(QToolButton * &btn, const char* str, int id);
};

#endif // MAINWINDOW_H
