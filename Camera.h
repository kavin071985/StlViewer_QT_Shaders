#pragma once
#include<glm.hpp>
class Camera
{
	glm::vec3 position;
	glm:: vec3 ViewDirection;
	const glm:: vec3 UP;
	glm::vec2 oldMousePosition;
	static const float MOVEMENT_SPEED;
	glm::vec3 panDirection;
public:
	Camera();
	glm::mat4 getWorldToViewMatrix();
	void mouseUpdate(const glm::vec2& newMousePosition);
	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();
	void moveUp();
	void moveDown();	
	void resetCam();
};

