#include "layer\SGDSolver.h"

SGDSolver::SGDSolver(double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate)
:Solver(regularizationRate, weightLearningRate, biasLearningRate, momentumRate){

}

void SGDSolver::update(vector<vector<shared_ptr<Matrix>>> convKernels, vector<vector<shared_ptr<Matrix>>> convKernelMomentum,
	vector<vector<shared_ptr<Matrix>>> convKernelsGradient, vector<shared_ptr<Matrix>> bias,
	vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int hiddenSize, int visualSize) {

	for (int i = 0; i < hiddenSize; i++)
	{
		for (size_t j = 0; j < visualSize; j++)
		{
			convKernelMomentum[i][j]->muli(momentumRate);
			convKernelMomentum[i][j]->addi(convKernelsGradient[i][j]);
			convKernelMomentum[i][j]->subi(convKernels[i][j]->mul(regularizationRate));
			convKernels[i][j]->addi(convKernelMomentum[i][j]->mul(weightLearningRate));
			convKernelsGradient[i][j]->setValues(0);
		}
		biasMomentum[i]->muli(momentumRate);
		biasMomentum[i]->addi(biasGradient[i]);
		biasMomentum[i]->subi(bias[i]->mul(regularizationRate));
		bias[i]->addi(biasMomentum[i]->mul(biasLearningRate));
		biasGradient[i]->setValues(0);
	}

}

void SGDSolver::update(vector<shared_ptr<Matrix>> weight, vector<shared_ptr<Matrix>> weightMomentum, vector<shared_ptr<Matrix>> weightGradient,
	vector<shared_ptr<Matrix>> bias, vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int num) {

	for (size_t i = 0; i < num; i++)
	{
		weightMomentum[i]->muli(momentumRate);
		weightMomentum[i]->addi(weightGradient[i]);
		weightMomentum[i]->subi(weight[i]->mul(regularizationRate));
		weight[i]->addi(weightMomentum[i]->mul(weightLearningRate));
		weightGradient[i]->setValues(0);

		biasMomentum[i]->muli(momentumRate);
		biasMomentum[i]->addi(biasGradient[i]);
		biasMomentum[i]->subi(bias[i]->mul(regularizationRate));
		bias[i]->addi(biasMomentum[i]->mul(biasLearningRate));
		biasGradient[i]->setValues(0);
	}

}