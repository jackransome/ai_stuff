#pragma once
#include "TrainingBoard.h"

class Tictactoe {
public:
	TrainingBoard trainingBoards[9];
	int sinceFirstMove;
	int numberOfTrainingBoards;
	int numberOfPossibleMoves;
	bool timeToTrain;
	int winningMove;
	//1s and 2s, 1s go first
	int board[3][3];
	//same format at board but boards that are invalid have a -1 at [0][0]
	int possibleMoves[9][3][3];

	Tictactoe();

	void init();

	void addTrainingResult(int winner);

	int nextTurn;

	void logBoard();

	void resetTrainingBoards();

	int*** getPossibleMoves();

	int makeMove(int _index);

	int getWinner();

	double* convertBoard(int _board[3][3]);

	void flipTrainingBoards();

	void printBoard();

	//(╯°□°)╯︵ ┻━┻
	void flipBoard();

};