#include "Tictactoe.h"
#include <iostream>
Tictactoe::Tictactoe() {
	init();
	numberOfTrainingBoards = 0;
	timeToTrain = false;
}

void Tictactoe::init() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = 0;
		}
	}
	nextTurn = 1;
	
	sinceFirstMove = 0;
}

void Tictactoe::addTrainingResult(int winner){
	int i = 1;
	while (sinceFirstMove > 0) {
		if (numberOfTrainingBoards - i < 0 || numberOfTrainingBoards - i > numberOfTrainingBoards - 1) {
			int e = 0;
		}
		trainingBoards[numberOfTrainingBoards - i].winner = winner;
		trainingBoards[numberOfTrainingBoards - i].movesFromWin = i;
		i++;
		sinceFirstMove--;
	}
}

int *** Tictactoe::getPossibleMoves(){
	bool isFilled = true;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0) {
				isFilled = false;
				for (int k = 0; k < 3; k++) {
					for (int l = 0; l < 3; l++) {
						possibleMoves[i * 3 + j][k][l] = board[k][l];
					}
				}
				possibleMoves[i * 3 + j][i][j] = nextTurn;
			}
			else {
				possibleMoves[i * 3 + j][0][0] = -1;
			}
		}
	}
	if (isFilled) {
		init();
	}
	return (int***)possibleMoves;
}

int Tictactoe::makeMove(int _index){
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = possibleMoves[_index][i][j];
			//std::cout << board[i][j];
		}
		//std::cout << "\n";
	}
	if (nextTurn == 1) {
		nextTurn = 2;
	}
	else {
		nextTurn = 1;
	}
	//std::cout << "\n";
	sinceFirstMove++;
	logBoard();
	if (getWinner() >= 0) {
		addTrainingResult(getWinner());
		init();
		if (numberOfTrainingBoards >= 100) {
			timeToTrain = true;
		}
	}
	return getWinner();
}

int Tictactoe::getWinner() {
	if (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][0] != 0) {
		return board[0][0];
	}
	if (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][0] != 0) {
		return board[1][0];
	}
	if (board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][0] != 0) {
		return board[2][0];
	}
	if (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[0][0] != 0) {
		return board[0][0];
	}
	if (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[0][1] != 0) {
		return board[0][1];
	}
	if (board[0][2] == board[1][2 ]&& board[1][2] == board[2][2] && board[0][2] != 0) {
		return board[0][2];
	}
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0) {
		return board[0][0];
	}
	if (board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[2][0] != 0) {
		return board[2][0];
	}
	//making sure board isnt filled
	bool filled = true;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0) {
				filled = false;
			}
		}
	}
	//return 0 if filled and nobody has won as it must be a tie
	if (filled) {
		return 0;
	}
	//otherwise return -1 because nobody has won
	return -1;
}

void Tictactoe::logBoard() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			trainingBoards[numberOfTrainingBoards].board[i][j] = board[i][j];
		}
	}
	numberOfTrainingBoards++;
	if (numberOfTrainingBoards == 109) {
		numberOfTrainingBoards = 0;
	}
}