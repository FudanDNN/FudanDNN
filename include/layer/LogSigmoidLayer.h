#include "util/utils.h"
#include "NonlinearLayer.h"

class LogSigmoidLayer : public NonlinearLayer
{

public:
	LogSigmoidLayer(size_t size);
	string getNetworkName() override;
	void compute() override;
	void calculate() override;
	void gradient() override;
	void writeSelf(string) override;
	void readSelf(string) override;
	size_t getSize();

};
