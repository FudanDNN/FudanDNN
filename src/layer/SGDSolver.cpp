#include "layer\SGDSolver.h"

SGDSolver::SGDSolver(double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate)
:Solver(regularizationRate,weightLearningRate, biasLearningRate, momentumRate){

}

void SGDSolver::update(vector<shared_ptr<Matrix>> weight, vector<shared_ptr<Matrix>> weightMomentum, vector<shared_ptr<Matrix>> weightGradient,
	vector<shared_ptr<Matrix>> bias, vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int num){
	for (int i = 0; i < num; i++)
	{
		weightMomentum[i]->multiple_inplace(momentumRate)->add_inplace(weightGradient[i]->multiple_inplace(1 - momentumRate));
		weight[i]->add_inplace(weightMomentum[i]->multiple(weightLearningRate));
		weightGradient[i]->initializeValue(0, 0);
		biasMomentum[i]->multiple_inplace(momentumRate)->add_inplace(biasGradient[i]->multiple_inplace(1 - momentumRate));
		bias[i]->add_inplace(biasMomentum[i]->multiple(biasLearningRate));
		biasGradient[i]->initializeValue(0, 0);
	}
}