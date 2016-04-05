#include "hyperparameterwidget.h"
#include <qfiledialog.h>
#include <qpushbutton.h>
template<typename PAIR_TYPE>
static void initComboBoxWithNamePair(const std::vector<PAIR_TYPE> nameMap, QComboBox * comboBox){
    for (auto p = nameMap.cbegin(); p != nameMap.cend(); ++p){
        comboBox->addItem(p->first);
    }
}
template<typename PAIR_TYPE, typename VALUE_TYPE>
static void parseComboBoxValue(const std::vector<PAIR_TYPE> nameMap, const VALUE_TYPE& value, QComboBox *comboBox){
    for (int i = 0; i != nameMap.size(); ++i){
        if (nameMap[i].second == value) {
            comboBox->setCurrentIndex(i);
            break;
        }
    }
}
/* NetworkHyperParameterWidget*/
const std::vector<NetworkHyperParameterWidget::SolverNameMapPair> 
NetworkHyperParameterWidget::solverNameMap{
    SolverNameMapPair{"SGD", fudandnn::SOLVER::SGD},
    SolverNameMapPair{"ADAGRADE", fudandnn::SOLVER::ADAGRAD }
};
const std::vector<NetworkHyperParameterWidget::CriteriaNameMapPair>
NetworkHyperParameterWidget::criteriaNameMap{
    CriteriaNameMapPair{ "MSE",fudandnn::CRITERIA::MSE },
    CriteriaNameMapPair{ "SOFTMAX",fudandnn::CRITERIA::SOFTMAX }
};
NetworkHyperParameterWidget::NetworkHyperParameterWidget(NetworkHyperParameterControl* control, QWidget *parent):
HyperParameterWidget(control,parent)
{
    this->solverComboBox = new QComboBox(this);
    initComboBoxWithNamePair(solverNameMap, solverComboBox);
    this->criteriaComboBox = new QComboBox(this);
    initComboBoxWithNamePair(criteriaNameMap, criteriaComboBox);
    this->outputSizeSpinBox = new QSpinBox(this); this->outputSizeSpinBox->setMaximum(99999);
    this->trainingTimesSpinBox = new QSpinBox(this); this->trainingTimesSpinBox->setMaximum(99999);
    this->batchSizeSpinBox = new QSpinBox(this); this->batchSizeSpinBox->setMaximum(99999);
    this->regularizationRateSpinBox = new QDoubleSpinBox(this);
    this->regularizationRateSpinBox->setSingleStep(0.0001);
    this->regularizationRateSpinBox->setDecimals(5);
    this->learningRateSpinBox = new QDoubleSpinBox(this);
    this->learningRateSpinBox->setSingleStep(0.01);
    this->learningRateSpinBox->setDecimals(4);
    this->momentumRateSpinBox = new QDoubleSpinBox(this);
    this->momentumRateSpinBox->setSingleStep(0.001);
    this->momentumRateSpinBox->setDecimals(4);
    parseParameterControl(control);
    connect(outputSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOutputSizeChanged(int)));
    connect(trainingTimesSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onTrainingTimesChanged(int)));
    connect(batchSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onBatchSizeChanged(int)));
    connect(solverComboBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(onSolverChanged(const QString &)));
    connect(criteriaComboBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(onCriteriaChanged(const QString &)));
    connect(regularizationRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onRegularizationRateChanged(double)));
    connect(learningRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onLearningRateChanged(double)));
    connect(momentumRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onMomentumRateChanged(double)));
    layout = new QFormLayout();
    layout->addRow("Solver:", solverComboBox);
    layout->addRow("Criteria:", criteriaComboBox);
    layout->addRow("Output Size:", outputSizeSpinBox);
    layout->addRow("Training Times:", trainingTimesSpinBox);
    layout->addRow("Batch Size:", batchSizeSpinBox);
    layout->addRow("Learning Rate:", learningRateSpinBox);
    layout->addRow("Regularization Rate:", regularizationRateSpinBox);
    layout->addRow("Momentum Rate:", momentumRateSpinBox);
    setLayout(layout);
}
void NetworkHyperParameterWidget::parseParameterControl(HyperParameterControl *control_p){
    NetworkHyperParameterControl *control = dynamic_cast<NetworkHyperParameterControl*>(control_p);
    parseComboBoxValue(solverNameMap, control->getSolver(), solverComboBox);
    parseComboBoxValue(criteriaNameMap, control->getCriteria(), criteriaComboBox);
    regularizationRateSpinBox->setValue(control->getRegularizationRate());
    learningRateSpinBox->setValue(control->getLearningRate());
}
void NetworkHyperParameterWidget::onSolverChanged(const QString & str){
    NetworkHyperParameterControl *control = dynamic_cast<NetworkHyperParameterControl*>(this->hyperParameterControl);
    for (auto p = solverNameMap.cbegin(); p != solverNameMap.cend(); ++p){
        if (p->first == str){
            control->setSolver(p->second);
            break;
        }
    }
}
void NetworkHyperParameterWidget::onCriteriaChanged(const QString & str){
    NetworkHyperParameterControl *control = dynamic_cast<NetworkHyperParameterControl*>(this->hyperParameterControl);
    for (auto p = criteriaNameMap.cbegin(); p != criteriaNameMap.cend(); ++p){
        if (p->first == str){
            control->setCriteria(p->second);
            break;
        }
    }
}
void NetworkHyperParameterWidget::onRegularizationRateChanged(double v){
    NetworkHyperParameterControl *control = dynamic_cast<NetworkHyperParameterControl*>(this->hyperParameterControl);
    control->setRegularizationRate(v);
}
void NetworkHyperParameterWidget::onLearningRateChanged(double v){
    NetworkHyperParameterControl *control = dynamic_cast<NetworkHyperParameterControl*>(this->hyperParameterControl);
    control->setLearningRate(v);
}
void NetworkHyperParameterWidget::onMomentumRateChanged(double v){
    NetworkHyperParameterControl *control = dynamic_cast<NetworkHyperParameterControl*>(this->hyperParameterControl);
    control->setMomentumRate(v);
}
void NetworkHyperParameterWidget::onOutputSizeChanged(int v){
    NetworkHyperParameterControl *control = dynamic_cast<NetworkHyperParameterControl*>(this->hyperParameterControl);
    control->setOutputSize(v);
}
void NetworkHyperParameterWidget::onTrainingTimesChanged(int v){
    NetworkHyperParameterControl *control = dynamic_cast<NetworkHyperParameterControl*>(this->hyperParameterControl);
    control->setTrainingTimes(v);
}
void NetworkHyperParameterWidget::onBatchSizeChanged(int v){
    NetworkHyperParameterControl *control = dynamic_cast<NetworkHyperParameterControl*>(this->hyperParameterControl);
    control->setBatchSize(v);
}
/* InputLayerHyperParameterWidget*/
InputLayerHyperParameterWidget::InputLayerHyperParameterWidget(InputLayerHyperParameterControl* control, QWidget *parent) :
HyperParameterWidget(control, parent)
{
	this->filename = new QPushButton();
	parseParameterControl(control);
	layout = new QFormLayout();
	layout->addRow(tr("Filename:"), this->filename);
	connect(this->filename, SIGNAL(clicked()), this, SLOT(onFileNameLineEditClicked()));
	setLayout(layout);
}
void InputLayerHyperParameterWidget::setFile(const std::string &file){
	this->filename->setText(QString::fromStdString(file));
	InputLayerHyperParameterControl* ilhpc = dynamic_cast<InputLayerHyperParameterControl*>(this->hyperParameterControl);
	if (ilhpc!=nullptr) ilhpc->setFile(file);
}

