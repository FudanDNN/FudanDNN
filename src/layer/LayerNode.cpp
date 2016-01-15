#include "layer\LayerNode.h"

LayerNode::LayerNode(int id, shared_ptr<Layer> layer){
	this->id = id;
	this->layer = layer;
}

int LayerNode::getId(){
	return id;
}

void LayerNode::addNextNode(shared_ptr<LayerNode> next){
	nextNodes.push_back(next);
}

void LayerNode::addPrevNode(shared_ptr<LayerNode> before){
	prevNodes.push_back(before);
}

vector<shared_ptr<LayerNode>> LayerNode::getNextNode(){
	return nextNodes;
}

vector<shared_ptr<LayerNode>> LayerNode::getPrevNode(){
	return prevNodes;
}

shared_ptr<Layer> LayerNode::getLayer(){
	return layer;
}

void LayerNode::clearEdges(){
	this->nextNodes.clear();
	this->prevNodes.clear();
}

void LayerNode::setInputCell(shared_ptr<InputCell> inputCell)
{
	this->inputCell = inputCell;
}

shared_ptr<InputCell> LayerNode::getInputCell()
{
	return this->inputCell;
}
