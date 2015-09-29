#include "CharacterSample.h"
#include <string>
#include <hash_map>
#include "AbstractMatrix.h"
#include "XMLDataParser.h"
#include "Input.h"

using namespace std;

#pragma once
class CharacterInput : public Input
{
public:
	CharacterInput(string xmlFile, int id, int dimension);
	shared_ptr<Sample> getNextSample() override;
	~CharacterInput();

private:
	hash_map<string, vector<double>> lookupTable;
	hash_map<string, shared_ptr<AbstractMatrix>> tagTable;
	size_t dimension;
};

