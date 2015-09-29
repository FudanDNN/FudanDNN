#pragma once
#include <string>
using namespace std;

#pragma once
class Type{
public:
	static enum AutoEncoder
	{
		AE,//∆’Õ®autoencoder
		DAE,//denoise autoencoder
		CAE,//contractive autoencoder
		SAE//sparse autoencoder
	};
	static enum RBM
	{
		RBM,//∆’Õ®RBM
	};
	static enum CNN
	{
		CNN,//∆’Õ®cnn
	};
	static enum NONLINEAR
	{
		SIGMOID,
		HARDSHRINK,
		HARHTANH,
		LOGSSIGMOID,
		PRELU,
		RELU,
		SOFTSHRINK,
	};
};