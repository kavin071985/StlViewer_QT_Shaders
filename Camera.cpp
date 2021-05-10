#pragma once
#include "Camera.h"
#include<gtx\transform.hpp>

const float Camera::MOVEMENT_SPEED = 0.1f;
Camera::Camera():ViewDirection(0.7f, -0.5f, 0.5f),
	position(-5.2f, 2.2f, -5.0f),
	UP(0.0f, 1.0f, 0.0f)
{
}


glm::mat4 Camera::getWorldToViewMatrix()
{
	return glm::lookAt(position,position+ViewDirection,UP);
}
void  Camera::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition-oldMousePosition;
	if(glm::length(mouseDelta)>=50.0f)
	{
		oldMousePosition= newMousePosition;
		return;
	}
	const float speed = 5.0f;
	panDirection = glm::cross(ViewDirection,UP);
	glm::mat4 rotator = glm::rotate(-mouseDelta.x/speed,UP)*
						glm::rotate(-mouseDelta.y/speed,panDirection);
	ViewDirection =glm::mat3(rotator) *ViewDirection;
	
	oldMousePosition= newMousePosition;
}

void Camera::moveForward()
{
	position += MOVEMENT_SPEED * ViewDirection;
}

void Camera::moveBackward()
{
	position += -MOVEMENT_SPEED * ViewDirection;
}

void Camera::strafeLeft()
{
	position += -MOVEMENT_SPEED * panDirection;
}

void Camera::strafeRight()
{
	position += MOVEMENT_SPEED * panDirection;
}

void Camera::moveUp()
{
	position += MOVEMENT_SPEED * UP;
}

void Camera::moveDown()
{
	position += -MOVEMENT_SPEED * UP;
}

void Camera::resetCam()
{
	ViewDirection=glm::vec3(0.7f, -0.5f, 0.5f);
	position=glm::vec3(-5.2f, 2.2f, -5.0f);
	
}
