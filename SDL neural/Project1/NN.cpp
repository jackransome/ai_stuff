#include "NN.h"
#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include <time.h>
#include <math.h>
#include <algorithm>

NN::NN()
{
}

void NN::forward(int _layer, int _index) {
	nodes[_layer][_index].value = 0;
	//adding all previous layer neurons values multiplied by the weight of the connection to them
	for (int i = 0; i < perLayer; i++) {
		if (!nodes[_layer - 1][i].exists) {
			break;
		}
		nodes[_layer][_index].value += nodes[_layer - 1][i].value * connections[_layer - 1][i][_index] + nodes[_layer][_index].bias;

	}
	//clamping to 0
	if (nodes[_layer][_index].value < 0) {
		nodes[_layer][_index].value = 0;
	}

}

// getting the gradient d weight from pre / d error
void NN::back(int _layer, int _index, int _nodeInPrevLayerIndex) {

	//getting d value of current / d weight from prev
	float dcdwp = nodes[_layer - 1][_nodeInPrevLayerIndex].value;
	//getting d error / d value of current
	float dedc = 0;
	for (int i = 0; i < perLayer; i++) {
		if (nodes[_layer + 1][i].exists) {
			// getting the d err d value of node in next layer times the weight between the two nodes
			double x = nodes[_layer + 1][i].dedv * connections[_layer][_index][i];
			// adding d err d weight of weight between this node and all nodes in the next layer
			dedc += x;// dErrorDConnections[_layer][_index][i];
		}
	}
	nodes[_layer][_index].dedv = dedc;
	//getting d error / d weight from prev
	float dedwp = dedc * dcdwp;
	dErrorDConnections[_layer - 1][_nodeInPrevLayerIndex][_index] = dedwp;

}

void NN::changeWeightsBasedOnBatchGradients(float _learningFactor) {
	double magnitude = 0;
	for (int i = 0; i < layers-1; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				magnitude += pow(batchDErrorDConnections[i][j][k], 2);
			}
		}
	}
	for (int i = 1; i < layers - 1; i++) {
		for (int j = 0; j < perLayer; j++) {
			if (nodes[i][j].exists) {
				magnitude += pow(nodes[i][j].dedv, 2);
			}
		}
	}
	
	magnitude = sqrt(magnitude);
	lastGradientMagnitude = magnitude;
	magnitudeCounter++;
	//limiting the magnitude of the gradient to 10^10
	if (magnitude > 10000000000) {
		std::cout << "magnitude over 10^10\n";
		double multiplier = 10000000000.0f / magnitude;
		for (int i = 0; i < layers - 1; i++) {
			for (int j = 0; j < perLayer; j++) {
				for (int k = 0; k < perLayer; k++) {
					batchDErrorDConnections[i][j][k] *= multiplier;
				}
			}
		}
		for (int i = 1; i < layers - 1; i++) {
			for (int j = 0; j < perLayer; j++) {
				if (nodes[i][j].exists) {
					nodes[i][j].dedv *= multiplier;
				}
			}
		}
	}
	if (magnitude >= 5) {
		magnitudeCounter = 0;
	}
	last5Magnitudes[magnitudeCounter] = lastGradientMagnitude;
	//changing weights based on batch gradients:
	for (int i = 0; i < layers-1; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {

				//learning factor is the max percentage bigger / smaller the connection should be able to get
				//batchDErrorDConnections[i][j][k] / gradientTotal is the percentage of that learning factor to be used
				double x = 1.0f - _learningFactor * (batchDErrorDConnections[i][j][k] / magnitude);
				double old = connections[i][j][k];
				//connections[i][j][k] = connections[i][j][k] * x;
				if (magnitude != 0) {
					connections[i][j][k] = connections[i][j][k] - _learningFactor * batchDErrorDConnections[i][j][k];
				}

			}
		}
	}
	//changing biases based on batch gradients
	for (int i = 1; i < layers - 1; i++) {
		for (int j = 0; j < perLayer; j++) {
			if (nodes[i][j].exists) {
				if (magnitude != 0) {
					nodes[i][j].bias = nodes[i][j].bias - _learningFactor * batchDErrorDBiases[i][j];
				}
			}
		}
	}
}