void InputLayerHyperParameterWidget::parseParameterControl(HyperParameterControl *control){
	if (control == nullptr) return;
	this->filename->setText(QString::fromStdString(
		dynamic_cast<InputLayerHyperParameterControl*>(this->hyperParameterControl)->getFile()
	));
}

void InputLayerHyperParameterWidget::onFileNameLineEditClicked(){
	QFileDialog fileDialog(this);
	fileDialog.setWindowTitle("Open file");
	fileDialog.setFileMode(QFileDialog::AnyFile);
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setNameFilter("XML files (*.xml);;All Files (*.*)");
	if (fileDialog.exec() == QFileDialog::Accepted)
	{
		std::string filename = fileDialog.selectedFiles()[0].toStdString();
		this->setFile(filename);
	}
}

/* LinearLayerHyperParameterWidget */
const std::vector<LinearLayerHyperParameterWidget::InitSchemeNameMapPair> 
LinearLayerHyperParameterWidget::initSchemeNameMap{
    InitSchemeNameMapPair("RANDOM",fudandnn::RANDOM),
    InitSchemeNameMapPair("RANDOM_SQRT", fudandnn::RANDOM_SQRT),
    InitSchemeNameMapPair("RANDOM_NORM1", fudandnn::RANDOM_NORM1),
};
LinearLayerHyperParameterWidget::LinearLayerHyperParameterWidget(LinearLayerHyperParameterControl* control, QWidget *parent)
	:HyperParameterWidget(control,parent)
{
	this->visualUnitCountSpinBox = new QSpinBox(this);
	this->hiddenUnitCountSpinBox = new QSpinBox(this);
	this->initSchemeComboBox = new QComboBox(this);
    this->visualUnitCountSpinBox->setMaximum(99999);
    this->hiddenUnitCountSpinBox->setMaximum(99999);
    initComboBoxWithNamePair(initSchemeNameMap, initSchemeComboBox);
	parseParameterControl(control);
	connect(this->visualUnitCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onVisualUnitCountChanged(int)));
	connect(this->hiddenUnitCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onHiddenUnitCountChanged(int)));
    connect(this->initSchemeComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(onInitSchemeChanged(const QString &)));
    QFormLayout *formLayout = new QFormLayout(this);
	formLayout->addRow("Visual Unit Count:", visualUnitCountSpinBox);
	formLayout->addRow("Hidden Unit Count:", hiddenUnitCountSpinBox);
	formLayout->addRow("Init Scheme:", initSchemeComboBox);
	setLayout(formLayout);
}
void LinearLayerHyperParameterWidget::parseParameterControl(HyperParameterControl * control){
	LinearLayerHyperParameterControl *linearControl = dynamic_cast<LinearLayerHyperParameterControl*> (control);
	this->visualUnitCountSpinBox->setValue(linearControl->getVisualUnitCount());
	this->hiddenUnitCountSpinBox->setValue(linearControl->getHiddenUnitCount());
    parseComboBoxValue(initSchemeNameMap, linearControl->getInitScheme(), initSchemeComboBox);
}
void LinearLayerHyperParameterWidget::onVisualUnitCountChanged(int i){
	dynamic_cast<LinearLayerHyperParameterControl*>(this->hyperParameterControl)->setVisualUnitCount(i);
}
void LinearLayerHyperParameterWidget::onHiddenUnitCountChanged(int i){
	dynamic_cast<LinearLayerHyperParameterControl*>(this->hyperParameterControl)->setHiddenUnitCount(i);
}
void LinearLayerHyperParameterWidget::onInitSchemeChanged(const QString & str){
    for (auto p = initSchemeNameMap.cbegin(); p != initSchemeNameMap.cend(); ++p){
        if (str == p->first){
            dynamic_cast<LinearLayerHyperParameterControl*> (this->hyperParameterControl)
                ->setInitScheme(p->second);
        }
    }
}
/* Nonlinear */
const std::vector<NonlinearHyperParameterWidget::NonlinearNameMapPair>
NonlinearHyperParameterWidget::nonlinearNameMap
{
    NonlinearNameMapPair({ QString("SIGMOID"), fudandnn::NONLINEAR::SIGMOID }),
    NonlinearNameMapPair({ QString("ReLU"), fudandnn::NONLINEAR::RELU })
};
NonlinearHyperParameterWidget::NonlinearHyperParameterWidget(NonlinearHyperParameterControl *control, QWidget *parent)
    :HyperParameterWidget(control, parent){
    this->sizeSpinBox = new QSpinBox(this);
    this->typeComboBox = new QComboBox(this);
    this->sizeSpinBox->setMaximum(99999);
    initComboBoxWithNamePair(nonlinearNameMap, typeComboBox);
    parseParameterControl(control);
    connect(this->sizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSizeChanged(int)));
    connect(this->typeComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(onTypeChanged(const QString &)));
    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow("Visual Unit Count:", sizeSpinBox);
    formLayout->addRow("Type:", typeComboBox);
    setLayout(formLayout);
}
void NonlinearHyperParameterWidget::parseParameterControl(HyperParameterControl *control_p){
    NonlinearHyperParameterControl * control = dynamic_cast<NonlinearHyperParameterControl *>(control_p);
    this->sizeSpinBox->setValue(control->getVisualUnitCount());
    parseComboBoxValue(nonlinearNameMap, control->getType(), typeComboBox);
}
void NonlinearHyperParameterWidget::onSizeChanged(int i){
    dynamic_cast<NonlinearHyperParameterControl*>(this->hyperParameterControl)->setVisualUnitCount(i);
}
void NonlinearHyperParameterWidget::onTypeChanged(const QString & typeStr){
    for (auto p = nonlinearNameMap.cbegin(); p != nonlinearNameMap.cend(); ++p){
        if (typeStr == p->first){
            dynamic_cast<NonlinearHyperParameterControl*> (this->hyperParameterControl)
                ->setType(p->second);
        }
    }
}

