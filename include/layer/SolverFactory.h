#ifndef __FUDANDNN_SOLVERFACTORY_H
#define __FUDANDNN_SOLVERFACTORY_H
#include "util/utils.h"
#include "Solver.h"

class SolverFactory{

public:
	shared_ptr<Solver> createSolver(int type, double regularizationRate, 
		double weightLearningRate, double biasLearningRate, double momentumRate);
    shared_ptr<Solver> createSolver(int type, double regularizationRate,
        double weightLearningRate, double biasLearningRate, double momentumRate, double floatStableEpsilon );
};

#endif