#include "layer\SolverFactory.h"
#include "layer\SGDSolver.h"
#include "layer\AdagradSolver.h"
#include "layer\RMSPropSolver.h"
shared_ptr<Solver> SolverFactory::createSolver(int type, double regularizationRate,
	double weightLearningRate, double biasLearningRate, double momentumRate){
	shared_ptr<Solver> solver;
	switch (type){
	case SOLVER::SGD:
		solver = shared_ptr<Solver>(new SGDSolver(regularizationRate,
			weightLearningRate, biasLearningRate, momentumRate));
		break;
    
	default:
		break;
	}
	return solver;
}
shared_ptr<Solver> SolverFactory::createSolver(int type, double regularizationRate,
    double weightLearningRate, double biasLearningRate, double momentumRate, double floatStableEpsilon){
    shared_ptr<Solver> solver;
    switch (type){
    case SOLVER::RMSPROP:
        solver = shared_ptr<Solver>(new RMSPropSolver(regularizationRate, weightLearningRate, biasLearningRate, momentumRate, floatStableEpsilon));
        break;
	case SOLVER::ADAGRAD:
        solver = shared_ptr<Solver>(new AdagradSolver(regularizationRate, weightLearningRate, biasLearningRate, floatStableEpsilon)) ;
        break;
    default:
        break;
    }
    return solver;
}