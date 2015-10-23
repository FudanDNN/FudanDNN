#include "hyperparameterwidget.h"
#include <qfiledialog.h>
#include <qpushbutton.h>

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
LinearLayerHyperParameterWidget::LinearLayerHyperParameterWidget(LinearLayerHyperParameterControl* control, QWidget *parent)
	:HyperParameterWidget(control,parent)
{
	this->visualUnitCountSpinBox = new QSpinBox(this);
    this->visualUnitCountSpinBox->setMaximum(999999);
	this->hiddenUnitCountSpinBox = new QSpinBox(this);
    this->hiddenUnitCountSpinBox->setMaximum(999999);
	this->initSchemeComboBox = new QComboBox(this);
    this->initSchemeComboBox->addItem("RANDOM", AbstractNetworkLayer::RANDOM);
    this->initSchemeComboBox->addItem("RANDOM_SQRT", AbstractNetworkLayer::RANDOM_SQRT);
    this->initSchemeComboBox->addItem("RANDOM_NORM1", AbstractNetworkLayer::RANDOM_NORM1);
    this->regularizationRateSpinBox = new QDoubleSpinBox(this);
    this->regularizationRateSpinBox->setSingleStep(0.01);
    this->weightLearningRateSpinBox = new QDoubleSpinBox(this);
    this->weightLearningRateSpinBox->setSingleStep(0.01);
    this->biasLearningRateSpinBox = new QDoubleSpinBox(this);
    this->biasLearningRateSpinBox->setSingleStep(0.01);
    this->momentumRateSpinBox = new QDoubleSpinBox(this);
    this->momentumRateSpinBox->setSingleStep(0.01);

    parseParameterControl(control);
	connect(this->visualUnitCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onVisualUnitCountChanged(int)));
    connect(this->hiddenUnitCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onHiddenUnitCountChanged(int)));
    connect(this->initSchemeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInitSchemeChanged(int)));
    connect(this->regularizationRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onRegularizationRateChanged(double)));
    connect(this->weightLearningRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onWeightLearningRateChanged(double)));
    connect(this->biasLearningRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onBiasLearningRateChanged(double)));
    connect(this->momentumRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onMomentumRateChanged(double)));
    QFormLayout *formLayout = new QFormLayout(this);
	formLayout->addRow("Visual Unit Count:", visualUnitCountSpinBox);
	formLayout->addRow("Hidden Unit Count:", hiddenUnitCountSpinBox);
	formLayout->addRow("Init Scheme:", initSchemeComboBox);
    formLayout->addRow("Regularization Rate:", regularizationRateSpinBox);
    formLayout->addRow("Weight Learning Rate:", weightLearningRateSpinBox);
    formLayout->addRow("Bias Learning Rate:", biasLearningRateSpinBox);
    formLayout->addRow("Momentum Rate:",momentumRateSpinBox);
	setLayout(formLayout);
}
void LinearLayerHyperParameterWidget::parseParameterControl(HyperParameterControl * control){
	LinearLayerHyperParameterControl *linearControl = dynamic_cast<LinearLayerHyperParameterControl*> (control);
	this->visualUnitCountSpinBox->setValue(linearControl->getVisualUnitCount());
	this->hiddenUnitCountSpinBox->setValue(linearControl->getHiddenUnitCount());
    this->initSchemeComboBox->setCurrentIndex(linearControl->getInitScheme());
    this->regularizationRateSpinBox->setValue(linearControl->getRegularizationRate());
    this->weightLearningRateSpinBox->setValue(linearControl->getWeightLearningRate());
    this->biasLearningRateSpinBox->setValue(linearControl->getBiasLearningRate());
    this->momentumRateSpinBox->setValue(linearControl->getMomentumRate());
}

