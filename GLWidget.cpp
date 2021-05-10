#include "GLWidget.h"
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include "glm.hpp"
#include "gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include "primitives/Vertex.h"
#include "primitives/Geometry.h"
#include "Camera.h"
#include<QMouseEvent>
#include<QKeyEvent>

Camera camera;
extern const char* vShader1;
extern const char* fShader1;

GLWidget::GLWidget()
{
	xRot = 0;
	yRot = 0;
	zRot = 0;
    GLuint shader=0;
}

void GLWidget::sendDataToOpenGL()
{
	//Shape1
	ShapeData shape = Geometry::makePlane();
	glGenVertexArrays(1,&vertexArrayObjectID1);
	glGenVertexArrays(1,&vertexArrayObjectID2);

	glBindVertexArray(vertexArrayObjectID1);
	glGenBuffers(1, &vertexBufferID1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID1);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 6));

	numIndices1 = shape.numIndices;
	glGenBuffers(1, &indexBufferID1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	
	///Shape2

	shape = Geometry::makeArrow();
	glBindVertexArray(vertexArrayObjectID2);
	glGenBuffers(1, &vertexBufferID2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID2);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 6));

	numIndices2 = shape.numIndices;
	glGenBuffers(1, &indexBufferID2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	shape.cleanup();
}

void GLWidget::initializeGL()
{
	setMouseTracking(false);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
	modelToProjectionMatrixULoc = glGetUniformLocation(shader,"modelToProjectionMatrix");
}

void GLWidget::paintGL()
{
	glUseProgram(shader);
	glViewport(0,0,width(),height());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	
	
	GLint ambientLightUniformLoc =glGetUniformLocation(shader,"ambientLight");
	glm::vec4 ambientLight(0.05f,0.05f,0.05f,1.0f);
	glUniform4fv(ambientLightUniformLoc,1,&ambientLight[0]);
	GLint lightPositionWorldUniformLoc =glGetUniformLocation(shader,"lightPositionWorld");
	glm::vec3 lightPositionWorld(0.0f,-0.5f,0.0f);
	glUniform3fv(lightPositionWorldUniformLoc,1,&lightPositionWorld[0]);

	GLint modelToWorldMatrixUL =glGetUniformLocation(shader,"modelToWorldMatrix");

	glm::mat4 viewToProjectionMatrix =glm::perspective(60.0f,((float)width())/height(),0.1f,50.0f);		//projectionMatrix:paralel fov & trapez cube
	//glm::mat4 projectionMatrix = glm::ortho (0.0f, 8.0f, 0.0f, 6.0f, 0.1f, 100.0f);
	glm::mat4 worldToViewMatrix =camera.getWorldToViewMatrix();
	glm::mat4 worldToProjectionMatrix=viewToProjectionMatrix*worldToViewMatrix;
	//cube 1
	
	glBindVertexArray(vertexArrayObjectID1);
	
	glm::mat4 cubeModel1ToWorldMatrix=glm::translate(glm::vec3(-0.0f, -1.0f, -0.0f)) *glm::rotate(0.0f,glm::vec3(1.0f,0.0f,0.0f));
	glm::mat4 modelToProjectionMatrix = worldToProjectionMatrix*cubeModel1ToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionMatrixULoc,1,GL_FALSE,&modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUL,1,GL_FALSE,&cubeModel1ToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES,numIndices1,GL_UNSIGNED_SHORT,0);
	//cube 2
	glm::mat4 cubeModelToWorldMatrix=glm::translate(glm::vec3(1.0f, 0.0f, -3.75f))* glm::rotate(126.0f,glm::vec3(0.0f,1.0f,0.0f));		
	modelToProjectionMatrix = worldToProjectionMatrix*cubeModelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionMatrixULoc,1,GL_FALSE,&modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUL,1,GL_FALSE,&cubeModelToWorldMatrix[0][0]);
	//glDrawElements(GL_TRIANGLES,cubeNumIndices,GL_UNSIGNED_SHORT,0);
	
	//arrow
	glBindVertexArray(vertexArrayObjectID2);

	glm::mat4 arrowModelToWorldMatrix=glm::translate(glm::vec3(0.0f, 0.0f, -3.0f))*glm::rotate(xRot,glm::vec3(1.0f,0.0f,0.0f))*glm::rotate(yRot,glm::vec3(0.0f,1.0f,0.0f));		

	modelToProjectionMatrix = worldToProjectionMatrix*arrowModelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionMatrixULoc,1,GL_FALSE,&modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUL,1,GL_FALSE,&arrowModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES,numIndices2,GL_UNSIGNED_SHORT,0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{ lastPos = event->pos();}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
 GLfloat dx = (GLfloat) (event->x() - lastPos.x()) / width();
 GLfloat dy = (GLfloat) (event->y() - lastPos.y()) / height();
 setFocus();
 if (event->buttons() & Qt::LeftButton) {
	camera.mouseUpdate(glm::vec2(event->x(),event->y()));
	repaint();
 } else if (event->buttons() & Qt::RightButton) {
	/*xRot += 180 * dy;
	zRot += 180 * dx;*/
	xRot -= 180 * dy;
	yRot += 180 * dx;
	repaint();
	
 }
 lastPos = event->pos();
}

void GLWidget::keyPressEvent(QKeyEvent*e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_S:
		camera.moveBackward();
		break;
	case Qt::Key::Key_A:
		camera.strafeLeft();
		break;
	case Qt::Key::Key_D:
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	}
	repaint();
}

void GLWidget::resetFrame()
{
	xRot = 0;
	yRot = 0;
	zRot = 0;
	camera.resetCam();
	repaint();
}

void GLWidget::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) 
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}



std::string GLWidget::readFile(const char* fileLocation)
/*{
	std::ifstream meInput(fileName);
	if (!meInput.good())
	{
		std::cout<<"failed to read file"<<fileName;
	}

	return std::string(std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());

}*/

{
	std::string content;
	//std::ifstream fileStream(fileLocation, std::ios::in);
	std::ifstream fileStream(fileLocation);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}


void GLWidget::installShaders()
{
	shader = glCreateProgram();

	if (!shader) 
	{
		printf("Failed to create shader\n");
		return;
	}
	/*std::string temp = readFile("VertexShaderCode.glsl");
	const char* vShader1 =temp.c_str();
	 temp = readFile("FragmentShaderCode.glsl");
	 const char* fShader1 =temp.c_str();*/

	AddShader(shader, vShader1, GL_VERTEX_SHADER);
	AddShader(shader, fShader1, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

}
