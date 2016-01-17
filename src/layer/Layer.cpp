#include "layer/Layer.h"
#include "util/Matrix.h"


//update error
void Layer::setVisualValue(vector<shared_ptr<Matrix>> visualValue)
{
	visualGradient.clear();
	for (size_t i = 0; i < visualValue.size(); i++)
	{
		shared_ptr<Matrix> m(new Matrix(this->visualRow, this->visualColumn));
		m->setValues(0);
		visualGradient.push_back(m);
	}
	this->visualValue = visualValue;
}

//add hidden layer gradient
void Layer::addHiddenGradient(vector<shared_ptr<Matrix>> hiddenGradient)
{
	if (this->hiddenGradient.size() == 0){
		this->hiddenGradient = hiddenGradient;
	}
	else{
		vector<shared_ptr<Matrix>> temp;
		size_t i = 0;
		for (shared_ptr<Matrix> matrix : hiddenGradient){
			temp.push_back(this->hiddenGradient[i]->add(matrix));
			i++;
		}
		this->hiddenGradient = temp;
	}
	
}

vector<shared_ptr<Matrix>> Layer::getVisualValue()
{
	return visualValue;
}

vector<shared_ptr<Matrix>> Layer::getHiddenValue()
{
	return hiddenValue;
}

vector<shared_ptr<Matrix>> Layer::getVisualGradient()
{
	return visualGradient;
}

vector<shared_ptr<Matrix>> Layer::getHiddenGradient()
{
	return hiddenGradient;
}

size_t Layer::getVisualRow()
{
	return this->visualRow;
};

size_t Layer::getVisualColumn()
{
	return this->visualColumn;
};

size_t Layer::getHiddenRow()
{
	return this->hiddenRow;
};

size_t Layer::getHiddenColumn()
{
	return this->hiddenColumn;
};

void Layer::setNetworkName(string networkName)
{
	this->networkname = networkName;
};

// rate for dropout
double Layer::getDropoutRate()
{
	return this->dropoutRate;
};

void Layer::setDropoutRate(double dropoutRate)
{
	this->dropoutRate = dropoutRate;
};

size_t Layer::getVisualSize(){
	return this->visualSize;
}

size_t Layer::getHiddenSize(){
	return this->hiddenSize;
}


