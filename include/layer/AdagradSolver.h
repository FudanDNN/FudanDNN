#ifndef __FUDANDNN_ADAGRAD_SOLVER_H
#define __FUDANDNN_ADAGRAD_SOLVER_H
#include "Solver.h"
class AdagradSolver :
    public Solver
{
public:
    /*
        Used the momentum array as gradient caches.
    */
    AdagradSolver(double regularizationRate, double weightLearningRate, double biasLearningRate, double floatStableEpsilon) :
        Solver(regularizationRate, weightLearningRate, biasLearningRate, 1), floatStableEpsilon(floatStableEpsilon)
    {}
    ~AdagradSolver();
    void update(vector<vector<shared_ptr<Matrix>>> convKernels, vector<vector<shared_ptr<Matrix>>> convKernelMomentum,
        vector<vector<shared_ptr<Matrix>>> convKernelsGradient, vector<shared_ptr<Matrix>> bias,
        vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int hiddenSize, int visualSize) override;
    void update(vector<shared_ptr<Matrix>> weight, vector<shared_ptr<Matrix>> weightMomentum, vector<shared_ptr<Matrix>> weightGradient,
        vector<shared_ptr<Matrix>> bias, vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int num) override;
private:
    double floatStableEpsilon;
};

#endif