/*NonLinearLayerHyperParameterWidget*/
NonLinearLayerHyperParameterWidget::NonLinearLayerHyperParameterWidget(NonLinearLayerHyperParameterControl* control, QWidget *parent)
    :HyperParameterWidget(control, parent)
{
    this->sizeSpinBox = new QSpinBox(this);
    this->sizeSpinBox->setMaximum(999999);
    this->typeComboBox = new QComboBox(this);
    this->typeComboBox->addItem("Sigmoid", NonLinearFactory::SIGMOID);
    this->typeComboBox->addItem("ReLU", NonLinearFactory::RELU);
    this->typeComboBox->addItem("HardShrink", NonLinearFactory::HARDSHRINK);
    this->typeComboBox->addItem("HarhTanh", NonLinearFactory::HARHTANH);
    this->typeComboBox->addItem("LogSigmoid", NonLinearFactory::LOGSSIGMOID);
    this->typeComboBox->addItem("PReLU", NonLinearFactory::PRELU);
    this->typeComboBox->addItem("SoftShrink", NonLinearFactory::SOFTSHRINK);
    
    parseParameterControl(control);
    connect(this->sizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSizeChanged(int)));
    connect(this->typeComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(onTypeChanged(int)));
    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow("Size:", sizeSpinBox);
    formLayout->addRow("Type:", typeComboBox);
    setLayout(formLayout);
}
void NonLinearLayerHyperParameterWidget::parseParameterControl(HyperParameterControl * control){
    NonLinearLayerHyperParameterControl *c = dynamic_cast<NonLinearLayerHyperParameterControl*> (control);
    this->sizeSpinBox->setValue(c->getSize());
    this->typeComboBox->setCurrentIndex(c->getType());
}

/* CNNSubnetHyperParameterWidget */
CNNSubnetHyperParameterWidget::CNNSubnetHyperParameterWidget(CNNSubnetHyperParameterControl* control, QWidget *parent)
    :HyperParameterWidget(control,parent)
{
    this->cnnIdLineEdit = new QLineEdit();
    this->cnnIdLineEdit->setReadOnly(true);
    parseParameterControl(control);
   QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow("CNN Subnet ID:", cnnIdLineEdit);
    setLayout(formLayout);
}
void CNNSubnetHyperParameterWidget::parseParameterControl(HyperParameterControl *control){
    CNNSubnetHyperParameterControl* cnnControl = dynamic_cast<CNNSubnetHyperParameterControl*> (control);
    QString s;
    s.sprintf("%d", cnnControl->getCnnId());
    cnnIdLineEdit->setText(s);
}

