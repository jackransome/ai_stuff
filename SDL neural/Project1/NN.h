#pragma once


struct Node
{
	double value;
	bool exists; // because the input and output layers might be smaller than the hidden one
	//d value d error
	double dedv;
};


class NN {
public:
	NN();

	int inputs;
	int layers;
	int perLayer; //must be > inputs at the moment due to size of Node.fromConnections being defined as perLay
	int outputs;

	// neuron 2 in layer 1 connects to a neuron 3 in layer 2 by connections[1][2][3]
	// input layer is layer 0
	// output layer is layer layers-1
	double ***connections;// [layers - 1][perLayer][perLayer];

	double ***dErrorDConnections;
	double ***batchDErrorDConnections;

	double *errors;

	double *inputSet;

	double *outputSet;

	// same way to access as connections, just missing the index of the next layers neurons
	Node **nodes;

	void forward(int _layer, int _index);
	
	void back(int _layer, int _index, int _nodeInPrevLayerIndex);

	void changeWeightsBasedOnBatchGradients(float _learningFactor);

	float addGradientsBasedOnWeights();

	void init(int _inputs, int _layers, int _perLayer, int _outputs);

	void trainNetwork();

	void addTrainingSet();

	void clearTrainingSets();

	void trainOnCachedSets();

	float test();

private:

	float absolute(float _x) {
		if (_x < 0) {
			return -_x;
		}
		return _x;
	}

};