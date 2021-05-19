
#include "GL\glew.h"
#include<QGLWidget>
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include "Geometry.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "glm.hpp"
#include "gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include "Vertex.h"
#include "Camera.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include "Shader.h"
#include "Renderer.h"


///Initializes & paints the openGL model, evaluates the mouse & key press events
class GLWidget : public QGLWidget
{
    Q_OBJECT
public slots:
	void resetFrame();
	void loadFile();
public:
    GLWidget();
	~GLWidget();
	void sendDataToOpenGL();
	void installShaders();
	void shutDown();
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*event);
	void mousePressEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent*event);
private:
	Shader* m_Shader;
	VertexArray *m_VertexArrayID;
	IndexBuffer *m_IndexBufferID;
	VertexArray *m_VertexArrayID2;
	IndexBuffer *m_IndexBufferID2;
	
	GLfloat m_xRotation;
	GLfloat m_yRot;
	GLfloat m_zRot;
	QPoint m_LastPosition;
	
};

