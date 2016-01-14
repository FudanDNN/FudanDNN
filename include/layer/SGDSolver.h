#ifndef __FUDANDNN_SGDSOLVER_H
#define __FUDANDNN_SGDSOLVER_H
#include "Solver.h"

class SGDSolver : public Solver{
public :
	SGDSolver(double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate);
	void update(vector<shared_ptr<Matrix>> weight, vector<shared_ptr<Matrix>> weightMomentum, vector<shared_ptr<Matrix>> weightGradient,
		vector<shared_ptr<Matrix>> bias, vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int num);
};

#endif