#pragma once
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