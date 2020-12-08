#pragma once

#include <unordered_map>
#include <glm.hpp>

namespace GameEngine {

// Input manager stores a key map that maps SDL_Keys to booleans.
// If the value in the key map is true, then the key is pressed.
// Otherwise, it is released.
class InputManager
{
public:
    InputManager();
    ~InputManager();

    void pressKey(unsigned int keyID);
    void releaseKey(unsigned int keyID);

	void setMouseCoords(float x, float y);
	void setTrueMouseCoords(float x, float y);
	void addMouseCoords(glm::vec2 _pos);
    bool isKeyPressed(unsigned int keyID);
	bool lastMouseL;
	bool lastMouseR;
	bool lastMouseM;
	//getters
	glm::vec2 getMouseCoords();
	glm::vec2 getTrueMouseCoords();
private:
    std::unordered_map<unsigned int, bool> _keyMap;
	glm::vec2 _mouseCoords;
	glm::vec2 _trueMouseCoords;
};

}