#ifndef __FUDANDNN_COMPONENTNODE_H
#define __FUDANDNN_COMPONENTNODE_H
#include "utils.h"
#include "AbstractComponent.h"

class ComponentNode
{
public:
	ComponentNode(int id, shared_ptr<AbstractComponent> component);
	int getId();
    void setId(int id){ this->id = id; }
	void addNextNode(shared_ptr<ComponentNode> next);
	void addBeforeNode(shared_ptr<ComponentNode> before);
	vector<shared_ptr<ComponentNode>> getNextNode();
	vector<shared_ptr<ComponentNode>> getBeforeNode();
	shared_ptr<AbstractComponent> getComponent();
    void clearEdges();

private:
	int id;
	shared_ptr<AbstractComponent> component;
	vector<shared_ptr<ComponentNode>> nextNodes;
	vector<shared_ptr<ComponentNode>> beforeNodes;
};
#endif