#ifndef HYPERPARAMETERWIDGET_H
#define HYPERPARAMETERWIDGET_H
#include <qwidget.h>
#include <hyperparametercontrol.h>
#include <string>
#include <QLineEdit>
#include <qformlayout.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qcombobox.h>
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
    QDoubleSpinBox *regularizationRateSpinBox, *weightLearningRateSpinBox, *biasLearningRateSpinBox, *momentumRateSpinBox;
	QComboBox *initSchemeComboBox;
public slots:
    void onVisualUnitCountChanged(int i){
        dynamic_cast<LinearLayerHyperParameterControl*>(this->hyperParameterControl)->setVisualUnitCount(i);
    }
    void onHiddenUnitCountChanged(int i){
        dynamic_cast<LinearLayerHyperParameterControl*>(this->hyperParameterControl)->setHiddenUnitCount(i);
    }
    void onInitSchemeChanged(int i){
        AbstractNetworkLayer::initializationScheme initScheme = (AbstractNetworkLayer::initializationScheme) i;
        dynamic_cast<LinearLayerHyperParameterControl*>(this->hyperParameterControl)->setInitScheme(initScheme);
    }
    void onRegularizationRateChanged(double d){
        dynamic_cast<LinearLayerHyperParameterControl*>(this->hyperParameterControl)->setRegularizationRate(d);
    }
    void onWeightLearningRateChanged(double d){
        dynamic_cast<LinearLayerHyperParameterControl*>(this->hyperParameterControl)->setWeightLearningRate(d);
    }
    void onBiasLearningRateChanged(double d){
        dynamic_cast<LinearLayerHyperParameterControl*>(this->hyperParameterControl)->setBiasLearningRate(d);
    }
    void onMomentumRateChanged(double d){
        dynamic_cast<LinearLayerHyperParameterControl*>(this->hyperParameterControl)->setMomentumRate(d);
    }
public:
	LinearLayerHyperParameterWidget(LinearLayerHyperParameterControl* control, QWidget *parent);
	void parseParameterControl(HyperParameterControl *control) override;
	~LinearLayerHyperParameterWidget(){}
};

/* Sigmoid层参数*/
class NonLinearLayerHyperParameterWidget : public HyperParameterWidget{
Q_OBJECT
private:
    QSpinBox *sizeSpinBox;
    QComboBox *typeComboBox;
public slots:
    void onSizeChanged(int i){
        dynamic_cast<NonLinearLayerHyperParameterControl*>(this->hyperParameterControl)->setSize(i);
    }
    void onTypeChanged(int t){
        NonLinearFactory::NONLINEAR type = (NonLinearFactory::NONLINEAR)t;
        dynamic_cast<NonLinearLayerHyperParameterControl*>(this->hyperParameterControl)->setType(type);
    }
public:
    NonLinearLayerHyperParameterWidget(NonLinearLayerHyperParameterControl *control, QWidget *parent);
    void parseParameterControl(HyperParameterControl *control) override;
    ~NonLinearLayerHyperParameterWidget(){}
};

/* CNN子网络的参数 */
class CNNSubnetHyperParameterWidget : public HyperParameterWidget{
Q_OBJECT
private:
    QLineEdit *cnnIdLineEdit;
public:
    CNNSubnetHyperParameterWidget(CNNSubnetHyperParameterControl* control, QWidget *parent);
    void parseParameterControl(HyperParameterControl *control) override;
    ~CNNSubnetHyperParameterWidget(){}
};
/* CNN IO Unit */
class CNNIOHyperParameterWidget : public HyperParameterWidget{
    Q_OBJECT
private:
    QLineEdit *cnnIdLineEdit;
public:
    CNNIOHyperParameterWidget(CNNIOHyperParameterControl* control, QWidget *parent);
    void parseParameterControl(HyperParameterControl *control) override;
    ~CNNIOHyperParameterWidget(){}
};
/*CNN Convolution Layer*/
class CNNConvolutionLayerHyperParameterWidget : public HyperParameterWidget{
    Q_OBJECT
private:
    QSpinBox *kernelSizeSpinBox, *strideSpinBox, *featureMapNumSpinBox, *numSpinBox;
    QSpinBox *visualRowSpinBox, *visualColumnSpinBox;
    QComboBox *initSchemeComboBox;
    QDoubleSpinBox *regularizationRateSpinBox, *kernelLearningRateSpinBox, *biasLearningRateSpinBox, *momentumRateSpinBox;
    QLineEdit *cnnIdLineEdit;
public slots:
    void onKernelSizeChanged(int);
    void onStrideChanged(int);
    void onFeatureMapNumChanged(int);
    void onNumChanged(int);
    void onVisualRowChanged(int);
    void onVisualColumnChanged(int);
    void onInitSchemeChanged(int);
    void onRegularizationRateChanged(double d){
        dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setRegularizationRate(d);
    }
    void onKernelLearningRate(double d){
        dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setKernelLearningRate(d);
    }
    void onBiasLearningRateChanged(double d){
        dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setBiasLearningRate(d);
    }
    void onMomentumRateChanged(double d){
        dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setMomentumRate(d);
    }
public:
    CNNConvolutionLayerHyperParameterWidget(CNNConvolutionLayerHyperParameterControl* control, QWidget *parent);
    void parseParameterControl(HyperParameterControl *control) override;
    ~CNNConvolutionLayerHyperParameterWidget(){}
};

/*CNN Max Pooling Layer*/
class CNNMaxPoolingLayerHyperParameterWidget : public HyperParameterWidget{
    Q_OBJECT
private:
    QSpinBox *poolingSizeSpinBox, *strideSpinBox, *numSpinBox;
    QSpinBox *visualRowSpinBox, *visualColumnSpinBox;
    QLineEdit *cnnIdLineEdit;
public slots:
    void onPoolingSizeChanged(int);
    void onStrideChanged(int);
    void onNumChanged(int);
    void onVisualRowChanged(int);
    void onVisualColumnChanged(int);
public:
    CNNMaxPoolingLayerHyperParameterWidget(CNNMaxPoolingLayerHyperParameterControl* control, QWidget *parent);
    void parseParameterControl(HyperParameterControl *control) override;
    ~CNNMaxPoolingLayerHyperParameterWidget(){}
};

/*CNN NonLinear Layer*/
class CNNNonLinearLayerHyperParameterWidget : public HyperParameterWidget{
    Q_OBJECT
private:
    QSpinBox *numSpinBox, *visualRowSpinBox, *visualColumnSpinBox;
    QComboBox *typeComboBox;
    QLineEdit *cnnIdLineEdit;

public slots:
    void onNumChanged(int);
    void onVisualRowChanged(int);
    void onVisualColumnChanged(int);
    void onTypeChanged(int);
public:
    CNNNonLinearLayerHyperParameterWidget(CNNNonLinearLayerHyperParameterControl* control, QWidget *parent);
    void parseParameterControl(HyperParameterControl *control) override;
    ~CNNNonLinearLayerHyperParameterWidget(){}
};
#endif