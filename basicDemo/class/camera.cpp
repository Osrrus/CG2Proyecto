#define GLM_ENABLE_EXPERIMENTAL

#include "camera.h"
#include <glm/gtx/transform.hpp>

const float MOVEMENT_SPEED = 0.1f;
const float ROTATIONAL_SPEED = 0.006f;
const float SENSITIVITY = 0.1f;

Camera::Camera() :
	viewDirection(0.0f, 0.0f, -1.0f),
	position(0.0f,0.0f,3.0f),
	oldMousePosition(0.0f,0.0f),
	UP(0.0f, 1.0f, 0.0f){}

void Camera::mouseUpdate(const glm::vec2& newMousePosition) {

	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 50.0f) {
		oldMousePosition = newMousePosition;
		return;
	}
	
	glm::vec3 toRotateAround = glm::cross(viewDirection, UP);
	glm::mat4 rotator = glm::rotate(-mouseDelta.x*ROTATIONAL_SPEED, UP) *
						glm::rotate(-mouseDelta.y*ROTATIONAL_SPEED, toRotateAround);

	viewDirection = glm::mat3(rotator) * viewDirection;

	oldMousePosition = newMousePosition;
}

glm::mat4 Camera::getWorlToViewMatrix() const{

	return glm::lookAt(position,position + viewDirection, UP);

}

void Camera::moveForward() {

	position += MOVEMENT_SPEED * viewDirection;

}

void Camera::moveBackward() {

	position += -MOVEMENT_SPEED * viewDirection;

}

void Camera::strafeLeft() {

	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position += -MOVEMENT_SPEED * strafeDirection;
}

void Camera::strafeRight() {

	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position += MOVEMENT_SPEED * strafeDirection;
}

void Camera::moveUp() {

	position += MOVEMENT_SPEED * UP;

}

void Camera::moveDown() {

	position += -MOVEMENT_SPEED * UP;

}
