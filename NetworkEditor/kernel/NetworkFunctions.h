#ifndef __FUDANDNN_NETWORKFUNCTIONS_H
#define __FUDANDNN_NETWORKFUNCTIONS_H
#include "utils.h"


//sigmoid
double sigmoid(double);
double sigmoidDerivative(double);
//LogSigmoid
double logsigmoid(double);
double logsigmoidDerivative(double);
//HardTanH
double hardTanH(double);
double hardTanHDerivative(double);
//HardShrink
double hardShrink(double);
double hardShrinkDerivative(double);
//SoftShrink
double softShrink(double, double);
double softShrinkDerivative(double, double);
//ReLU
double reLU(double);
double reLUDerivative(double);
//PReLU
double pReLU(double);
double pReLUDerivative(double);
//random
double random(double);
double max(double a, double b);
double min(double a, double b);
#endif