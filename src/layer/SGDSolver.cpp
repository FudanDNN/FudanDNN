#include "layer\SGDSolver.h"

SGDSolver::SGDSolver(double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate)
:Solver(regularizationRate,weightLearningRate, biasLearningRate, momentumRate){

}

void SGDSolver::update(vector<vector<shared_ptr<Matrix>>> convKernels, vector<vector<shared_ptr<Matrix>>> convKernelMomentum,
	vector<vector<shared_ptr<Matrix>>> convKernelsGradient, vector<shared_ptr<Matrix>> bias,
	vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int num){

	for (int i = 0; i < num; i++)
	{
		for (size_t j = 0; j < num; j++)
		{
			convKernelMomentum[i][j]->muli(momentumRate);
			convKernelsGradient[i][j]->muli(weightLearningRate);
			convKernelMomentum[i][j]->addi(convKernelsGradient[i][j]);
			convKernels[i][j]->addi(convKernelMomentum[i][j]);
			convKernelsGradient[i][j]->setValues(0);
		}
		biasMomentum[i]->muli(momentumRate);
		biasGradient[i]->muli(biasLearningRate);
		biasMomentum[i]->addi(biasGradient[i]);

		bias[i]->addi(biasMomentum[i]);
		biasGradient[i]->setValues(0);
	}
	
}

void SGDSolver::update(shared_ptr<Matrix> weight, shared_ptr<Matrix> weightMomentum, shared_ptr<Matrix> weightGradient,
	shared_ptr<Matrix> bias, shared_ptr<Matrix> biasMomentum, shared_ptr<Matrix> biasGradient, int num){

	weightMomentum->muli(momentumRate);
	weightMomentum->addi(weightGradient);
	weightMomentum->muli(weightLearningRate);
	weight->addi(weightMomentum);
	biasMomentum->muli(momentumRate);
	biasMomentum->addi(biasGradient);
	biasMomentum->muli(biasLearningRate);
	bias->addi(biasMomentum);

	weightGradient->setValues(0);
	biasGradient->setValues(0);

}