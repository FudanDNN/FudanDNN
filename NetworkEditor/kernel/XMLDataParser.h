#ifndef __FUDANDNN_XMLDATAPARSER_H
#define __FUDANDNN_XMLDATAPARSER_H
#include "utils.h"
#include "Matrix.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "Sample.h"
#include "CharacterSample.h"
#include "TrainingSample.h"

class XMLDataParser{
public:
	static size_t xmlCheck(string fileName);
	XMLDataParser(string fileName);
	shared_ptr<TrainingSample> getNextTrainingSample();
	shared_ptr<Sample> getNextSample();
	shared_ptr<CharacterSample> getNextCharacterSample();
	string getType();
	string getNetworkName();
	size_t getSampleNum();
	size_t getIRowSize();
	bool isEnd();
	void backToHead();
	static vector<string> split(string str, string pattern, size_t reserveSize);
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
#endif