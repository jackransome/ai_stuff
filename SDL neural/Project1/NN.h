#pragma once


struct Node
{
	double value;
	bool exists; // because the input and output layers might be smaller than the hidden one
	//d value d error
	double dedv;
};

const int inputs = 6;
const int layers = 2;
const int perLayer = 6; //must be > inputs at the moment due to size of Node.fromConnections being defined as perLay
const int outputs = 6;

class NN {
public:
	NN();


	// neuron 2 in layer 1 connects to a neuron 3 in layer 2 by connections[1][2][3]
	// input layer is layer 0
	// output layer is layer layers-1
	double connections[layers - 1][perLayer][perLayer];

	double dErrorDConnections[layers][perLayer][perLayer];
	double batchDErrorDConnections[layers][perLayer][perLayer];

	float errors[outputs];

	float inputSet[inputs];

	float outputSet[outputs];

	float learningFactor = 1 / (layers*perLayer*perLayer);

	// same way to access as connections, just missing the index of the next layers neurons
	Node nodes[layers][perLayer];

	void forward(int _layer, int _index);
	
	void back(int _layer, int _index, int _nodeInPrevLayerIndex);

	void changeWeightsBasedOnBatchGradients(float _learningFactor);

	float addGradientsBasedOnWeights();

	void init();

	void trainNetwork();

	float test();

private:

	float absolute(float _x) {
		if (_x < 0) {
			return -_x;
		}
		return _x;
	}

};