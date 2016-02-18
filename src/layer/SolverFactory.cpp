#include "layer\SolverFactory.h"
#include "layer\SGDSolver.h"
#include "layer\AdagradSolver.h"
shared_ptr<Solver> SolverFactory::createSolver(int type, double regularizationRate,
	double weightLearningRate, double biasLearningRate, double momentumRate){
	shared_ptr<Solver> solver;
	switch (type){
	case SOLVER::SGD:
		solver = shared_ptr<Solver>(new SGDSolver(regularizationRate,
			weightLearningRate, biasLearningRate, momentumRate));
		break;
    case SOLVER::ADAGRAD:
        solver = shared_ptr<Solver>(new AdagradSolver(regularizationRate, weightLearningRate, biasLearningRate, momentumRate)) ;
        break;
	default:
		break;
	}
	return solver;
}