#ifndef __FUDANDNN_XMLDATAPARSER_H
#define __FUDANDNN_XMLDATAPARSER_H
#include "utils.h"
#include "Matrix.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "Sample.h"

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