/* Convolution*/
const std::vector<ConvolutionLayerHyperParameterWidget::InitSchemeNameMapPair>
ConvolutionLayerHyperParameterWidget::initSchemeNameMap{
    InitSchemeNameMapPair("RANDOM", fudandnn::RANDOM),
    InitSchemeNameMapPair("RANDOM_SQRT", fudandnn::RANDOM_SQRT),
    InitSchemeNameMapPair("RANDOM_NORM1", fudandnn::RANDOM_NORM1),
};
ConvolutionLayerHyperParameterWidget::ConvolutionLayerHyperParameterWidget(ConvolutionLayerHyperParameterControl* control, QWidget *parent)
    :HyperParameterWidget(control, parent){
    layout = new QFormLayout(this);
    setLayout(layout);
    this->rowSizeSpinBox = new QSpinBox(this); this->rowSizeSpinBox->setMaximum(99999);
    this->columnSizeSpinBox = new QSpinBox(this); this->columnSizeSpinBox->setMaximum(99999);
    this->kRowSizeSpinBox = new QSpinBox(this); this->kRowSizeSpinBox->setMaximum(99999);
    this->kColumnSizeSpinBox = new QSpinBox(this); this->kColumnSizeSpinBox->setMaximum(99999);
    this->channelSpinBox = new QSpinBox(this); this->channelSpinBox->setMaximum(99999);
    this->featureMapSizeSpinBox = new QSpinBox(this); this->featureMapSizeSpinBox->setMaximum(99999);
    this->strideSpinBox = new QSpinBox(this); this->strideSpinBox->setMaximum(99999);
    this->initSchemeComboBox = new QComboBox(this); 
    initComboBoxWithNamePair(initSchemeNameMap, initSchemeComboBox);
    connect(rowSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onRowSizeChanged(int)));
    connect(columnSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onColumnSizeChanged(int)));
    connect(kRowSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onKRowSizeChanged(int)));
    connect(kColumnSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onKColumnSizeChanged(int)));
    connect(channelSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onChannelChanged(int)));
    connect(featureMapSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFeatureMapSizeChanged(int)));
    connect(strideSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onStrideChanged(int)));
    connect(initSchemeComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(onInitSchemeChanged(const QString &)));
    parseParameterControl(control);
    layout->addRow("Row Size:", rowSizeSpinBox);
    layout->addRow("Column Size:", columnSizeSpinBox);
    layout->addRow("Kernel Row Size:", kRowSizeSpinBox);
    layout->addRow("Kernel Column Size:", kColumnSizeSpinBox);
    layout->addRow("Channel Size:", channelSpinBox);
    layout->addRow("Feature Map Size:", featureMapSizeSpinBox);
    layout->addRow("Stride:", strideSpinBox);
    layout->addRow("Init Scheme:", initSchemeComboBox);
}
void ConvolutionLayerHyperParameterWidget::parseParameterControl(HyperParameterControl *control_p){
    auto control = dynamic_cast<ConvolutionLayerHyperParameterControl*> (control_p);
    this->rowSizeSpinBox->setValue(control->getRowSize());
    this->columnSizeSpinBox->setValue(control->getColumnSize());
    this->kRowSizeSpinBox->setValue(control->getKRowSize());
    this->kColumnSizeSpinBox->setValue(control->getKColumnSize());
    this->channelSpinBox->setValue(control->getChannelSize());
    this->featureMapSizeSpinBox->setValue(control->getFeatureMapSize());
    this->strideSpinBox->setValue(control->getStride());
    parseComboBoxValue(initSchemeNameMap, control->getInitScheme(), initSchemeComboBox);
}
void ConvolutionLayerHyperParameterWidget::onRowSizeChanged(int i){
    dynamic_cast<ConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setRowSize(i);
}
void ConvolutionLayerHyperParameterWidget::onColumnSizeChanged(int i){
    dynamic_cast<ConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setColumnSize(i);
}
void ConvolutionLayerHyperParameterWidget::onKRowSizeChanged(int i){
    dynamic_cast<ConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setKRowSize(i);
}
void ConvolutionLayerHyperParameterWidget::onKColumnSizeChanged(int i){
    dynamic_cast<ConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setKColumnSize(i);
}
void ConvolutionLayerHyperParameterWidget::onChannelChanged(int i){
    dynamic_cast<ConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setChannelSize(i);
}
void ConvolutionLayerHyperParameterWidget::onFeatureMapSizeChanged(int i){
    dynamic_cast<ConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setFeatureMapSize(i);
}
void ConvolutionLayerHyperParameterWidget::onStrideChanged(int i){
    dynamic_cast<ConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setStride(i);
}
void ConvolutionLayerHyperParameterWidget::onInitSchemeChanged(const QString & str){
    for (auto p = initSchemeNameMap.cbegin(); p != initSchemeNameMap.cend(); ++p){
        if (str == p->first){
            dynamic_cast<ConvolutionLayerHyperParameterControl*>(this->hyperParameterControl)->setInitScheme(p->second);
            break;
        }
    }
}