CNNConvolutionLayerHyperParameterWidget::CNNConvolutionLayerHyperParameterWidget(CNNConvolutionLayerHyperParameterControl* control, QWidget *parent)
    :HyperParameterWidget(control, parent)
{
    this->kernelSizeSpinBox = new QSpinBox(this);
    this->kernelSizeSpinBox->setMaximum(999999);
    this->strideSpinBox = new QSpinBox(this);
    this->strideSpinBox->setMaximum(999999);
    this->featureMapNumSpinBox = new QSpinBox(this);
    this->featureMapNumSpinBox->setMaximum(999999);
    this->numSpinBox = new QSpinBox(this);
    this->numSpinBox->setMaximum(999999);
    this->visualRowSpinBox = new QSpinBox(this);
    this->visualRowSpinBox->setMaximum(999999);
    this->visualColumnSpinBox = new QSpinBox(this);
    this->visualColumnSpinBox->setMaximum(999999);
    this->initSchemeComboBox = new QComboBox(this);
    this->initSchemeComboBox->addItem("RANDOM", AbstractNetworkLayer::RANDOM);
    this->initSchemeComboBox->addItem("RANDOM_SQRT", AbstractNetworkLayer::RANDOM_SQRT);
    this->initSchemeComboBox->addItem("RANDOM_NORM1", AbstractNetworkLayer::RANDOM_NORM1);
    this->regularizationRateSpinBox = new QDoubleSpinBox(this);
    this->regularizationRateSpinBox->setSingleStep(0.0001);
    this->regularizationRateSpinBox->setDecimals(5);
    this->kernelLearningRateSpinBox = new QDoubleSpinBox(this);
    this->kernelLearningRateSpinBox->setSingleStep(0.01);
    this->biasLearningRateSpinBox = new QDoubleSpinBox(this);
    this->biasLearningRateSpinBox->setSingleStep(0.01);
    this->momentumRateSpinBox = new QDoubleSpinBox(this);
    this->momentumRateSpinBox->setSingleStep(0.01);
    this->cnnIdLineEdit = new QLineEdit(this);
    parseParameterControl(control);

    cnnIdLineEdit->setReadOnly(true);
    connect(this->kernelSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onKernelSizeChanged(int)));
    connect(this->strideSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onStrideChanged(int)));
    connect(this->featureMapNumSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFeatureMapNumChanged(int)));
    connect(this->numSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onNumChanged(int)));
    connect(this->visualRowSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onVisualRowChanged(int)));
    connect(this->visualColumnSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onVisualColumnChanged(int)));
    connect(this->initSchemeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInitSchemeChanged(int)));
    connect(this->regularizationRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onRegularizationRateChanged(double)));
    connect(this->kernelLearningRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onKernelLearningRate(double)));
    connect(this->biasLearningRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onBiasLearningRateChanged(double)));
    connect(this->momentumRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onMomentumRateChanged(double)));
    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow("Kernel Size:", kernelSizeSpinBox);
    formLayout->addRow("Stride:", strideSpinBox);
    formLayout->addRow("Feature Map Num:", featureMapNumSpinBox);
    formLayout->addRow("Num:", numSpinBox);
    formLayout->addRow("Visual Row:", visualRowSpinBox);
    formLayout->addRow("Visual Column:", visualColumnSpinBox);
    formLayout->addRow("Init Scheme:", initSchemeComboBox);
    formLayout->addRow("Regularization Rate:", regularizationRateSpinBox);
    formLayout->addRow("Kernel Learning Rate:", kernelLearningRateSpinBox);
    formLayout->addRow("Bias Learning Rate:", biasLearningRateSpinBox);
    formLayout->addRow("Momentum Rate:", momentumRateSpinBox);
    formLayout->addRow("CNN Subnet ID:", cnnIdLineEdit);
    setLayout(formLayout);
}
void CNNConvolutionLayerHyperParameterWidget::parseParameterControl(HyperParameterControl *c) {
    CNNConvolutionLayerHyperParameterControl *control = dynamic_cast<CNNConvolutionLayerHyperParameterControl*>( c);
    kernelSizeSpinBox->setValue(control->getKernelSize());
    strideSpinBox->setValue(control->getStride());
    featureMapNumSpinBox->setValue(control->getFeatureMapNum());
    numSpinBox->setValue(control->getNum());
    visualRowSpinBox->setValue(control->getVisualRow());
    visualColumnSpinBox->setValue(control->getVisualColumn());
    initSchemeComboBox->setCurrentIndex((int)control->getInitScheme());
    this->regularizationRateSpinBox->setValue(control->getRegularizationRate());
    this->kernelLearningRateSpinBox->setValue(control->getKernelLearningRate());
    this->biasLearningRateSpinBox->setValue(control->getBiasLearningRate());
    this->momentumRateSpinBox->setValue(control->getMomentumRate());
    QString cnnIdString; cnnIdString.sprintf("%d", control->getCnnId());
    cnnIdLineEdit->setText(cnnIdString);
}

void CNNConvolutionLayerHyperParameterWidget::onKernelSizeChanged(int s){
    dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(this->getHyperParameterControl())->setKernelSize((size_t)s);
}
void CNNConvolutionLayerHyperParameterWidget::onStrideChanged(int s){
    dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(this->getHyperParameterControl())->setStride((size_t)s);
}
void CNNConvolutionLayerHyperParameterWidget::onFeatureMapNumChanged(int s){
    dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(this->getHyperParameterControl())->setFeatureMapNum((size_t)s);
}
void CNNConvolutionLayerHyperParameterWidget::onNumChanged(int s){
    dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(this->getHyperParameterControl())->setNum((size_t)s);
}
void CNNConvolutionLayerHyperParameterWidget::onVisualRowChanged(int s){
    dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(this->getHyperParameterControl())->setVisualRow((size_t)s);
}
void CNNConvolutionLayerHyperParameterWidget::onVisualColumnChanged(int s){
    dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(this->getHyperParameterControl())->setVisualColumn((size_t)s);
}

