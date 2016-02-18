#pragma once
#include "Solver.h"
class RMSPropSolver :
    public Solver
{
public:
    RMSPropSolver(double regularizationRate, double weightLearningRate, double biasLearningRate, double decayRate, double floatStableEpsilon) :
        Solver(regularizationRate, weightLearningRate, biasLearningRate, 1), floatStableEpsilon(floatStableEpsilon), decayRate(decayRate)
    {}
    ~RMSPropSolver();
    void update(vector<vector<shared_ptr<Matrix>>> convKernels, vector<vector<shared_ptr<Matrix>>> convKernelMomentum,
        vector<vector<shared_ptr<Matrix>>> convKernelsGradient, vector<shared_ptr<Matrix>> bias,
        vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int hiddenSize, int visualSize) override;
    void update(vector<shared_ptr<Matrix>> weight, vector<shared_ptr<Matrix>> weightMomentum, vector<shared_ptr<Matrix>> weightGradient,
        vector<shared_ptr<Matrix>> bias, vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int num) override;
private:
    double floatStableEpsilon;
    double decayRate;
};

