#ifndef __FUDANDNN_CHARACTERINPUT_H
#define __FUDANDNN_CHARACTERINPUT_H
#include "utils.h"
#include "CharacterSample.h"
#include "AbstractMatrix.h"
#include "XMLDataParser.h"
#include "Input.h"

class CharacterInput : public Input
{
public:
	CharacterInput(string xmlFile, int id, int dimension);
	shared_ptr<TrainingSample> getNextTrainingSample() override;
	~CharacterInput();

private:
	map<string, vector<double>> lookupTable;
	map<string, shared_ptr<AbstractMatrix>> tagTable;
	size_t dimension;
};


#endif