void CNNConvolutionLayerHyperParameterWidget::onInitSchemeChanged(int c){
    dynamic_cast<CNNConvolutionLayerHyperParameterControl*>(this->getHyperParameterControl())->setInitScheme((AbstractNetworkLayer::initializationScheme)c);
}

/* CNN MaxPooling Layer*/
CNNMaxPoolingLayerHyperParameterWidget::CNNMaxPoolingLayerHyperParameterWidget(CNNMaxPoolingLayerHyperParameterControl* control, QWidget *parent)
    :HyperParameterWidget(control, parent)
{
    this->poolingSizeSpinBox = new QSpinBox(this);
    this->poolingSizeSpinBox->setMaximum(999999);
    this->strideSpinBox = new QSpinBox(this);
    this->strideSpinBox->setMaximum(999999);
    this->numSpinBox = new QSpinBox(this);
    this->numSpinBox->setMaximum(999999);
    this->visualRowSpinBox = new QSpinBox(this);
    this->visualRowSpinBox->setMaximum(999999);
    this->visualColumnSpinBox = new QSpinBox(this);
    this->visualColumnSpinBox->setMaximum(999999);
    this->cnnIdLineEdit = new QLineEdit(this);
    parseParameterControl(control);

    cnnIdLineEdit->setReadOnly(true);
    connect(this->poolingSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onPoolingSizeChanged(int)));
    connect(this->strideSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onStrideChanged(int)));
    connect(this->numSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onNumChanged(int)));
    connect(this->visualRowSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onVisualRowChanged(int)));
    connect(this->visualColumnSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onVisualColumnChanged(int)));
    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow("Pooling Size:", poolingSizeSpinBox);
    formLayout->addRow("Stride:", strideSpinBox);
    formLayout->addRow("Num:", numSpinBox);
    formLayout->addRow("Visual Row:", visualRowSpinBox);
    formLayout->addRow("Visual Column:", visualColumnSpinBox);
    formLayout->addRow("CNN Subnet ID:", cnnIdLineEdit);
    setLayout(formLayout);
}
void CNNMaxPoolingLayerHyperParameterWidget::parseParameterControl(HyperParameterControl *c) {
    CNNMaxPoolingLayerHyperParameterControl *control = dynamic_cast<CNNMaxPoolingLayerHyperParameterControl*>(c);
    poolingSizeSpinBox->setValue(control->getPoolingSize());
    strideSpinBox->setValue(control->getStride());
    numSpinBox->setValue(control->getNum());
    visualRowSpinBox->setValue(control->getVisualRow());
    visualColumnSpinBox->setValue(control->getVisualColumn());
    
    QString cnnIdString; cnnIdString.sprintf("%d", control->getCnnId());
    cnnIdLineEdit->setText(cnnIdString);
}

void CNNMaxPoolingLayerHyperParameterWidget::onPoolingSizeChanged(int s){
    dynamic_cast<CNNMaxPoolingLayerHyperParameterControl*>(this->getHyperParameterControl())->setPoolingSize((size_t)s);
}
void CNNMaxPoolingLayerHyperParameterWidget::onStrideChanged(int s){
    dynamic_cast<CNNMaxPoolingLayerHyperParameterControl*>(this->getHyperParameterControl())->setStride((size_t)s);
}
void CNNMaxPoolingLayerHyperParameterWidget::onNumChanged(int s){
    dynamic_cast<CNNMaxPoolingLayerHyperParameterControl*>(this->getHyperParameterControl())->setNum((size_t)s);
}
void CNNMaxPoolingLayerHyperParameterWidget::onVisualRowChanged(int s){
    dynamic_cast<CNNMaxPoolingLayerHyperParameterControl*>(this->getHyperParameterControl())->setVisualRow((size_t)s);
}
void CNNMaxPoolingLayerHyperParameterWidget::onVisualColumnChanged(int s){
    dynamic_cast<CNNMaxPoolingLayerHyperParameterControl*>(this->getHyperParameterControl())->setVisualColumn((size_t)s);
}

