#include "CharacterInput.h"

CharacterInput::CharacterInput(string xmlFile, int id, int dimension) : Input(xmlFile, id)
{
	this->dimension = dimension;
}

shared_ptr<Sample> CharacterInput::getNextSample()
{

	shared_ptr<CharacterSample> cs = shared_ptr<CharacterSample>(parser->getNextCharacterSample());
	vector<string> s = cs->getSentece();
	size_t size = cs->getLength();
	shared_ptr<AbstractMatrix> m = shared_ptr<AbstractMatrix>(new Matrix(dimension, size));
	for (size_t i = 0; i < size; i++)
	{
		vector<double> embedding = lookupTable.find(s[i])->second;
		m->setColumn(i, embedding);
	}
	shared_ptr<AbstractMatrix> tagCode = tagTable.find(cs->getTag()[0])->second;
	
	shared_ptr<Sample> sample(new Sample(m, tagCode, "tagging",
		m->getRowSize(), m->getColumnSize(), tagCode->getRowSize(), tagCode->getColumnSize()));

	return sample;

}
