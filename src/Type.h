#ifndef __FUDANDNN_TYPE_H
#define __FUDANDNN_TYPE_H
#include "utils.h"

class Type{
public:
	enum AutoEncoder
	{
		// normal autoencoder
		AE,

		// denoise autoencoder
		DAE,

		// contractive autoencoder
		CAE,

		// sparse autoencoder
		SAE
	};
	enum RBM
	{
		RBM,
	};
	enum CNN
	{
		CNN,
	};
	enum NONLINEAR
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
#endif