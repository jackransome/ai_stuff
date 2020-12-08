#pragma once
#include "GLTexture.h"
#include "SpriteBatch.h"
#include "ResourceManager.h"
namespace GameEngine {
	class SpriteSheet
	{
	public:
		SpriteSheet();
		~SpriteSheet();
		void init(GameEngine::SpriteBatch* _sb, int _width, int _height, int _scale, int _numberOfFrames, int _frameChangeInterval, int offSetX, int offSetY);
		void loadTexture(std::string _path);
		void run();
		void draw(float _x, float _y);
		void draw(float _x, float _y, float _angle);
		void draw(float _x, float _y, float _angle, float _lightAlpha);
		void draw(float _x, float _y, float _angle, bool _flip);
		void setFrame(int _frame);
		void reset();
	private:
		int frame = 0;
		int numberOfFrames;
		int frameChangeInterval;
		int frameChangeCounter = 0;
		GameEngine::GLTexture texture;
		float width;
		float height;
		float scale;
		GameEngine::SpriteBatch* sb;
		int offSetX;
		int offSetY;
	};

}