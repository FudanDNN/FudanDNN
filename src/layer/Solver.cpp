#include "layer\Solver.h"

Solver::Solver(double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate){
	this->regularizationRate = regularizationRate;
	this->weightLearningRate = weightLearningRate;
	this->biasLearningRate = biasLearningRate;
	this->momentumRate = momentumRate;
}

void Solver::setRegularizationRate(double regularizationRate){
	this->regularizationRate = regularizationRate;
}

void Solver::setWeightLearningRate(double weightLearningRate){
	this->weightLearningRate = weightLearningRate;
}

void Solver::setBiasLearningRate(double biasLearningRate){
	this->biasLearningRate = biasLearningRate;
}

void Solver::setMomentumRate(double momentumRate){
	this->momentumRate = momentumRate;
}