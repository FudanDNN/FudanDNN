const enum SOLVER
{
	SGD,
    ADAGRAD
};
const enum NONLINEAR
{
	SIGMOID,
	HARDSHRINK,
	HARHTANH,
	LOGSSIGMOID,
	PRELU,
	RELU,
	SOFTSHRINK,
};
const enum initializationScheme
{
	RANDOM,			//[-1,1)
	RANDOM_SQRT,	//[-1/sqrt(visual), 1/sqrt(visual))
	RANDOM_NORM1	//[-1/visual, 1/visual)
};
const enum INPUT
{
	XMLINPUT,
};
const enum CRITERIA
{
	MSE,
	SOFTMAX,
};