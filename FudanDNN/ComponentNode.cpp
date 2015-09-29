#include "ComponentNode.h"

ComponentNode::ComponentNode(int id, shared_ptr<AbstractComponent> component){
	this->id = id;
	this->component = component;
}

int ComponentNode::getId(){
	return this->id;
}

void ComponentNode::addNextNode(shared_ptr<ComponentNode> next){
	nextNodes.push_back(next);
}

void ComponentNode::addBeforeNode(shared_ptr<ComponentNode> before){
	beforeNodes.push_back(before);
}

vector<shared_ptr<ComponentNode>> ComponentNode::getNextNode(){
	return nextNodes;
}

vector<shared_ptr<ComponentNode>> ComponentNode::getBeforeNode(){
	return beforeNodes;
}

shared_ptr<AbstractComponent> ComponentNode::getComponent(){
	return this->component;
}