float NN::addGradientsBasedOnWeights() {
	//clearing dErrorDConnections
	for (int i = 0; i < layers-1; i++) {
		for (int j = 0; j < perLayer; j++) { 
			for (int k = 0; k < perLayer; k++) {
				dErrorDConnections[i][j][k] = 0;
			}
		}
	}
	//forward propagation
	run();
	// calculating the error for each output based on the calculated output layer values and the output set
	for (int i = 0; i < outputs; i++) {
		errors[i] = pow(outputSet[i] - nodes[layers - 1][i].value, 2);
		if (nodes[layers - 1][i].value > 500) {
			int f = 3;
		}
		//std::cout << "r: " << outputSet[i] << ", p: " << nodes[layers - 1][i].value << std::endl;
		
	}
	//calculating total error
	float totalError = 0;
	for (int i = 0; i < outputs; i++) {
		totalError += errors[i];
	}
	//fill dErrorDConnections[layers-2][indexInLayer][i] for last layer of connections
	for (int i = 0; i < perLayer; i++) {
		for (int j = 0; j < outputs; j++) {
			dErrorDConnections[layers - 2][i][j] = -2 * nodes[layers - 2][i].value*(outputSet[j] - nodes[layers - 1][j].value);
			nodes[layers - 1][j].dedv = -2 * (outputSet[j] - nodes[layers - 1][j].value);
		}
	}
	// looping from last layer to last non output layer running back on each to populate dConnectionsDError
	for (int i = layers - 2; i > 0; i--) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				if (nodes[i][j].exists && nodes[i - 1][k].exists) {
					back(i, j, k);
				}	
			}
		}
	}
	//adding the gradients for this case to the batch:
	for (int i = 0; i < layers-1; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				batchDErrorDConnections[i][j][k] += dErrorDConnections[i][j][k];
			}
		}
	}
	//adding biase gradients to the batch
	for (int i = 1; i < layers; i++) {
		for (int j = 0; j < perLayerDimension; j++) {
			if (nodes[i][j].exists) {
				batchDErrorDBiases[i][j] += nodes[i][j].dedv;
			}
		}
	}
	return totalError;
}

void NN::init(int _inputs, int _layers, int _perLayer, int _outputs)
{
	stopLearning = false;
	inputs = _inputs;
	layers = _layers;
	perLayer = _perLayer;
	outputs = _outputs;
	//setting up caching for training sets (batching)
	maxCachedSets = 1000;
	numberOfCachedSets = 0;
	cachedInputs = (double**)malloc(maxCachedSets * inputs * sizeof(double*));
	cachedOutputs = (double**)malloc(maxCachedSets*outputs * sizeof(double*));


	perLayerDimension = std::max(std::max(perLayer, inputs), outputs);
	//allocating errors memory:
	errors = (double*)malloc(outputs * sizeof(double));
	//allocating outputSet memory:
	outputSet = (double*)malloc(outputs * sizeof(double));
	//allocating inputSet memory:
	inputSet = (double*)malloc(inputs * sizeof(double));
	//allocating nodes memory
	nodes = (Node**)malloc(layers*(sizeof(Node*)));
	for (int i = 0; i < layers; i++) {
		nodes[i] = (Node*)malloc(perLayerDimension * sizeof(Node));
		for (int j = 0; j < perLayerDimension; j++) {
			nodes[i][j].bias = 0;
			//setting up nodes as existing or not depending on different numbers of inputs outputs and amount of nodes in hidden layers
			if (i == 0) {
				if (j < inputs) {
					nodes[i][j].exists = true;
				}
				else {
					nodes[i][j].exists = false;
				}
			}
			else if (i == layers - 1) {
				if (j < outputs) {
					nodes[i][j].exists = true;
				}
				else {
					nodes[i][j].exists = false;
				}
			}
			else {
				if (j < perLayer) {
					nodes[i][j].exists = true;
				}
				else {
					nodes[i][j].exists = false;
				}
			}
		}
	}
	//allocating batchDErrorDBiases memory:
	batchDErrorDBiases = (double**)malloc(layers*(sizeof(double*)));
	for (int i = 0; i < layers; i++) {
		batchDErrorDBiases[i] = (double*)malloc(perLayerDimension * sizeof(double));
	}
	//seeding rand()
	srand(time(NULL));
	//allocating dErrorDConnections memory
	dErrorDConnections = (double***)malloc((layers-1) * sizeof(double**));
	for (int i = 0; i < layers-1; i++) {
		dErrorDConnections[i] = (double**)malloc(perLayerDimension * sizeof(double));
		for (int j = 0; j < perLayerDimension; j++) {
			dErrorDConnections[i][j] = (double*)malloc(perLayerDimension * sizeof(double));
		}
	}
	//allocating batchDErrorDConnections memory
	batchDErrorDConnections = (double***)malloc((layers - 1) * sizeof(double**));
	for (int i = 0; i < layers - 1; i++) {
		batchDErrorDConnections[i] = (double**)malloc(perLayerDimension * sizeof(double));
		for (int j = 0; j < perLayerDimension; j++) {
			batchDErrorDConnections[i][j] = (double*)malloc(perLayerDimension * sizeof(double));
		}
	}
	//allocating connections memory and setting all weights to random between 0 and 1
	connections = (double***)malloc((layers-1) * sizeof(double**));
	for (int i = 0; i < layers-1; i++) {
		connections[i] = (double**)malloc(perLayerDimension * sizeof(double*));
		for (int j = 0; j < perLayerDimension; j++) {
			connections[i][j] = (double*)malloc(perLayerDimension * sizeof(double));
			for (int k = 0; k < perLayerDimension; k++) {
				connections[i][j][k] = -0.5 + 1 * ((float)rand() / (float)RAND_MAX);
			}
		}
	}
	//setting up nodes array (for the right umber of inputs and outputs)
	for (int i = 0; i < layers; i++) {
		for (int j = 0; j < perLayer; j++) {
			nodes[i][j].exists = true;
		}
	}
	for (int i = inputs; i < perLayer; i++) {
		nodes[0][i].exists = false;
	}
	for (int i = outputs; i < perLayer; i++) {
		nodes[layers - 1][i].exists = false;
	}
}