/*Max Pooling*/
MaxPoolingHyperParameterWidget::MaxPoolingHyperParameterWidget(MaxPoolingHyperParameterControl* control, QWidget *parent)
    :HyperParameterWidget(control, parent){
    layout = new QFormLayout(this);
    setLayout(layout);
    this->rowSizeSpinBox = new QSpinBox(this); this->rowSizeSpinBox->setMaximum(99999);
    this->columnSizeSpinBox = new QSpinBox(this); this->columnSizeSpinBox->setMaximum(99999);
    this->kRowSizeSpinBox = new QSpinBox(this); this->kRowSizeSpinBox->setMaximum(99999);
    this->kColumnSizeSpinBox = new QSpinBox(this); this->kColumnSizeSpinBox->setMaximum(99999);
    this->channelSpinBox = new QSpinBox(this); this->channelSpinBox->setMaximum(99999);
    this->featureMapSizeSpinBox = new QSpinBox(this); this->featureMapSizeSpinBox->setMaximum(99999);
    this->strideSpinBox = new QSpinBox(this); this->strideSpinBox->setMaximum(99999);
    connect(rowSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onRowSizeChanged(int)));
    connect(columnSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onColumnSizeChanged(int)));
    connect(kRowSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onKRowSizeChanged(int)));
    connect(kColumnSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onKColumnSizeChanged(int)));
    connect(channelSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onChannelChanged(int)));
    connect(featureMapSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFeatureMapSizeChanged(int)));
    connect(strideSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onStrideChanged(int)));
    parseParameterControl(control);
    layout->addRow("Row Size:", rowSizeSpinBox);
    layout->addRow("Column Size:", columnSizeSpinBox);
    layout->addRow("Kernel Row Size:", kRowSizeSpinBox);
    layout->addRow("Kernel Column Size:", kColumnSizeSpinBox);
    layout->addRow("Stride Size:", strideSpinBox);
    layout->addRow("Channel Size:", channelSpinBox);
    layout->addRow("Feature Map Size:", featureMapSizeSpinBox);
}
void MaxPoolingHyperParameterWidget::parseParameterControl(HyperParameterControl *control_p){
    auto control = dynamic_cast<MaxPoolingHyperParameterControl*> (control_p);
    this->rowSizeSpinBox->setValue(control->getRowSize());
    this->columnSizeSpinBox->setValue(control->getColumnSize());
    this->kRowSizeSpinBox->setValue(control->getKRowSize());
    this->kColumnSizeSpinBox->setValue(control->getKColumnSize());
    this->channelSpinBox->setValue(control->getChannelSize());
    this->featureMapSizeSpinBox->setValue(control->getFeatureMapSize());
    this->strideSpinBox->setValue(control->getStride());
}
void MaxPoolingHyperParameterWidget::onRowSizeChanged(int i){
    dynamic_cast<MaxPoolingHyperParameterControl*>(this->hyperParameterControl)->setRowSize(i);
}
void MaxPoolingHyperParameterWidget::onColumnSizeChanged(int i){
    dynamic_cast<MaxPoolingHyperParameterControl*>(this->hyperParameterControl)->setColumnSize(i);
}
void MaxPoolingHyperParameterWidget::onKRowSizeChanged(int i){
    dynamic_cast<MaxPoolingHyperParameterControl*>(this->hyperParameterControl)->setKRowSize(i);
}
void MaxPoolingHyperParameterWidget::onKColumnSizeChanged(int i){
    dynamic_cast<MaxPoolingHyperParameterControl*>(this->hyperParameterControl)->setKColumnSize(i);
}
void MaxPoolingHyperParameterWidget::onChannelChanged(int i){
    dynamic_cast<MaxPoolingHyperParameterControl*>(this->hyperParameterControl)->setChannelSize(i);
}
void MaxPoolingHyperParameterWidget::onFeatureMapSizeChanged(int i){
    dynamic_cast<MaxPoolingHyperParameterControl*>(this->hyperParameterControl)->setFeatureMapSize(i);
}
void MaxPoolingHyperParameterWidget::onStrideChanged(int i){
    dynamic_cast<MaxPoolingHyperParameterControl*>(this->hyperParameterControl)->setStride(i);
}

