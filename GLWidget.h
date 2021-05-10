
#ifndef WINDOW_H
#define WINDOW_H
#include "GL\glew.h"
#include<QGLWidget>
#include "primitives\Geometry.h"

class GLWidget : public QGLWidget,public Geometry
{
    Q_OBJECT
public slots:
	void resetFrame();
public:
    GLWidget();
	void sendDataToOpenGL();
	std::string readFile(const char* fileName);
	void installShaders();
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void mousePressEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent*);
	GLuint shader;
	GLuint vertexBufferID1;
	GLuint indexBufferID1;
	GLuint vertexBufferID2;
	GLuint indexBufferID2;
	GLint modelToProjectionMatrixULoc;
	GLuint numIndices1;
	GLuint numIndices2;

	GLuint vertexArrayObjectID1;
	GLuint vertexArrayObjectID2;
private:
	float xRot;
	float yRot;
	float zRot;
	QPoint lastPos;
	
};
#endif