void NN::trainNetwork() {
	//train
	int batchSize = 1000;
	
	//clearing the batch gradient
	clearBatchGradient();
	double error = 0;
	for (int m = 0; m < batchSize; m++) {
		
		//setup input set
		for (int i = 0; i < inputs; i++) {
			inputSet[i] = (float)rand() / (float)RAND_MAX;
		}
		//setup output set
		//outputSet[0] = inputSet[2] * 0.5 + inputSet[1] * 0.25;
		//outputSet[1] = inputSet[0] * 0.5 + inputSet[2] * 0.25;
		//outputSet[2] = inputSet[1] * 0.5 + inputSet[0] * 0.25;
		
		for (int i = 0; i < outputs; i++) {
			if (i == outputs - 1) {
				outputSet[i] = inputSet[0] * 0.5;
			}
			else {
				outputSet[i] = inputSet[i + 1] * 0.5;
			}
		}
		//adding the gradients for this batch
		//error = (error * m + addGradientsBasedOnWeights())/(m +1);
		double x = addGradientsBasedOnWeights();
		//std::cout << " error : " << x << std::endl;
		error += x;
		
	}
	std::cout << " average error : " << error / batchSize << std::endl;
	//printing the average error for this last batch

	changeWeightsBasedOnBatchGradients(0.1);
	
}

void NN::addTrainingSet(double * _inputs, double * _outputs){
	cachedInputs[numberOfCachedSets] = (double*)malloc(sizeof(double)*inputs);
	memcpy(cachedInputs[numberOfCachedSets], _inputs, sizeof(double)*inputs);
	cachedOutputs[numberOfCachedSets] = (double*)malloc(sizeof(double)*outputs);
	memcpy(cachedOutputs[numberOfCachedSets], _outputs, sizeof(double)*outputs);
	numberOfCachedSets++;
	//std::cout << "XX: memory: " << _inputs[0] << "| graph position: " << _inputs[1] << "| prediction: " << _inputs[2] << "| memory action: " << _inputs[3] << ":: reward: " << _outputs[0] << "\n";
}

void NN::clearTrainingSets(){
	//std::cout << "numberOfCachedSets = " << numberOfCachedSets << "\n";
	numberOfCachedSets = 0;
}

