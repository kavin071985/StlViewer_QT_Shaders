#pragma once
#include<glm.hpp>
///Camera class to zoom, pan & rotate the view matrix
///@param m_Position : position of the camera
///@param m_ViewDirection : direction which the camera points
///@param m_UP: up vector of the camera
///@param m_OldMousePosition : Records the mouse coordinates
///@param m_CameraSpeed: speed at which the camera flies
///@param m_PanDirection: perpendicular vector to UP& view direction using cross product
class Camera
{
	glm::vec3 m_Position;
	glm:: vec3 m_ViewDirection;
	const glm:: vec3 m_UP;
	glm::vec2 m_OldMousePosition;
	const float m_CameraSpeed;
	glm::vec3 m_PanDirection;
public:
	Camera();
	glm::mat4 getWorldToViewMatrix();
	void mouseUpdate(const glm::vec2& newMousePosition);
	void moveForward();
	void moveBackward();
	void panLeft();
	void panRight();
	void moveUp();
	void moveDown();	
	void resetCam();
};

