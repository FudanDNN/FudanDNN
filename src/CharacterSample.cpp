#include "CharacterSample.h"


CharacterSample::CharacterSample()
{
}


CharacterSample::~CharacterSample()
{
}

vector<string> CharacterSample::getSentece()
{
	return this->sentence;
}

void CharacterSample::setSentece(vector<string> sentence)
{
	this->sentence = sentence;
}

vector<string> CharacterSample::getTag()
{
	return this->tag;
}

void CharacterSample::setTag(vector<string> tag)
{
	this->tag = tag;
}

size_t CharacterSample::getLength()
{
	return this->length;
}