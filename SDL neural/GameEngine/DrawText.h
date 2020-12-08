#pragma once
#include "GLTexture.h"
#include "SpriteBatch.h"
#include "ResourceManager.h"
#include <string>
namespace GameEngine {
	class DrawText
	{
	public:
		DrawText();
		~DrawText();
		void draw(float _x, float _y, int _index, float _scale);
		void draw(float _x, float _y, std::string _string, float _scale);
		void drawString(float _x, float _y, std::string _string, float _scale);
		void init(GameEngine::SpriteBatch* _sb);
		void drawAll(float _x, float _y, float _scale);
	private:
		GameEngine::GLTexture texture;
		GameEngine::SpriteBatch* sb;
		glm::vec2 getUVX(int _index);
		int getIndexFromString(std::string _string);
		int getIndexFromString(int _int);
	};
}