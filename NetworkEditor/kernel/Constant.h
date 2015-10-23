#ifndef CONSTANT_H
#define CONSTANT_H
static enum NetworkError
{
	NOINPUT,
	NOCRITERIA,
	WRONGHIDDENNUM,
	WRONGVISUALNUM,
	CORRECT
};
static enum
{
	SIGMOID,
	RELU,
	HARDSHRINK,
	HARHTANH,
	LOGSSIGMOID,
	PRELU,
	SOFTSHRINK,
};
static enum XMLError
{
	RIGHT,
};

static enum WorkingThreadJobType{
    TRAIN_JOB,
    PREDICT_JOB
};

#endif