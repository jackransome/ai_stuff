#pragma once
#include <glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
namespace GameEngine {
	struct Light {
		float x;
		float y;
		float radius;
		glm::vec3 color;
		int ID;
		bool flicker;
		glm::vec2 flickerRange;
	};
	extern int init();
	extern void drawRect(int x, int y, int width, int height, float _lightAlpha, Color color, SpriteBatch* spriteBatch);
	extern void drawRect(int x, int y, int width, int height, Color color, SpriteBatch* spriteBatch);
	extern void drawLine(glm::vec2 point1, glm::vec2 point2, glm::mat4 cameraMatrix);
	extern void drawBasicLine(glm::vec2 _point1, glm::vec2 _point2, float _thickness, float _r, float _g, float _b);
	extern void drawBasicLine(glm::vec3 _point1, glm::vec3 _point2, float _thickness, float _r, float _g, float _b);
	extern void fillBackGround(float _r, float _g, float _b);
	extern void drawBasicQuad(glm::vec2 _p1, glm::vec2 _p2, glm::vec2 _p3, glm::vec2 _p4, float _r, float _g, float _b);
	extern void drawBasicQuad(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3, glm::vec3 _p4, float _r, float _g, float _b);
	extern void drawBasicTriangle(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3, float _r, float _g, float _b);
}
