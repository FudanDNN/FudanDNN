#include "layer\AdagradSolver.h"

AdagradSolver::~AdagradSolver()
{
}
void AdagradSolver::update(vector<shared_ptr<Matrix>> weight, vector<shared_ptr<Matrix>> weightMomentum, vector<shared_ptr<Matrix>> weightGradient,
    vector<shared_ptr<Matrix>> bias, vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int num) {
    const auto sqrFunc = [](double x)-> double { return x*x; };
    const auto divide1 = [](double x)-> double { return 1 / x; };
    for (int i = 0; i < num; ++i){
        shared_ptr<Matrix> gradSqr = weightGradient[i]->map(sqrFunc);
        weightMomentum[i]->addi(gradSqr);
        shared_ptr<Matrix> delta = weightGradient[i]->mul(weightLearningRate);
        shared_ptr<Matrix> dividor = weightMomentum[i]->map(sqrt);
        dividor->addi(floatStableEpsilon);
        dividor->mapi(divide1);
        delta->mulewi(dividor);
        weight[i]->subi(delta);
        weightGradient[i]->setValues(0);
    }
    for (int i = 0; i < num; ++i){
        shared_ptr<Matrix> gradSqr = bias[i]->map(sqrFunc);
        biasMomentum[i]->addi(gradSqr);
        shared_ptr<Matrix> delta = biasGradient[i]->mul(biasLearningRate);
        shared_ptr<Matrix> dividor = biasMomentum[i]->map(sqrt);
        dividor->addi(floatStableEpsilon);
        dividor->mapi(divide1);
        delta->mulewi(dividor);
        bias[i]->subi(delta);
        biasGradient[i]->setValues(0);
    }
}

void AdagradSolver::update(vector<vector<shared_ptr<Matrix>>> convKernels, vector<vector<shared_ptr<Matrix>>> convKernelMomentum,
    vector<vector<shared_ptr<Matrix>>> convKernelsGradient, vector<shared_ptr<Matrix>> bias,
    vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int hiddenSize, int visualSize)
{
    const auto sqrFunc = [](double x)-> double { return x*x; };
    const auto divide1 = [](double x)-> double { return 1 / x; };
    for (int i = 0; i < hiddenSize; ++i){
        for (int j = 0; j < visualSize; ++j){
            shared_ptr<Matrix> gradSqr = convKernelsGradient[i][j]->map(sqrFunc);
            convKernelMomentum[i][j]->addi(gradSqr);
            shared_ptr<Matrix> delta = convKernelsGradient[i][j]->mul(weightLearningRate);
            shared_ptr<Matrix> dividor = convKernelMomentum[i][j]->map(sqrt);
            dividor->addi(floatStableEpsilon);
            dividor->mapi(divide1);
            delta->mulewi(dividor);
            convKernels[i][j]->subi(delta);
            convKernelsGradient[i][j]->setValues(0);
        }
        shared_ptr<Matrix> gradSqr = bias[i]->map(sqrFunc);
        biasMomentum[i]->addi(gradSqr);
        shared_ptr<Matrix> delta = biasGradient[i]->mul(biasLearningRate);
        shared_ptr<Matrix> dividor = biasMomentum[i]->map(sqrt);
        dividor->addi(floatStableEpsilon);
        dividor->mapi(divide1);
        delta->mulewi(dividor);
        bias[i]->subi(delta);
        biasGradient[i]->setValues(0);
    }
}