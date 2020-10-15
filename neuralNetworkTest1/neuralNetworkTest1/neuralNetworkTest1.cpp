
#include "pch.h"
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

const int inputs = 4;
const int layers = 8;
const int perLayer = 4; //must be > inputs at the moment due to size of Node.fromConnections being defined as perLay
const int outputs = 4;

// neuron 2 in layer 1 connects to a neuron 3 in layer 2 by connections[1][2][3]
// input layer is layer 0
// output layer is layer layers-1
float connections[layers][perLayer][perLayer];

float dErrorDConnections[layers][perLayer][perLayer];
float batchDErrorDConnections[layers][perLayer][perLayer];

float errors[outputs];

float inputSet[inputs];

float outputSet[outputs];

float learningFactor = 1/(layers*perLayer*perLayer);

struct Node
{
	float value;
	bool exists; // because the input and output layers might be smaller than the hidden one
};

// same way to access as connections, just missing the index of the next layers neurons
Node nodes[layers][perLayer];

//calculate value of a node based of the values of the previous layer and the weights of the connections to them
void forward(int _layer, int _index) {
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
void back(int _layer, int _index, int _nodeInPrevLayerIndex) {

	//getting d value of current / d weight from prev
	float dcdwp = nodes[_layer - 1][_nodeInPrevLayerIndex].value;
	//getting d value of current / d error
	float dedc = 0;
	for (int i = 0; i < perLayer; i++) {
		dedc += dErrorDConnections[_layer][_index][i];
	}
	float dedwp = dedc * dcdwp;
	dErrorDConnections[_layer - 1][_nodeInPrevLayerIndex][_index] = dedwp;
}

void changeWeightsBasedOnBatchGradients(float _learningFactor) {
	float gradientTotal = 0;
	for (int i = 0; i < layers; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {
				gradientTotal += abs(batchDErrorDConnections[i][j][k]);
			}
		}
	}
	//changing weights based on batch gradients:
	for (int i = 0; i < layers; i++) {
		for (int j = 0; j < perLayer; j++) {
			for (int k = 0; k < perLayer; k++) {

				//learning factor is the max percentage bigger / smaller the connection should be able to get
				//batchDErrorDConnections[i][j][k] / gradientTotal is the percentage of that learning factor to be used
				connections[i][j][k] = connections[i][j][k] * (1 - _learningFactor * (batchDErrorDConnections[i][j][k] / gradientTotal));
			}
		}
	}
}

float addGradientsBasedOnWeights() {
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
		errors[i] = pow(outputSet[i] - nodes[layers - 1][i].value, 2);
	}
	//calculating total error
	float totalError = 0;
	for (int i = 0; i < outputs; i++) {
		totalError += errors[i];
	}
	//fill dErrorDConnections[layers-2][indexInLayer][i] for last layer of connections
	for (int i = 0; i < perLayer; i++) {
		for (int j = 0; j < outputs; j++) {
			dErrorDConnections[layers - 2][i][j] = -2 * nodes[layers - 2][i].value*(outputSet[i] - nodes[layers - 1][i].value);
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

int main()
{
	//seeding rand()
	srand(time(NULL));
	//setting all connection weights to 1
	for (int i = 0; i < layers; i++) {
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
		nodes[layers-1][i].exists = false;
	}


	//train
	int batchSize = 1000;
	int batches = 10000;
	std::cout << "batches: " << batches << "\nbatch size: " << batchSize << "\n";
	for (int l = 0; l < batches; l++) {
		std::cout << "round " << l << " average error : ";
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
		std::cout << error/batchSize << "\n";
		changeWeightsBasedOnBatchGradients(0.1);

		//for (int i = 0; i < layers; i++) {
		//	for (int j = 0; j < perLayer; j++) {
		//		for (int k = 0; k < perLayer; k++) {
		//			std::cout << dConnectionsDError[i][j][k] << "\n";
		//		}
		//	}
		//}
	}

	std::cout << "TESTING:\n";
	//testing:
	int testSize = 100;
	for (int m = 0; m < testSize; m++) {
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
		std::cout << "error: " << addGradientsBasedOnWeights() << "\n";
	}
	while (true) {}
}