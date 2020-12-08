#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "BoundingBox.h"
namespace GameEngine {

    //Camera class for 2D
    class Camera2D
    {
    public:
        Camera2D();
        ~Camera2D();

        //sets up the orthographic matrix and screen dimensions
        void init(int screenWidth, int screenHeight, int _x, int _y);

        //updates the camera matrix if needed
        void update();

		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

        void setPosition(const glm::vec2& newPosition) { _position = newPosition; _needsMatrixUpdate = true; }
        void setScale(float newScale) { _scale = newScale; _needsMatrixUpdate = true; }
		
		glm::vec2 getPosition();
        float getScale() { return _scale; }
        glm::mat4 getCameraMatrix() { return _cameraMatrix; }
		void followObject(BoundingBox* _bb);
		glm::vec2 getVelocity() { return xyVel; };
		void setScreenShakeIntensity(float _intensity) { screenShakeIntensity = _intensity; };
		void settleScreenShake();
		void addScreenShake();
    private:
        int _screenWidth, _screenHeight;
        bool _needsMatrixUpdate;
        float _scale;
		float vel;
		float screenShakeIntensity;
		glm::vec2 truePosition;
        glm::vec2 _position;
        glm::mat4 _cameraMatrix;
        glm::mat4 _orthoMatrix;
		glm::vec2 xyVel;
		glm::vec2 screenShake;
		
    };

}