void NN::trainOnCachedSets(float _learningRate){
	if (stopLearning) {
		return;
	}
	clearBatchGradient();
	double error = 0;
	double singleError;
	bool toPerturb = false;
	for (int i = 0; i < numberOfCachedSets; i++) {
		singleError = addGradientFromCachedSet(i);
		error += singleError;
		if (singleError > 0.5 && counter > 1400) {
			toPerturb = true;
		}
	}
	if (toPerturb) {
		std::cout << "perturbing!(disabled right now) error: " << error / numberOfCachedSets << "\n";
		//perturb();
		counter = 0;
	}
	counter++;
	//std::cout << "avg err: " << error << "\n";
	errorCounter++;
	if (errorCounter == 5) {
		errorCounter = 0;
	}
	last5Errors[errorCounter] = error / numberOfCachedSets;

	changeWeightsBasedOnBatchGradients(_learningRate);
}

double NN::addGradientFromCachedSet(int _index){
	inputSet = cachedInputs[_index];
	outputSet = cachedOutputs[_index];
	//std::cout << "memory: " << inputSet[0] << "| graph position: " << inputSet[1] << "| prediction: " << inputSet[2] << "| memory action: " << inputSet[3] << ":: reward: " << outputSet[0] << "\n";
	//double board[3][3];
	//for (int i = 0; i < 3; i++) {
	//	for (int k = 0; k < 3; k++) {
	//		if (inputSet[i * 3 + k] == 1) {
	//			board[i][k] = 1;
	//		} else if (inputSet[i * 3 + k + 9] == 1) {
	//			board[i][k] = 2;
	//		}
	//		else if (inputSet[i * 3 + k + 18] == 1) {
	//			board[i][k] = 0;
	//		}
	//		
	//	}
	//}
	//for (int i = 0; i < 3; i++) {
	//	std::cout << "\n";
	//	for (int j = 0; j < 3; j++) {
	//		std::cout << board[i][j] << " ";
	//	}
	//}
	//if (outputSet[0] == 10) {
	//	std::cout << outputSet[0] << "\n";
	//}
	//else {
	//	std::cout << outputSet[0] << "\n";
	//}
	
	return addGradientsBasedOnWeights();
}

float NN::test(){
	float error = 0;
	int batchSize = 1;
	for (int m = 0; m < batchSize; m++) {
		//setup input set
		for (int i = 0; i < inputs; i++) {
			inputSet[i] =  round((float)rand() / (float)RAND_MAX);
		}
		//setup output set
		//outputSet[0] = inputSet[2] * 0.5 + inputSet[1] * 0.25;
		//outputSet[1] = inputSet[0] * 0.5 + inputSet[2] * 0.25;
		//outputSet[2] = inputSet[1] * 0.5 + inputSet[0] * 0.25;
		
		for (int i = 0; i < outputs; i++) {
			if (i == outputs - 1) {
				outputSet[i] = inputSet[0];
			}
			else {
				outputSet[i] = inputSet[i + 1];
			}
		}
		//adding the gradients for this batch
		//error = (error * m + addGradientsBasedOnWeights())/(m +1);
		getBinaryTrainingSet();
		error += addGradientsBasedOnWeights();

	}
	for (int i = 0; i < outputs; i++) {
		std::cout << outputSet[i] << " vs " << nodes[layers - 1][i].value << "\n";
	}
	
	return error / batchSize;
}

void NN::getBinaryTrainingSet() {
	int input1 = round(3*((float)rand() / (float)RAND_MAX));
	int input2 = round(3 * ((float)rand() / (float)RAND_MAX));
	if (((float)rand() / (float)RAND_MAX) > 0.9) {
		if (((float)rand() / (float)RAND_MAX) > 0.5) {
			input1 = input2 = 0;
		}
		else {
			input1 = input2 = 3;
		}
	}
	int output = input1 + input2;
	inputSet[0] = 1 & input1;
	inputSet[1] = (2 & input1) > 0;
	inputSet[2] = 1 & input2;
	inputSet[3] = (2 & input2) > 0;
	//outputSet[0] = 1 & output;
	//outputSet[1] = (2 & output) > 0;
	//outputSet[2] = (4 & output) > 0;
	outputSet[0] = 0;
	outputSet[1] = 0;
	outputSet[2] = 0;
	outputSet[3] = 0;
	outputSet[4] = 0;
	outputSet[5] = 0;
	outputSet[6] = 0;
	outputSet[output] = 1;

}

