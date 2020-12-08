
#include <math.h>
#include <iostream>
#include <string>
#include <thread>

#include <GameEngine/Errors.h>
#include <GameEngine/ResourceManager.h>

#include "MainGame.h"


//Constructor, just initializes private member variables
MainGame::MainGame() :
	_screenWidth(1920),
	_screenHeight(1080),
	_time(0.0f),
	_gameState(GameState::PLAY),
	_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight, 0, 0);
}

MainGame::~MainGame()
{
}

//This runs the game
void MainGame::run() {
	initSystems();


	//This only returns when the game ends
	gameLoop();

	sound.unloadAll();
}
//Initialize SDL and Opengl and whatever else we need
void MainGame::initSystems() {
	srand(time(NULL));
	GameEngine::init();

	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();

	spriteBatch.init();
	drawText.init(&spriteBatch);
	_fpsLimiter.init(_maxFPS);
	_camera.setScreenShakeIntensity(0);

	nn = NN();
	nn.init();
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.addAttribute("test");
	_colorProgram.linkShaders();
}

//This is the main game loop for our program
void MainGame::gameLoop() {

	//Will loop until we set _gameState to EXIT
	while (_gameState != GameState::EXIT) {

		_fpsLimiter.begin();

		processInput();
		_time += 0.1;


		_camera.update();
		//std::cout << _fps << std::endl;
		updateGame();

		drawGame();

		_fps = _fpsLimiter.end();

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 1000) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
	}
}
void MainGame::updateGame() {

	glm::vec2 mousePos = _inputManager.getMouseCoords();
	glm::vec2 cameraPos = _camera.getPosition();

	_inputManager.lastMouseR = _inputManager.isKeyPressed(SDL_BUTTON_RIGHT);
	_inputManager.lastMouseL = _inputManager.isKeyPressed(SDL_BUTTON_LEFT);
	_inputManager.lastMouseM = _inputManager.isKeyPressed(SDL_BUTTON_MIDDLE);
}
//Processes input with SDL
void MainGame::processInput() {
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	glm::vec2 cameraPos = _camera.getPosition();

	glm::vec2 trueMouseCoords = _inputManager.getTrueMouseCoords();
	_inputManager.setMouseCoords(trueMouseCoords.x + cameraPos.x - _screenWidth / 2, -(trueMouseCoords.y - cameraPos.y - _screenHeight / 2));

	_inputManager.lastMouseR = _inputManager.isKeyPressed(SDL_BUTTON_RIGHT);
	_inputManager.lastMouseL = _inputManager.isKeyPressed(SDL_BUTTON_LEFT);
	_inputManager.lastMouseM = _inputManager.isKeyPressed(SDL_BUTTON_MIDDLE);

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			cameraPos = _camera.getPosition();
			_inputManager.setTrueMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
	if (_inputManager.isKeyPressed(SDLK_f)) {
		nn.init();
	}
}

//Draws the game using the almighty OpenGL
void MainGame::drawGame() {
	//Set the base depth to 1.0
	glClearDepth(1.0);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLineWidth(1);

	glm::mat4 camera = _camera.getCameraMatrix();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(-1, -1);
	glVertex2f(-1, 1);
	glVertex2f(1, 1);
	glVertex2f(1, -1);
	glEnd();
	//Enable the shader
	_colorProgram.use();
	//We are using texture unit 0
	glActiveTexture(GL_TEXTURE0);
	//Get the uniform location
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//Tell the shader that the texture is in texture unit 0
	glUniform1i(textureLocation, 0);

	//Set the camera matrixx
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	spriteBatch.begin();

	GameEngine::Color colour;
	colour.r = 10;
	colour.g = 255;
	colour.b = 10;
	//spriteBatch.draw(glm::vec4(50, 50, 50, 50), glm::vec4(0, 0, 0, 0), NULL, 1, colour);
	//spriteBatch.drawLine(glm::vec2(0,0), glm::vec2(100, 100), 60, 30, 220, NULL, 1);

	//DRAW THE NETWORK:
	//(STILL HAVE TO COPY IN THE NN CODE FROM THE OTHER PROJ)
	nn.trainNetwork();
	for (int i = 0; i < layers - 1; i++) {
		for (int j = 0; j < perLayer; j++) { //the previous
			for (int k = 0; k < perLayer; k++) { //the current
				if (!(nn.nodes[i][j].exists && nn.nodes[i + 1][k].exists)) {
					//spriteBatch.drawLine(glm::vec2(i * 100, j * 10 - 30), glm::vec2(i * 100 + 100, k * 10 - 30), 255, 0, 0, NULL, 1);
				}
				else {
					if (nn.connections[i][j][k] > 1) {
						spriteBatch.drawLine(glm::vec2(i * 100, j * 10 - 30), glm::vec2(i * 100 + 100, k * 10 - 30), 0/*nn.dErrorDConnections[i][j][k] * 255*/, 255, 0, NULL, 1);
					}
					else {
						spriteBatch.drawLine(glm::vec2(i * 100, j * 10 - 30), glm::vec2(i * 100 + 100, k * 10 - 30), 0/*nn.dErrorDConnections[i][j][k] * 255*/, nn.connections[i][j][k] * 255, 0, NULL, 1);
					}
				}
				
			}

		}
	}
	for (int i = 0; i < layers; i++) {
		for (int j = 0; j < perLayer; j++) { //the previous
			for (int k = 0; k < perLayer; k++) { //the current
				if (i == 0 && k == inputs) break;
				if (i == layers - 1 && k == outputs) break;
				//spriteBatch.drawLine(glm::vec2(i * 100, j * 100 + - 500), glm::vec2(i * 100 + 100, k * 100 - 500), nn.dErrorDConnections[i][j][k] * 255, 0, 0, NULL, 1);
			}

		}
	}
	colour.r = nn.test() * 5;
	colour.g = 0;
	colour.b = 0;
	spriteBatch.draw(glm::vec4(-300, 50, 50, 50), glm::vec4(0, 0, 0, 0), NULL, 1, colour);

	spriteBatch.end();

	spriteBatch.renderBatch();

	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);


	//disable the shader
	_colorProgram.unuse();

	//Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}