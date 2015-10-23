#include "NetworkNode.h"

NetworkNode::NetworkNode(int id, shared_ptr<AbstractLayer> layer){
	this->id = id;
	this->layer = layer;
}

int NetworkNode::getId(){
	return id;
}

void NetworkNode::addNextNode(shared_ptr<NetworkNode> next){
	nextNodes.push_back(next);
}

void NetworkNode::addBeforeNode(shared_ptr<NetworkNode> before){
	beforeNodes.push_back(before);
}

vector<shared_ptr<NetworkNode>> NetworkNode::getNextNode(){
	return nextNodes;
}

vector<shared_ptr<NetworkNode>> NetworkNode::getBeforeNode(){
	return beforeNodes;
}

shared_ptr<AbstractLayer> NetworkNode::getLayer(){
	return layer;
}

void NetworkNode::addInput(shared_ptr<Input> input){
	this->input = input;
}
shared_ptr<Input> NetworkNode::getInput(){
	return this->input;
}

void NetworkNode::clearEdges(){
    this->nextNodes.clear();
    this->beforeNodes.clear();
}