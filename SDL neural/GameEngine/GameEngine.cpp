#include <SDL.h>
#include <SDL_mixer.h>
#include <GL/glew.h>
#include "GameEngine.h"
#include <iostream>
namespace GameEngine {
	int init() {
		//Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		//Tell SDL that we want a double buffered window so we dont get
		//any flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
		return 0;
	}
	void drawRect(int x, int y, int width, int height, float _lightAlpha, Color color, SpriteBatch* spriteBatch) {
		spriteBatch->draw(glm::vec4(x, y, width, height), glm::vec4(1, 1, 1, 1), NULL, 0.0f, color, _lightAlpha);
	}
	void drawRect(int x, int y, int width, int height, Color color, SpriteBatch* spriteBatch) {
		//spriteBatch->draw(glm::vec4(x, y, width, height), glm::vec4(1, 1, 1, 1), NULL, 0.0f, color, _lightAlpha);
		spriteBatch->draw(glm::vec4(x, y, width, height), glm::vec4(1, 1, 1, 1), NULL, 1.0f, color);
	}
	void drawLine(glm::vec2 point1, glm::vec2 point2, glm::mat4 cameraMatrix) {
		glBegin(GL_LINES);
		glm::vec4 test1 = cameraMatrix * glm::vec4(point1.x, point1.y, 1, 1);
		glVertex3f(test1.x, test1.y, 0);
		glm::vec4 test = cameraMatrix * glm::vec4(point2.x, point2.y, 1, 1);
		glVertex3f(test.x, test.y, 0);
		glEnd();
	}
	void drawBasicLine(glm::vec2 _point1, glm::vec2 _point2, float _thickness, float _r, float _g, float _b) {
		glLineWidth(_thickness);
		glColor3f(_r, _g, _b);
		glBegin(GL_LINES);
		glVertex3f(_point1.x, _point1.y, 0.0);
		glVertex3f(_point2.x, _point2.y, 0.0);
		glEnd();
	}
	void drawBasicLine(glm::vec3 _point1, glm::vec3 _point2, float _thickness, float _r, float _g, float _b) {
		glLineWidth(_thickness);
		glColor3f(_r, _g, _b);
		glBegin(GL_LINES);
		float max = 1000;
		float min = 0.9;
		glVertex3f(_point1.x, _point1.y, -1-(1 / (_point1.z - 0.1) - 1 / min) / (1 / max + 1 / min));
		glVertex3f(_point2.x, _point2.y, -1-(1 / (_point2.z - 0.1) - 1 / min) / (1 / max + 1 / min));
		glEnd();
	}
	void fillBackGround(float _r, float _g, float _b) {
		glColor3f(_r, _g, _b);
		glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0.0);
		glVertex3f(1, -1, 0.0);
		glVertex3f(1, 1, 0.0);
		glVertex3f(-1, 1, 0.0);
		glEnd();
	}
	void drawBasicQuad(glm::vec2 _p1, glm::vec2 _p2, glm::vec2 _p3, glm::vec2 _p4, float _r, float _g, float _b) {
		glColor3f(_r, _g, _b);
		glBegin(GL_POLYGON);
		glVertex3f(_p1.x, _p1.y, 0.0);
		glVertex3f(_p2.x, _p2.y, 0.0);
		glVertex3f(_p3.x, _p3.y, 0.0);
		glVertex3f(_p4.x, _p4.y, 0.0);
		glEnd();
		drawBasicLine(_p1, _p2, 2, 0, 0, 0);
		drawBasicLine(_p2, _p3, 2, 0, 0, 0);
		drawBasicLine(_p3, _p4, 2, 0, 0, 0);
		drawBasicLine(_p4, _p1, 2, 0, 0, 0);
	}
	void drawBasicQuad(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3, glm::vec3 _p4, float _r, float _g, float _b) {
		glColor3f(_r, _g, _b);
		glBegin(GL_POLYGON);
		glVertex3f(_p1.x, _p1.y, 0.0);
		glVertex3f(_p2.x, _p2.y, 0.0);
		glVertex3f(_p3.x, _p3.y, 0.0);
		glVertex3f(_p4.x, _p4.y, 0.0);
		glEnd();
		drawBasicLine(_p1, _p2, 1.5, 0, 0, 0);
		drawBasicLine(_p2, _p3, 1.5, 0, 0, 0);
		drawBasicLine(_p3, _p4, 1.5, 0, 0, 0);
		drawBasicLine(_p4, _p1, 1.5, 0, 0, 0);
	}
	void drawBasicTriangle(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3, float _r, float _g, float _b)
	{
		glColor4f(_r, _g, _b, 1);
		glBegin(GL_TRIANGLES);

		float max = 1000;
		float min = 0.9;
		glVertex3f(_p1.x, _p1.y, -1-(1/_p1.z - 1/min) / (1/max + 1/min));
		glVertex3f(_p2.x, _p2.y, -1 - (1 / _p2.z - 1 / min) / (1 / max + 1 / min));
		glVertex3f(_p3.x, _p3.y, -1-(1 / _p3.z - 1 / min) / (1 / max + 1 / min));
		//std::cout << -(1 / _p3.z - 1 / min) / (1 / max + 1 / min) << std::endl;
		glEnd();
		//drawBasicLine(_p1, _p2, 1.5, 0, 0, 0);
		//drawBasicLine(_p2, _p3, 1.5, 0, 0, 0);
		//drawBasicLine(_p3, _p1, 1.5, 0, 0, 0);
	}
}