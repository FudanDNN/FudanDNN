#include "layer\SGDSolver.h"

SGDSolver::SGDSolver(double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate)
:Solver(regularizationRate,weightLearningRate, biasLearningRate, momentumRate){

}

void SGDSolver::update(vector<shared_ptr<Matrix>> weight, vector<shared_ptr<Matrix>> weightMomentum, vector<shared_ptr<Matrix>> weightGradient,
	vector<shared_ptr<Matrix>> bias, vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int num){
	
}