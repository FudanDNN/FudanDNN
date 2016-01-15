#include "util/utils.h"
#include "NonlinearLayer.h"

class SoftShrinkLayer : public NonlinearLayer
{

public:
	SoftShrinkLayer(size_t size);
	string getNetworkName() override;
	void compute() override;
	void calculate() override;
	void gradient() override;
	void writeSelf(string) override;
	void readSelf(string) override;
	size_t getSize();

};
