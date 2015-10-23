#include "RBMDataGenerator.h"

void generateCompressableData(string filename)
{

	srand((unsigned)time(NULL));

	ofstream fout(filename);
	shared_ptr<AbstractMatrix> mat(new Matrix(5, 3));
	mat->initializeValue(0, 1);

	for (size_t i = 0; i < 10000; i++)
	{
		shared_ptr<AbstractMatrix> vec(new Matrix(3, 1));
		vec->initializeValue(0, 1);
		//printf("%f\n", vec->getValue(0, 0));
		shared_ptr<AbstractMatrix> ans = vec->multipleLeft(mat);
		double norm2 = ans->norm2();
		double denominator = sqrt(norm2);
		ans = ans->multiple(1 / denominator);
		for (size_t j = 0; j < 5; j++)
		{
			fout << ans->getValue(j, 0) << ' ';
		}
		fout << '\n';
	}
}

void RBMPretrain(string filename)
{
	srand((unsigned)time(NULL));
	RBM *rbm = new RBM(5, 3, NonLinearFactory::SIGMOID, false, AbstractNetworkLayer::RANDOM);
	rbm->setWeightLearningRate(0.1);
	rbm->setBiasLearningRate(0.2);
	rbm->setRegularizationRate(0.0001);
	rbm->preTraining(filename);
}

// int main()
// {
// 	string filename = "CompressableData.txt";
// 	//generateCompressableData(filename);
// 	RBMPretrain(filename);
	
// 	string filename = "hello.txt";
// 	string line;
// 	ifstream fin = ifstream(filename);
// 	while (getline(fin, line))
// 	{
// 		cout << line.at(0) << endl;
// 	}
	
// 	printf("All processes have been done\n");

// }