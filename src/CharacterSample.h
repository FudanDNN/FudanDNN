#pragma once
#include "utils.h"

class CharacterSample
{
public:
	CharacterSample();
	vector<string> getSentece();
	void setSentece(vector<string>);
	vector<string> getTag();
	void setTag(vector<string>);
	size_t getLength();
	~CharacterSample();

protected:
	vector<string> sentence;
	vector<string> tag;
	size_t length;
};

