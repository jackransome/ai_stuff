#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <vector>

#include <GameEngine/GameEngine.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/GLTexture.h>
#include <GameEngine/Sprite.h>
#include <GameEngine/Window.h>
#include <GameEngine/InputManager.h>
#include <GameEngine/Timing.h>
#include <GameEngine/CollisionDetection.h>
#include <GameEngine/SpriteBatch.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/DrawText.h>
#include <GameEngine/Sound.h>
#include "NN.h"
#include "Tictactoe.h"

enum class GameState { PLAY, EXIT };

//
enum Mode { ttt, graph };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
private:

	Mode mode = graph;

	NN nn;

	Tictactoe tictactoe;

	//oscilates from 0 to 20
	int graphPosition;

	//true = up, false = down
	bool graphDirection;

	double nnMemory;

	float learningRate = 0.000001;

	int gradientGraph[100];
	int gradientIndex = 0;
	float errorGraph[100];
	int errorIndex = 0;
	int closestNodeToPlayerIndex;

	bool evaluating = false;
	bool drawing = false;

	bool playerNodeHasChanged;

	std::vector<glm::vec2> tempPath;

	GameEngine::CollisionDetection collisionDetection;
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	void updateGame();
	GameEngine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	GameEngine::GLSLProgram _colorProgram;
	GameEngine::Camera2D _camera;

	GameEngine::SpriteBatch spriteBatch;

	GameEngine::DrawText drawText;

	GameEngine::InputManager _inputManager;
	GameEngine::FpsLimiter _fpsLimiter;

	GameEngine::Sound sound;

	float _maxFPS;
	float _fps;
	float _time;
	bool lastPressedL = false;
	bool lastPressedR = false;
	bool lastg = false;
};
