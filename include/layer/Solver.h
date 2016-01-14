#ifndef __FUDANDNN_SOLVER_H
#define __FUDANDNN_SOLVER_H
#include "util/Matrix.h"

class Solver{
public:
	Solver(double regularizationRate, double weightLearningRate, double biasLearningRate, double momentumRate);
	virtual void update(vector<shared_ptr<Matrix>> weight, vector<shared_ptr<Matrix>> weightMomentum, vector<shared_ptr<Matrix>> weightGradient,
		vector<shared_ptr<Matrix>> bias, vector<shared_ptr<Matrix>> biasMomentum, vector<shared_ptr<Matrix>> biasGradient, int num){};
	void setRegularizationRate(double regularizationRate);
	void setWeightLearningRate(double weightLearningRate);
	void setBiasLearningRate(double biasLearningRate);
	void setMomentumRate(double momentumRate);
protected:
	double regularizationRate = 0;
	double weightLearningRate = 0.05;
	double biasLearningRate = 0.05;
	double momentumRate = 0.9;
};

#endif