/*Concat Layer*/
ConcatLayerHyperParameterWidget::ConcatLayerHyperParameterWidget(ConcatLayerHyperParameterControl* control, QWidget *parent)
    :HyperParameterWidget(control, parent){
    layout = new QFormLayout(this);
    setLayout(layout);
    this->rowSizeSpinBox = new QSpinBox(this); this->rowSizeSpinBox->setMaximum(99999);
    this->columnSizeSpinBox = new QSpinBox(this); this->columnSizeSpinBox->setMaximum(99999);
    this->channelSpinBox = new QSpinBox(this); this->channelSpinBox->setMaximum(99999);
    connect(rowSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onRowSizeChanged(int)));
    connect(columnSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onColumnSizeChanged(int)));
    connect(channelSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onChannelChanged(int)));
    parseParameterControl(control);
    layout->addRow("Row Size:", rowSizeSpinBox);
    layout->addRow("Column Size:", columnSizeSpinBox);
    layout->addRow("Channel Size:", channelSpinBox);
}
void ConcatLayerHyperParameterWidget::parseParameterControl(HyperParameterControl *control_p){
    auto control = dynamic_cast<ConcatLayerHyperParameterControl*> (control_p);
    this->rowSizeSpinBox->setValue(control->getRowSize());
    this->columnSizeSpinBox->setValue(control->getColumnSize());
    this->channelSpinBox->setValue(control->getChannelSize());
}
void ConcatLayerHyperParameterWidget::onRowSizeChanged(int i){
    dynamic_cast<ConcatLayerHyperParameterControl*>(this->hyperParameterControl)->setRowSize(i);
}
void ConcatLayerHyperParameterWidget::onColumnSizeChanged(int i){
    dynamic_cast<ConcatLayerHyperParameterControl*>(this->hyperParameterControl)->setColumnSize(i);
}
void ConcatLayerHyperParameterWidget::onChannelChanged(int i){
    dynamic_cast<ConcatLayerHyperParameterControl*>(this->hyperParameterControl)->setChannelSize(i);
}
