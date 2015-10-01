#pragma once
#include "utils.h"
#include "Matrix.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "Sample.h"
#include "CharacterSample.h"

class XMLDataParser{
public:
	XMLDataParser(string fileName);
	shared_ptr<Sample> getNextSample();
	string getType();
	string getNetworkName();
	size_t getSampleNum();
	bool isEnd();
	void backToHead();
	static vector<string> split(string str, string pattern);
	shared_ptr<CharacterSample> getNextCharacterSample();
private:
	string networkName;
	string fileName;
	string type;
	size_t iRowSize;
	size_t iColumnSize;
	size_t sampleNum;
	size_t oRowSize;
	size_t oColumnSize;
	size_t sampleIndex;
	TiXmlDocument *doc;
	TiXmlElement *sample;
	void init();
};