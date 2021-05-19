#pragma once
#include "Camera.h"
#include<gtx\transform.hpp>


Camera::Camera():m_ViewDirection(0.7f, -0.5f, 0.5f),
	m_Position(-5.2f, 2.2f, -5.0f),m_CameraSpeed(0.1f),
	m_UP(0.0f, 1.0f, 0.0f)
{
}

/// Returns the Camera view matrix
glm::mat4 Camera::getWorldToViewMatrix()
{
	return glm::lookAt(m_Position,m_Position+m_ViewDirection,m_UP);
}
/// Changes in mouse position leading to rotation of the camera & sets the view direction for pan & zoom 
///@param newMousePosition : position of mouse on the projected screen
///@param mouseDelta : change in the mouse position
void  Camera::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition-m_OldMousePosition;
	if(glm::length(mouseDelta)>=50.0f)
	{
		m_OldMousePosition= newMousePosition;
		return;
	}
	const float speed = 5.0f;
	m_PanDirection = glm::cross(m_ViewDirection,m_UP); ///right hand direction to pan
	glm::mat4 rotator = glm::rotate(-mouseDelta.x/speed,m_UP)*
						glm::rotate(-mouseDelta.y/speed,m_PanDirection);
	m_ViewDirection =glm::mat3(rotator) *m_ViewDirection;
	
	m_OldMousePosition= newMousePosition;
}
///Moves camera forward
void Camera::moveForward()
{
	m_Position += m_CameraSpeed * m_ViewDirection;
}
///Moves camera backward
void Camera::moveBackward()
{
	m_Position += -m_CameraSpeed * m_ViewDirection;
}
///Pans the camera to the left
void Camera::panLeft()
{
	m_Position += -m_CameraSpeed * m_PanDirection;
}
///Pans the camera to the right

void Camera::panRight()
{
	m_Position += m_CameraSpeed * m_PanDirection;
}
///Pans the camera to the up
void Camera::moveUp()
{
	m_Position += m_CameraSpeed * m_UP;
}
///Pans the camera to the down
void Camera::moveDown()
{
	m_Position += -m_CameraSpeed * m_UP;
}
///Resets the camera to its initial position
void Camera::resetCam()
{
	m_ViewDirection=glm::vec3(0.7f, -0.5f, 0.5f);
	m_Position=glm::vec3(-5.2f, 2.2f, -5.0f);
	
}
