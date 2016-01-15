#ifndef __FUDANDNN_INPUTCELL_H
#define __FUDANDNN_INPUTCELL_H
#include "util\utils.h"
#include "util\Matrix.h"

class InputCell{
private:
	shared_ptr<Matrix> inputMat;
	shared_ptr<Matrix> gradientMat;
	size_t id;
public:
	InputCell(size_t id);
	size_t getId();
	void setInputMat(shared_ptr<Matrix> inputMat);
	void setGradientMat(shared_ptr<Matrix> gradientMat);
	shared_ptr<Matrix> getInputMat();
	shared_ptr<Matrix> getGradientMat();
};
#endif