#include "AbstractComponent.h"

void AbstractComponent::setVisualValue(vector<shared_ptr<AbstractMatrix>> visualValue)
{
	visualGradient.clear();
	for (size_t i = 0; i < visualValue.size(); i++)
	{
		shared_ptr<AbstractMatrix> m(new Matrix(this->visualRow, this->visualColumn));
		m->initializeValue(0, 0);
		visualGradient.push_back(m);
	}
	this->visualValue = visualValue;
}

vector<shared_ptr<AbstractMatrix>> AbstractComponent::getVisualValue()
{
	return visualValue;
}

vector<shared_ptr<AbstractMatrix>> AbstractComponent::getHiddenValue()
{
	return hiddenValue;
}

void AbstractComponent::setHiddenGradient(vector<shared_ptr<AbstractMatrix>> hiddenGradient)
{
	if (this->hiddenGradient.size()==0){
		this->hiddenGradient = hiddenGradient;
	}
	else{
		vector<shared_ptr<AbstractMatrix>> temp;
		size_t i=0;
		for (shared_ptr<AbstractMatrix> matrix : hiddenGradient){
			temp.push_back(this->hiddenGradient[i]->add(matrix));
			i++;
		}
		this->hiddenGradient = temp;
	}
}

vector<shared_ptr<AbstractMatrix>> AbstractComponent::getVisualGradient()
{
	return this->visualGradient;
}

vector<shared_ptr<AbstractMatrix>> AbstractComponent::getHiddenGradient()
{
	return this->hiddenGradient;
}

void AbstractComponent::setNum(size_t num)
{
	this->num = num;
}

size_t AbstractComponent::getNum()
{
	return num;
}
