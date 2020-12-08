#include "DrawText.h"
#include <sstream>


namespace GameEngine {
	DrawText::DrawText()
	{
	}


	DrawText::~DrawText()
	{
	}
	void DrawText::init(GameEngine::SpriteBatch* _sb) {
		texture = GameEngine::ResourceManager::getTexture("Textures/lettersX2.png");
		sb = _sb;
	}

	glm::vec2 DrawText::getUVX(int _index) {
		int width;
		int uvx;
		if (_index < 12) {
			width = 3;
			uvx = _index * 4;
		}
		else if (_index == 12) {
			width = 5;
			uvx = _index * 4;
		}
		else if (_index == 13) {
			width = 4;
			uvx = 54;
		}
		else if (_index < 16) {
			width = 3;
			uvx = 59 + (_index - 14) * 4;
		}
		else if (_index == 16) {
			width = 4;
			uvx = 67;
		}
		else if (_index < 22) {
			width = 3;
			uvx = 72 + (_index - 17) * 4;
		}
		else if (_index == 22) {

			width = 5;
			uvx = 92;
		}
		else if (_index == 27) {
			width = 2;
			uvx = 97 + (_index - 23) * 4;
		} else {
			width = 3;
			uvx = 97 + (_index - 23) * 4;
		}
		return glm::vec2(uvx, width);
	}
	void DrawText::draw(float _x, float _y, int _int, float _scale) {
		int test = getIndexFromString(_int);
		GameEngine::Color color;
		color.r = 10;
		color.g = 10;
		color.b = 10;
		color.a = 255;
		sb->draw(glm::vec4(_x, _y, 7.0f * _scale, 10.0f * _scale), glm::vec4(7.0f*test / 252.0f, 0, 7.0f / 252.0f, 1), texture.id, 0.0f, color, -1);
	}
	void DrawText::draw(float _x, float _y, std::string _string, float _scale) {
		int test = getIndexFromString(_string);
		GameEngine::Color color;
		color.r = 10;
		color.g = 10;
		color.b = 10;
		color.a = 255;
		sb->draw(glm::vec4(_x, _y, 7.0f * _scale, 10.0f * _scale), glm::vec4(7.0f*test / 252.0f, 0, 7.0f / 252.0f, 1), texture.id, 0.0f, color, -1);
	}

	void DrawText::drawString(float _x, float _y, std::string _string, float _scale)
	{
		for (int i = 0; i < _string.size(); i++)
		{
			draw(_x + i*(7 * _scale + 2), _y, std::string(1, _string[i]), _scale);
		}
	}

	void DrawText::drawAll(float _x, float _y, float _scale) {
		for (int i = 0; i < 36; i++) {
			GameEngine::Color color;
			color.r = 10;
			color.g = 10;
			color.b = 10;
			color.a = 255;
			sb->draw(glm::vec4(_x+(i*_scale*9), _y, 7.0f * _scale, 10.0f * _scale), glm::vec4(7.0f*i / 252.0f, 0, 7.0f / 252.0f, 1), texture.id, 0.0f, color, -1);
		}
	}
	int DrawText::getIndexFromString(int _int) {
		return 26 + _int;
	}
	int DrawText::getIndexFromString(std::string _string) {
		if (_string == "a") {
			return 0;
		} else 	if (_string == "b") {
			return 1;
		}	else	if (_string == "c") {
			return 2; 
		}	else	if (_string == "d") {
			return 3;
		}	else	if (_string == "e") {
			return 4;
		}	else	if (_string == "f") {
			return 5;
		}	else	if (_string == "g") {
			return 6;
		}	else	if (_string == "h") {
			return 7;
		}	else	if (_string == "i") {
			return 8;
		}	else	if (_string == "j") {
			return 9;
		}	else	if (_string == "k") {
			return 10;
		}	else	if (_string == "l") {
			return 11;
		}	else	if (_string == "m") {
			return 12;
		}	else	if (_string == "n") {
			return 13;
		}	else	if (_string == "o") {
			return 14;
		}	else	if (_string == "p") {
			return 15;
		}	else	if (_string == "q") {
			return 16;
		}	else	if (_string == "r") {
			return 17;
		}	else	if (_string == "s") {
			return 18;
		}	else	if (_string == "t") {
			return 19;
		}	else	if (_string == "u") {
			return 20;
		}	else	if (_string == "v") {
			return 21;
		}	else	if (_string == "w") {
			return 22;
		}	else	if (_string == "x") {
			return 23;
		}	else	if (_string == "y") {
			return 24;
		}	else	if (_string == "z") {
			return 25;
		}	else 	if (_string == "0") {
			return 26;
		}	else	if (_string == "1") {
			return 27;
		}	else	if (_string == "2") {
			return 28;
		}	else	if (_string == "3") {
			return 29;
		}	else	if (_string == "4") {
			return 30;
		}	else	if (_string == "5") {
			return 31;
		}	else	if (_string == "6") {
			return 32;
		}	else	if (_string == "7") {
			return 33;
		}	else	if (_string == "8") {
			return 34;
		}	else	if (_string == "9") {
			return 35;
		}
	}
}
