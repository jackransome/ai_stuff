#include "Camera2D.h"
namespace GameEngine {

    Camera2D::Camera2D() : _position(0.0f, 0.0f),
		truePosition(0.0f, 0.0f),
        _cameraMatrix(1.0f),
        _orthoMatrix(1.0f),
        _scale(1.0f),
        _needsMatrixUpdate(true),
        _screenWidth(500),
        _screenHeight(500)
    {
    }


    Camera2D::~Camera2D()
    {
    }

    void Camera2D::init(int screenWidth, int screenHeight, int _x, int _y) {
        _screenWidth = screenWidth;
        _screenHeight = screenHeight;
        _orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
		screenShake.x = 0;
		screenShake.y = 0;
		truePosition.x = _x;
		truePosition.y = _y;
    }

    //updates the camera matrix if needed
    void Camera2D::update() {

        //Only update if our position or scale have changed
        if (_needsMatrixUpdate) {
            
            //Camera Translation
            glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
            _cameraMatrix = glm::translate(_orthoMatrix, translate);

            //Camera Scale
            glm::vec3 scale(_scale, _scale, 0.0f);
            _cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

            _needsMatrixUpdate = false;
        }
    }

	glm::vec2  Camera2D::getPosition() {
		return _position;
	}

	glm::vec2  Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
		//Make it so that 0,0 is the center
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		//Scale the Coords
		screenCoords /= _scale;
		//Translate with the camera position
		screenCoords += glm::vec2(_position.x, -_position.y);
		return screenCoords;
	}
	void Camera2D::followObject(BoundingBox* _bb) {
		if (truePosition.x < _bb->x) {
			xyVel.x = floor((_bb->x - truePosition.x) / 20);
		}
		if (truePosition.x > _bb->x) {
			xyVel.x = floor((_position.x - _bb->x) / -20);
		}
		if (truePosition.y < _bb->y ) {
			xyVel.y = floor(((_bb->y) - truePosition.y) / 20);
		}
		if (truePosition.y > _bb->y ) {
			xyVel.y = floor((truePosition.y - (_bb->y)) / -20);
		}
		truePosition.x += xyVel.x;
		truePosition.y += xyVel.y;
		
		//_position.x = floor(_position.x);
		//_position.y = floor(_position.y);
		_position = truePosition;
		_position += screenShake;
		_needsMatrixUpdate = true;
		addScreenShake();
		settleScreenShake();
		
	}
	void Camera2D::addScreenShake() {
		screenShake.x = ((double)rand() / (RAND_MAX)) * screenShakeIntensity*2 - screenShakeIntensity;
		screenShake.y = ((double)rand() / (RAND_MAX)) * screenShakeIntensity * 2 - screenShakeIntensity;
	}
	void Camera2D::settleScreenShake() {
		screenShakeIntensity *= 0.97;
		if (screenShakeIntensity > -1 && screenShakeIntensity < 1) {
			screenShakeIntensity = 0;
		}
	}
}
