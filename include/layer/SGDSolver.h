#ifndef __FUDANDNN_SGDSOLVER_H
#define __FUDANDNN_SGDSOLVER_H
#include "Solver.h"

class SGDSolver : public Solver{
public :
	SGDSolver(double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate);
	void update(vector<vector<shared_ptr<Matrix>>> convKernels, vector<vector<shared_ptr<Matrix>>> convKernelMomentum,
		vector<vector<shared_ptr<Matrix>>> convKernelsGradient, vector<shared_ptr<Matrix>> bias,
		vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int num);
	void update(shared_ptr<Matrix> weight, shared_ptr<Matrix> weightMomentum, shared_ptr<Matrix> weightGradient,
		shared_ptr<Matrix> bias, shared_ptr<Matrix> biasMomentum, shared_ptr<Matrix> biasGradient, int num);
};

#endif