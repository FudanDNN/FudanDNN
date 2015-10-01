cmake_minimum_required(VERSION 2.8)

project(FDUDNN_MASTER)

set (SRC_LIST 
	AbstractComponent.cpp
	AbstractLayer.cpp
	AbstractMatrix.cpp
	AbstractNetworkLayer.cpp
	AutoEncoder.cpp
	CNN1DComponent.cpp
	CNN2DComponent.cpp
	CharacterInput.cpp
	CharacterSample.cpp
	ComponentNode.cpp
	ContractiveAutoEncoder.cpp
	ConvolutionalNetworkLayer.cpp
	Criteria.cpp
	DenoiseAutoEncoder.cpp
	HardShrink.cpp
	HardTanH.cpp
	Input.cpp
	LinearLayer.cpp
	LogSigmoid.cpp
	MSE.cpp
	MasterControl.cpp
	Matrix.cpp
	MaxPoolingLayer.cpp
	NetworkFunction.cpp
	NetworkNode.cpp
	NonLinearComponent.cpp
	NonLinearFactory.cpp
	NonLinearLayer.cpp
	PReLU.cpp
	RBM.cpp
	RBMDataGenerator.cpp
	ReLU.cpp
	Sample.cpp
	Sigmoid.cpp
	SoftShrink.cpp
	SparseAutoEncoder.cpp
	XMLDataParser.cpp
	tinystr.cpp
	tinyxml.cpp
	tinyxmlerror.cpp
	tinyxmlparser.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(fdudnn_master ${SRC_LIST})