int NN::getLast5MagnitudesAverage()
{
	int total = 0;
	for (int i = 0; i < 5; i++) {
		total += last5Magnitudes[i];
	}
	return total / 5;
}

float NN::getLast5ErrorsAverage()
{
	float total = 0;
	for (int i = 0; i < 5; i++) {
		total += last5Errors[i];
	}
	return total / 5;
}

void NN::addTrainingSetTest(){
	
	for (int i = 0; i < inputs; i++) {
		inputSet[i] = round((float)rand() / (float)RAND_MAX);
	}
	//setup output set
	//outputSet[0] = inputSet[2] * 0.5 + inputSet[1] * 0.25;
	//outputSet[1] = inputSet[0] * 0.5 + inputSet[2] * 0.25;
	//outputSet[2] = inputSet[1] * 0.5 + inputSet[0] * 0.25;

	/*for (int i = 0; i < outputs; i++) {
		if (i == outputs - 1) {
			outputSet[i] = inputSet[0];
		}
		else {
			outputSet[i] = inputSet[i + 1];
		}
	}*/
	getBinaryTrainingSet();
	addTrainingSet(inputSet, outputSet);
}

void NN::clearBatchGradient(){
	//clearing connections gradient
	for (int i = 0; i < layers - 1; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				batchDErrorDConnections[i][j][k] = 0;
			}
		}
	}
	//clearing biases gradient
	for (int i = 0; i < layers; i++) {
		for (int j = 0; j < perLayerDimension; j++) {
			batchDErrorDBiases[i][j] = 0;
		}
	}
}

void NN::perturb(){
	for (int i = 0; i < layers - 1; i++) {
		for (int j = 0; j < perLayer; j++) {
			nodes[i][j].bias += -0.05 + 0.1*((float)rand() / (float)RAND_MAX);
			for (int k = 0; k < perLayer; k++) {
				//if (connections[i][j][k] < 0.01) {
					connections[i][j][k] += -0.15+0.3*((float)rand() / (float)RAND_MAX);
					
				//}
			}
		}
	}
}

void NN::run() {
	//setting first layer to the input set
	for (int i = 0; i < inputs; i++) {
		nodes[0][i].value = inputSet[i];
	}
	// looping from first(input) layer to last non output layer and running forward() on each
	for (int i = 1; i < layers; i++) {
		for (int j = 0; j < perLayer; j++) {
			forward(i, j);
		}
	}

	//checking how many of the neurons are deactivated completely
	/*
	int numberOfNodes = 0;
	int nodes0 = 0;
	for (int i = 0; i < layers; i++) {
		for (int j = 0; j < perLayer; j++) {
			if (nodes[i][j].exists){
				numberOfNodes++;
				if (nodes[i][j].value == 0) {
					nodes0++;
				}
			}
		}
	}
	std::cout << (int)(100*((float)nodes0 / (float)numberOfNodes)) << "% of neurons are off\n";
	*/
}


void NN::doTestSet() {
	int mistakes = 0;
	int batchSize = 100;
	for (int m = 0; m < batchSize; m++) {
		getBinaryTrainingSet();
		run();
		//check if largest output in the last layer if the same one that is largest in the output set
		int nnIndex = 0;
		for (int i = 1; i < outputs; i++) {
			if (nodes[layers - 1][i].value > nodes[layers - 1][nnIndex].value) {
				nnIndex = i;
			}
		}
		int setIndex = 0;
		for (int i = 1; i < outputs; i++) {
			if (outputSet[i] > outputSet[setIndex]) {
				setIndex = i;
			}
		}
		//if incorrect answer:
		if (setIndex != nnIndex) {
			mistakes++;
		}
	}
	float percentage = (float)(batchSize - mistakes) / (float)batchSize;
	if (percentage == 1) {
		stopLearning = true;
	}
	std::cout << mistakes << " wrong out of " << batchSize << " runs: " << 100*percentage << "% correct\n";
}