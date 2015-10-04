#include "XMLDataParser.h"
XMLDataParser::XMLDataParser(string fileName){
	this->fileName = fileName;
	cout << fileName << endl;
	doc=(new TiXmlDocument());
	if (!doc->LoadFile(fileName.c_str()))
	{
		cerr << doc->ErrorDesc() << endl;
	}
	init();
}
void XMLDataParser::backToHead(){
	this->sampleIndex = 0;
	TiXmlElement *root = doc->RootElement();
	TiXmlElement *samples = root->FirstChildElement("samples");
	this->sample = (samples->FirstChildElement("sample"));
}
bool XMLDataParser::isEnd(){
	return sampleIndex >= sampleNum;
}

shared_ptr<CharacterSample> XMLDataParser::getNextCharacterSample(){
	if (this->sampleIndex >= sampleNum){
		this->sampleIndex = 0;
		TiXmlElement *root = (doc->RootElement());
		TiXmlElement *samples = (root->FirstChildElement("samples"));
		this->sample = (samples->FirstChildElement("sample"));
	}
	//cout << "sampleIndex:" << sampleIndex << endl;
	TiXmlElement *inputElement = (this->sample->FirstChildElement("input"));
	TiXmlElement *outputElement = (this->sample->FirstChildElement("output"));
	string inputStr = inputElement->GetText();
	string outputStr = outputElement->GetText();
	shared_ptr<CharacterSample> trainingSample;
	vector<string> input;
	vector<string> output;

	//cout << "input:" << inputStr << endl;
	for (size_t i = 0; i < iRowSize; i++){
		vector<string> point = split(inputStr, " ");
		for (size_t j = 0; j < iColumnSize; j++){
			// column and row are invert in files 
			input[j]=point[j];
		}
		point.clear();
	}

	//cout << "output:" << outputStr << endl;
	for (size_t i = 0; i < oRowSize; i++){
		vector<string> point = split(outputStr, " ");
		for (size_t j = 0; j < oColumnSize; j++){
			// column and row are invert in files 
			output[j] = point[j];
		}
		point.clear();
	}
	trainingSample = shared_ptr<CharacterSample>(new CharacterSample());
	trainingSample->setSentece(input);
	trainingSample->setTag(output);
	TiXmlElement *releaseSample = sample;
	sample = (sample->NextSiblingElement());
	this->sampleIndex++;

	return trainingSample;
}

shared_ptr<Sample> XMLDataParser::getNextSample(){
	if (this->sampleIndex >= sampleNum){
		this->sampleIndex = 0;
		TiXmlElement *root = (doc->RootElement());
		TiXmlElement *samples = (root->FirstChildElement("samples"));
		this->sample = (samples->FirstChildElement("sample"));
	}
	//cout << "sampleIndex:" << sampleIndex << endl;
	TiXmlElement *inputElement = (this->sample->FirstChildElement("input"));
	TiXmlElement *outputElement = (this->sample->FirstChildElement("output"));
	string inputStr = inputElement->GetText();
	string outputStr = outputElement->GetText();
	shared_ptr<Sample> trainingSample;
	shared_ptr<AbstractMatrix> input(new Matrix(iRowSize, iColumnSize));
	shared_ptr<AbstractMatrix> output(new Matrix(oRowSize, oColumnSize));

	//cout << "input:" << inputStr << endl;
	for (size_t i = 0; i < iRowSize; i++){
		vector<string> point = split(inputStr, " ");
		for (size_t j = 0; j < iColumnSize; j++){
			double n;
			n = atof(point[j + i*iColumnSize].c_str());
			//cout << "input:" << n << endl;
			// column and row are invert in files 
			input->setValue(j, i, n);
		}
		point.clear();
	}

	//cout << "output:" << outputStr << endl;
	for (size_t i = 0; i < oRowSize; i++){
		vector<string> point = split(outputStr, " ");
		for (size_t j = 0; j < oColumnSize; j++){
			double n;
			n = atof(point[j + i*oColumnSize].c_str());
			//cout << "output:" << n << endl;
			// column and row are invert in files 
			output->setValue(i, j, n);
		}
		point.clear();
	}
	trainingSample = shared_ptr<Sample>(new Sample(input, output, type, iRowSize, iColumnSize, oRowSize, oColumnSize));
	TiXmlElement *releaseSample = sample;
	sample = (sample->NextSiblingElement());
	this->sampleIndex++;

	return trainingSample;
}
vector<string> XMLDataParser::split(string str, string pattern)
{
	string::size_type pos;
	vector<string> result;
	str += pattern;
	int size = str.size();
	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos<size)
		{
			string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
string XMLDataParser::getType(){
	return this->type;
}
string XMLDataParser::getNetworkName(){
	return this->networkName;
}
size_t XMLDataParser::getSampleNum(){
	return this->sampleNum;
}
void XMLDataParser::init(){
	TiXmlElement *root = (doc->RootElement());
	TiXmlElement *samples = (root->FirstChildElement("samples"));
	TiXmlAttribute *typeAttribute = (samples->FirstAttribute());
	this->type = typeAttribute->Value();
	// column and row are invert in files
	this->iColumnSize = typeAttribute->Next()->IntValue();
	this->iRowSize = typeAttribute->Next()->Next()->IntValue();
	this->sampleNum = typeAttribute->Next()->Next()->Next()->IntValue();
	this->oColumnSize = typeAttribute->Next()->Next()->Next()->Next()->IntValue();
	this->oRowSize = typeAttribute->Next()->Next()->Next()->Next()->Next()->IntValue();
	this->sample = (samples->FirstChildElement("sample"));
	this->sampleIndex = 0;

	/*cout << "type:" << type << endl;
	cout << "networkname:" << networkName << endl;
	cout << "row:" << iRowSize << endl;
	cout << "column:" << iColumnSize << endl;*/
}
/*int main(){
	XMLDataParser *parser = new XMLDataParser("dataset/digital.xml");
	for (int i = 0; i < 5000; i++){
		cout << i << ":" << endl;
		cout << parser->getNextSample()->getInput() << endl;
	}
	while (1);
}*/