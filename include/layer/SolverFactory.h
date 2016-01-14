#ifndef __FUDANDNN_NONLINEARFACTORY_H
#define __FUDANDNN_NONLINEARFACTORY_H
#include "util/utils.h"
#include "Solver.h"

class SolverFactory{

public:
	shared_ptr<Solver> createSolver(int type, double regularizationRate, 
		double weightLearningRate, double biasLearningRate, double momentumRate);
};

#endif