/* CNN NonLinear Layer */
CNNNonLinearLayerHyperParameterWidget::CNNNonLinearLayerHyperParameterWidget(CNNNonLinearLayerHyperParameterControl* control, QWidget *parent)
    :HyperParameterWidget(control, parent)
{
    this->numSpinBox = new QSpinBox(this);
    this->numSpinBox->setMaximum(999999);
    this->visualRowSpinBox = new QSpinBox(this);
    this->visualRowSpinBox->setMaximum(999999);
    this->visualColumnSpinBox = new QSpinBox(this);
    this->visualColumnSpinBox->setMaximum(999999);
    this->typeComboBox = new QComboBox(this);
    this->typeComboBox->addItem("Sigmoid", NonLinearFactory::SIGMOID);
    this->typeComboBox->addItem("ReLU", NonLinearFactory::RELU);
    this->typeComboBox->addItem("HardShrink", NonLinearFactory::HARDSHRINK);
    this->typeComboBox->addItem("HarhTanh", NonLinearFactory::HARHTANH);
    this->typeComboBox->addItem("LogSigmoid", NonLinearFactory::LOGSSIGMOID);
    this->typeComboBox->addItem("PReLU", NonLinearFactory::PRELU);
    this->typeComboBox->addItem("SoftShrink", NonLinearFactory::SOFTSHRINK);
    this->cnnIdLineEdit = new QLineEdit(this);
    cnnIdLineEdit->setReadOnly(true);
    parseParameterControl(control);

    connect(this->numSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onNumChanged(int)));
    connect(this->visualRowSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onVisualRowChanged(int)));
    connect(this->visualColumnSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onVisualColumnChanged(int)));
    connect(this->typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChanged(int)));
    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow("Num:", numSpinBox);
    formLayout->addRow("Visual Row:", visualRowSpinBox);
    formLayout->addRow("Visual Column:", visualColumnSpinBox);
    formLayout->addRow("Type:", typeComboBox);
    formLayout->addRow("CNN Subnet ID:", cnnIdLineEdit);
    setLayout(formLayout);
}
void CNNNonLinearLayerHyperParameterWidget::parseParameterControl(HyperParameterControl *c) {
    CNNNonLinearLayerHyperParameterControl *control = dynamic_cast<CNNNonLinearLayerHyperParameterControl*>(c);
    numSpinBox->setValue(control->getNum());
    visualRowSpinBox->setValue(control->getVisualRow());
    visualColumnSpinBox->setValue(control->getVisualColumn());
    typeComboBox->setCurrentIndex((int)(control->getType()));
    QString cnnIdString; cnnIdString.sprintf("%d", control->getCnnId());
    cnnIdLineEdit->setText(cnnIdString);
}

void CNNNonLinearLayerHyperParameterWidget::onNumChanged(int s){
    dynamic_cast<CNNNonLinearLayerHyperParameterControl*>(this->getHyperParameterControl())->setNum((size_t)s);
}
void CNNNonLinearLayerHyperParameterWidget::onVisualRowChanged(int s){
    dynamic_cast<CNNNonLinearLayerHyperParameterControl*>(this->getHyperParameterControl())->setVisualRow((size_t)s);
}
void CNNNonLinearLayerHyperParameterWidget::onVisualColumnChanged(int s){
    dynamic_cast<CNNNonLinearLayerHyperParameterControl*>(this->getHyperParameterControl())->setVisualColumn((size_t)s);
}
void CNNNonLinearLayerHyperParameterWidget::onTypeChanged(int s){
    dynamic_cast<CNNNonLinearLayerHyperParameterControl*>(this->getHyperParameterControl())->setType((NonLinearFactory::NONLINEAR)s);
}

CNNIOHyperParameterWidget::CNNIOHyperParameterWidget(CNNIOHyperParameterControl* control, QWidget *parent) :
    HyperParameterWidget(control, parent){
    this->cnnIdLineEdit = new QLineEdit();
    this->cnnIdLineEdit->setReadOnly(true);
    parseParameterControl(control);
    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow("CNN Subnet ID:", cnnIdLineEdit);
    setLayout(formLayout);
}

void CNNIOHyperParameterWidget::parseParameterControl(HyperParameterControl* c){
    CNNIOHyperParameterControl *control = dynamic_cast<CNNIOHyperParameterControl*>(c);
    QString s; s.sprintf("%d",control->getCnnId());
    cnnIdLineEdit->setText(s);
}