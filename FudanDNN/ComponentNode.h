#pragma once
#include "stdafx.h"
#include <vector>
#include "AbstractComponent.h"

using namespace std;

#pragma once
class ComponentNode
{
public:
	ComponentNode(int id, shared_ptr<AbstractComponent> component);
	int getId();
	void addNextNode(shared_ptr<ComponentNode> next);
	void addBeforeNode(shared_ptr<ComponentNode> before);
	vector<shared_ptr<ComponentNode>> getNextNode();
	vector<shared_ptr<ComponentNode>> getBeforeNode();
	shared_ptr<AbstractComponent> getComponent();

private:
	int id;
	shared_ptr<AbstractComponent> component;
	vector<shared_ptr<ComponentNode>> nextNodes;
	vector<shared_ptr<ComponentNode>> beforeNodes;
};