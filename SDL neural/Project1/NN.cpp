#include "NN.h"
#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include <time.h>
#include <math.h>

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
		nodes[_layer][_index].value += nodes[_layer - 1][i].value * connections[_layer - 1][i][_index];
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
	//getting d value of current / d error
	float dedc = 0;
	for (int i = 0; i < perLayer; i++) {
		if (nodes[_layer + 1][i].exists) {
			dedc += dErrorDConnections[_layer][_index][i];
		}
	}
	//getting d error / d weight from prev
	float dedwp = dedc * dcdwp;
	dErrorDConnections[_layer - 1][_nodeInPrevLayerIndex][_index] = dedwp;
}

void NN::changeWeightsBasedOnBatchGradients(float _learningFactor) {
	double magnitude = 0;
	for (int i = 0; i < layers - 1; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				if (nodes[i][j].exists && nodes[i + 1][k].exists) {
					magnitude += pow(batchDErrorDConnections[i][j][k], 2);
				}
			}
		}
	}
	magnitude = sqrt(magnitude);
	//changing weights based on batch gradients:
	for (int i = 0; i < layers - 1; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				float test = _learningFactor * batchDErrorDConnections[i][j][k] / magnitude;
				connections[i][j][k] = connections[i][j][k] - _learningFactor * batchDErrorDConnections[i][j][k] / magnitude;
			}
		}
	}
}

float NN::addGradientsBasedOnWeights() {
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
	// calculating the error for each output based on the calculated output layer values and the output set
	for (int i = 0; i < outputs; i++) {
		errors[i] = pow(nodes[layers - 1][i].value - outputSet[i], 2);
	}
	//calculating total error
	float totalError = 0;
	for (int i = 0; i < outputs; i++) {
		totalError += errors[i];
	}
	//fill dErrorDConnections[layers-2][indexInLayer][i] for last layer of connections
	for (int i = 0; i < perLayer; i++) {
		for (int j = 0; j < outputs; j++) {
			
			dErrorDConnections[layers - 2][i][j] = 2 * nodes[layers - 2][i].value*(nodes[layers - 1][j].value - outputSet[i]);
 		}
	}
	// looping from last layer to last non output layer running back on each to populate dConnectionsDError
	for (int i = layers - 2; i > 0; i--) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				back(i, j, k); //WHAT ABOUT INPUT LAYER BEING SMALLER
			}
		}
	}
	//adding the gradients for this case to the batch:
	for (int i = 0; i < layers; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				batchDErrorDConnections[i][j][k] += dErrorDConnections[i][j][k];
			}
		}
	}
	return totalError;
}

void NN::init()
{
	//seeding rand()
	srand(time(NULL));
	//setting all connection weights to 1
	for (int i = 0; i < layers - 1; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				connections[i][j][k] = ((float)rand() / (float)RAND_MAX);
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
	int batchSize = 100;
	std::cout << " average error : ";
	float error = 0;;
	//clearing the batch gradient
	for (int i = 0; i < layers; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				batchDErrorDConnections[i][j][k] = 0;
			}
		}
	}
	for (int m = 0; m < batchSize; m++) {
		//setup input set
		for (int i = 0; i < inputs; i++) {
			inputSet[i] = (float)rand() / (float)RAND_MAX;
		}
		//setup output set
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
		error += addGradientsBasedOnWeights();
	}
	//printing the average error for this last batch
	std::cout << error / batchSize << std::endl;

	for (int i = 0; i < layers - 1; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				batchDErrorDConnections[i][j][k] /= batchSize;
			}
		}
	}
	changeWeightsBasedOnBatchGradients(0.01);

	//for (int i = 0; i < layers; i++) {
	//	for (int j = 0; j < perLayer; j++) {
	//		for (int k = 0; k < perLayer; k++) {
	//			std::cout << dConnectionsDError[i][j][k] << "\n";
	//		}
	//	}
	//}
}

float NN::test(){
	float error = 0;
	int batchSize = 100;
	for (int m = 0; m < batchSize; m++) {
		//setup input set
		for (int i = 0; i < inputs; i++) {
			inputSet[i] = i;// (float)rand() / (float)RAND_MAX;
		}
		//setup output set
		for (int i = 0; i < outputs; i++) {
			if (i == outputs - 1) {
				outputSet[i] = inputSet[0] * 0.5;
			}
			else {
				outputSet[i] = inputSet[i + 1] * 0.5;
			}
			outputSet[i] = inputSet[i]*5;
		}
		//adding the gradients for this batch
		//error = (error * m + addGradientsBasedOnWeights())/(m +1);
		error += addGradientsBasedOnWeights();
	}
	return error / batchSize;
}
