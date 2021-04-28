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
		//init();
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
	//sinceFirstMove++;
	if (getWinner() >= 0) {
		//addTrainingResult(getWinner());
		//init();
		//if (numberOfTrainingBoards >= 100) {
			//timeToTrain = true;
		//}
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

double* Tictactoe::convertBoard(int _board[3][3])
{
	double* convertedBoard = (double*)malloc(18 * sizeof(double));

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 1) {
				convertedBoard[i * 3 + j] = 1;
				convertedBoard[i * 3 + j + 9] = 0;
			}
			else if (board[i][j] == 2) {
				convertedBoard[i * 3 + j + 9] = 1;
				convertedBoard[i * 3 + j] = 0;
			}
			else {
				convertedBoard[i * 3 + j + 9] = 0;
				convertedBoard[i * 3 + j] = 0;
			}
		}
	}
	return convertedBoard;
}

void Tictactoe::flipTrainingBoards(){
	for (int i = 0; i < numberOfTrainingBoards; i++) {
		// change 1s to 2s and 2s to 1s
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				if (trainingBoards[i].board[j][k] == 1) {
					trainingBoards[i].board[j][k] = 2;
				}
				if (trainingBoards[i].board[j][k] == 2) {
					trainingBoards[i].board[j][k] = 1;
				}
			}
		}
	}
}

void Tictactoe::printBoard(){
	for (int i = 0; i < 3; i++) {
		std::cout << "\n";
		for (int j = 0; j < 3; j++) {
			std::cout << board[i][j] << " ";
		}
	}
	
}

void Tictactoe::flipBoard(){
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 1) {
				board[i][j] = 2;
			} else if (board[i][j] == 2) {
				board[i][j] = 1;
			}
		}
	}
	if (nextTurn == 1) {
		nextTurn = 2;
	}
	else {
		nextTurn = 1;
	}
}

void Tictactoe::logBoard() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			trainingBoards[numberOfTrainingBoards].board[i][j] = board[i][j];
		}
	}
	numberOfTrainingBoards++;
	if (numberOfTrainingBoards == 9) {
		//std::cout << "MAX NUMBER OF TRAINING BOARDS REACHED" << std::endl;
	}
}

void Tictactoe::resetTrainingBoards(){
	numberOfTrainingBoards = 0;
}
