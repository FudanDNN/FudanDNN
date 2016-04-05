#include "Model.h"
#include "layer/SGDSolver.h"

int main(){
	
	shared_ptr<Model> model(new Model());
	size_t input = model->addInput(XMLINPUT, "dataset/mnist32_1000.xml");

	size_t cnn1 = model->addConvolutionLayerToNetwork(32, 32, 5, 5, 1, 8, 1, RANDOM_SQRT, SGD, 0, 0.001, 0.001, 0.0, 0);

	size_t pool1 = model->addMaxPoolingLayerToNetwork(28, 28, 2, 2, 8, 8, 2, 0);

	size_t cnn2 = model->addConvolutionLayerToNetwork(14, 14, 5, 5, 8, 16, 1, RANDOM_SQRT, SGD, 0, 0.001, 0.001, 0.0, 0);

	size_t pool2 = model->addMaxPoolingLayerToNetwork(10, 10, 2, 2, 16, 16, 2, 0);

	size_t concat = model->addConcatLayerToNetwork(5, 5, 16);

	size_t l1 = model->addLinearLayerToNetwork(400, 120, 1, RANDOM_SQRT, SGD, 0, 0.05, 0.05, 0.0, 0);
	size_t n1 = model->addNonlinearLayerToNetwork(120, RELU, 1, -3, 3, 0.001, 0.0001);
	size_t l2 = model->addLinearLayerToNetwork(120, 84, 1, RANDOM_SQRT, SGD, 0, 0.05, 0.05, 0.0, 0);
	size_t n2 = model->addNonlinearLayerToNetwork(84, RELU, 1, -3, 3, 0.001, 0.0001);
	size_t l3 = model->addLinearLayerToNetwork(84, 10, 1, RANDOM_SQRT, SGD, 0, 0.05, 0.05, 0.0, 0);
	size_t n3 = model->addNonlinearLayerToNetwork(10, SIGMOID, 1, -10, 10, 0.001, 0.0001);

	model->addEdgeToNetwork(cnn1, pool1);
	model->addEdgeToNetwork(pool1, cnn2);
	model->addEdgeToNetwork(cnn2, pool2);
	model->addEdgeToNetwork(pool2, concat);
	model->addEdgeToNetwork(concat, l1);
	model->addEdgeToNetwork(l1, n1);
	model->addEdgeToNetwork(n1, l2);
	model->addEdgeToNetwork(l2, n2);
	model->addEdgeToNetwork(n2, l3);
	model->addEdgeToNetwork(l3, n3);


	model->setCriteria(MSE, 10);

	model->setTrainingTimes(100000);
	model->setBatchSize(1);

	model->linkInputToNetwork(input, cnn1);

	model->run();
	
	/*
	shared_ptr<MatrixPool> mp = MatrixPool::getInstance();
	shared_ptr<Matrix> A = mp->allocMatrixUnclean(5, 5);
	shared_ptr<Matrix> B = mp->allocMatrixUnclean(3, 3);
	shared_ptr<Matrix> C = mp->allocMatrixUnclean(7, 7);
	A->initializeRandom(-10, 10);
	B->initializeRandom(-10, 10);
	A->mapi(round);
	B->mapi(round);
	A->print();
	B->print();

	A->narrowCorr(B, 1)->print();
	A->narrowRConv(B, 1)->print();
	*/
	/*
	shared_ptr<Model> model(new Model());
	size_t input = model->addInput(XMLINPUT, "dataset/digital.xml");

	size_t cnn1 = model->addConvolutionLayerToNetwork(5, 5, 4, 4, 1, 10, 1, RANDOM_NORM1, SGD, 0, 0.005, 0.005, 0, 0);

	size_t pool1 = model->addMaxPoolingLayerToNetwork(2, 2, 2, 2, 10, 10, 2, 0);

	size_t concat = model->addConcatLayerToNetwork(1, 1, 10);

	size_t l1 = model->addLinearLayerToNetwork(10, 30, 1, 1, SGD, 0, 0.001, 0.005, 0, 0);
	size_t n1 = model->addNonlinearLayerToNetwork(30, RELU, 1, -3, 3, 0.001, 0.0001);
	size_t l2 = model->addLinearLayerToNetwork(30, 10, 1, 1, SGD, 0, 0.001, 0.005, 0, 0);
	size_t n2 = model->addNonlinearLayerToNetwork(10, SIGMOID, 1, -3, 3, 0.001, 0.0001);


	model->addEdgeToNetwork(cnn1, pool1);
	model->addEdgeToNetwork(pool1, concat);
	model->addEdgeToNetwork(concat, l1);
	model->addEdgeToNetwork(l1, n1);
	model->addEdgeToNetwork(n1, l2);
	model->addEdgeToNetwork(l2, n2);


	model->setCriteria(MSE, 10);

	model->setTrainingTimes(100000);
	model->setBatchSize(1);

	model->linkInputToNetwork(input, cnn1);

	model->run();
	*/
	system("pause");
}