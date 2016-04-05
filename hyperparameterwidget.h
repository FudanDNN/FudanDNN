#ifndef __HYPERPARAMETERWIDGET_H
#define __HYPERPARAMETERWIDGET_H
#include <qwidget.h>
#include <hyperparametercontrol.h>
#include <string>
#include <vector>
#include <utility>
#include <QLineEdit>
#include <qformlayout.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include "Constant.h"
class HyperParameterWidget :
	public QWidget
{
protected:
	HyperParameterControl * hyperParameterControl;
public:
	HyperParameterWidget(HyperParameterControl *control, QWidget* parent) : 
		QWidget(parent),hyperParameterControl(control) {}
	HyperParameterControl* getHyperParameterControl(){ return hyperParameterControl; }
	/* 解析并展示一个ParameterControl，需要具体的widget来实现*/
	virtual void parseParameterControl(HyperParameterControl *control) = 0;
	~HyperParameterWidget(){}
};
class NetworkHyperParameterWidget : public HyperParameterWidget{
Q_OBJECT
private:
    QFormLayout *layout;
    QComboBox *solverComboBox;
    QComboBox *criteriaComboBox;
    QSpinBox *outputSizeSpinBox, *trainingTimesSpinBox, *batchSizeSpinBox;
    QDoubleSpinBox *regularizationRateSpinBox;
    QDoubleSpinBox *learningRateSpinBox;
    QDoubleSpinBox *momentumRateSpinBox;
    using SolverNameMapPair = std::pair<QString, fudandnn::SOLVER>;
    const static std::vector<SolverNameMapPair> solverNameMap;
    using CriteriaNameMapPair = std::pair<QString, fudandnn::CRITERIA>;
    const static std::vector<CriteriaNameMapPair> criteriaNameMap;
public slots:
    void onSolverChanged(const QString & );
    void onCriteriaChanged(const QString &);
    void onRegularizationRateChanged(double);
    void onLearningRateChanged(double);
    void onMomentumRateChanged(double);
    void onOutputSizeChanged(int i);
    void onTrainingTimesChanged(int i);
    void onBatchSizeChanged(int i);
public:
    NetworkHyperParameterWidget(NetworkHyperParameterControl* control, QWidget *parent);
    void parseParameterControl(HyperParameterControl *control);
};
/* 输入层超参数*/
class InputLayerHyperParameterWidget : public HyperParameterWidget
{
Q_OBJECT
private:
	QPushButton *filename;
	QFormLayout *layout;
public slots:
	void onFileNameLineEditClicked();
public:
	InputLayerHyperParameterWidget(InputLayerHyperParameterControl* control, QWidget *parent);
	void parseParameterControl(HyperParameterControl *control) override;
	void setFile(const std::string &s);
	~InputLayerHyperParameterWidget(){}
};

/* 线性层超参数 */
class LinearLayerHyperParameterWidget : public HyperParameterWidget{
Q_OBJECT
private:
	QSpinBox *visualUnitCountSpinBox, *hiddenUnitCountSpinBox;
	QComboBox *initSchemeComboBox;
    using InitSchemeNameMapPair = std::pair<QString, fudandnn::initializationScheme>;
    static const std::vector<InitSchemeNameMapPair> initSchemeNameMap;
public slots:
	void onVisualUnitCountChanged(int);
	void onHiddenUnitCountChanged(int);
    void onInitSchemeChanged(const QString&);
public:
	LinearLayerHyperParameterWidget(LinearLayerHyperParameterControl* control, QWidget *parent);
	void parseParameterControl(HyperParameterControl *control) override;
	~LinearLayerHyperParameterWidget(){}
};

/* Nonlinear*/
class NonlinearHyperParameterWidget : public HyperParameterWidget{
    Q_OBJECT
private:
    QSpinBox *sizeSpinBox;
    QComboBox *typeComboBox;
    using NonlinearNameMapPair = std::pair<QString, fudandnn::NONLINEAR>;
    const static std::vector<NonlinearNameMapPair> nonlinearNameMap;
public slots:
    void onSizeChanged(int i);
    void onTypeChanged(const QString & typeStr);
public:
    NonlinearHyperParameterWidget(NonlinearHyperParameterControl *control, QWidget *parent);
    void parseParameterControl(HyperParameterControl *control) override;
    ~NonlinearHyperParameterWidget(){}
};

/* Convolution */
class ConvolutionLayerHyperParameterWidget : public HyperParameterWidget{
    Q_OBJECT
private:
    QSpinBox *rowSizeSpinBox, *columnSizeSpinBox;
    QSpinBox *kRowSizeSpinBox, *kColumnSizeSpinBox;
    QSpinBox *channelSpinBox, *featureMapSizeSpinBox, *strideSpinBox;
    QComboBox *initSchemeComboBox;
    QFormLayout * layout;
    using InitSchemeNameMapPair = std::pair<QString, fudandnn::initializationScheme>;
    static const std::vector<InitSchemeNameMapPair> initSchemeNameMap;
public slots:
    void onRowSizeChanged(int i);
    void onColumnSizeChanged(int i);
    void onKRowSizeChanged(int i);
    void onKColumnSizeChanged(int i);
    void onChannelChanged(int i);
    void onFeatureMapSizeChanged(int i);
    void onStrideChanged(int i);
    void onInitSchemeChanged(const QString & str);
public:
    ConvolutionLayerHyperParameterWidget(ConvolutionLayerHyperParameterControl* control, QWidget *parent);
    void parseParameterControl(HyperParameterControl *control) override;
    ~ConvolutionLayerHyperParameterWidget(){}
};

/*Max Pooling*/
class MaxPoolingHyperParameterWidget : public HyperParameterWidget{
    Q_OBJECT
private:
    QSpinBox *rowSizeSpinBox, *columnSizeSpinBox;
    QSpinBox *kRowSizeSpinBox, *kColumnSizeSpinBox;
    QSpinBox *channelSpinBox, *featureMapSizeSpinBox, *strideSpinBox;
    QFormLayout * layout;
    public slots:
    void onRowSizeChanged(int i);
    void onColumnSizeChanged(int i);
    void onKRowSizeChanged(int i);
    void onKColumnSizeChanged(int i);
    void onChannelChanged(int i);
    void onFeatureMapSizeChanged(int i);
    void onStrideChanged(int i);
public:
    MaxPoolingHyperParameterWidget(MaxPoolingHyperParameterControl* control, QWidget *parent);
    void parseParameterControl(HyperParameterControl *control) override;
    ~MaxPoolingHyperParameterWidget(){}
};

/*Concat Layer*/
class ConcatLayerHyperParameterWidget : public HyperParameterWidget{
    Q_OBJECT
private:
    QSpinBox *rowSizeSpinBox, *columnSizeSpinBox;
    QSpinBox *channelSpinBox;
    QFormLayout * layout;
    public slots:
    void onRowSizeChanged(int i);
    void onColumnSizeChanged(int i);
    void onChannelChanged(int i);
public:
    ConcatLayerHyperParameterWidget(ConcatLayerHyperParameterControl* control, QWidget *parent);
    void parseParameterControl(HyperParameterControl *control) override;
    ~ConcatLayerHyperParameterWidget(){}
};

#endif 