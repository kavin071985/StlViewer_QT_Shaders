#include "GLWidget.h"
#include "VertexBufferLayout.h"
#include <QFileDialog>
#include "QMessageBox"

Camera camera;
GLWidget::GLWidget():m_xRotation(0),m_yRot(0),m_zRot(0)
{	
}
//delete all raw member pointers
GLWidget::~GLWidget()
{
	shutDown(); 
}
///Loads the modal, creates layout,vertex array, vertex buffer & indices, sends data to them
void GLWidget::sendDataToOpenGL()
{
	Geometry model;
	//Shape1
	ShapeData shape = model.makePlane();
	VertexBufferLayout layout;
	layout.AddFloat(3); //3 positions
	layout.AddFloat(3);	//3 colors
	layout.AddFloat(3);	//3 normals

	m_VertexArrayID = new VertexArray();
	VertexBuffer vertexBufferID(shape.vertices, shape.vertexBufferSize());
	m_VertexArrayID->AddBuffer(vertexBufferID, layout);
	m_IndexBufferID= new IndexBuffer(shape.indices, shape.numIndices);
	m_VertexArrayID->Unbind();

	///Shape2
	QString filename = QFileDialog::getOpenFileName(this,tr("Open Stl File"),"C://","Stl(*.stl)"); //Browse & load the model file
	QMessageBox::information(this,tr("file name"),filename);//Message box for displaying file path
	shape = model.makeStl(filename);
	m_VertexArrayID2 = new VertexArray();
	VertexBuffer vertexBufferID2(shape.vertices, shape.vertexBufferSize());
	m_VertexArrayID2->AddBuffer(vertexBufferID2, layout);
	m_IndexBufferID2= new IndexBuffer(shape.indices, shape.numIndices);
	m_VertexArrayID2->Unbind();
	
	shape.cleanup();
}
/// Creates a shader program 
void GLWidget::installShaders()
{
	m_Shader = new Shader("res/shaders/Basic.shader"); 
	m_Shader->Unbind();
}

void GLWidget::initializeGL()
{
	setMouseTracking(false);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();

}

void GLWidget::paintGL()
{

	Renderer renderer;
	m_Shader->Bind();
	glViewport(0,0,width(),height());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	renderer.Clear(); //Clears the color & depth buffer bit
	
	// using ambient and diffuse lighting & sending the data to the shader program uniforms
	glm::vec4 ambientLight(0.05f,0.05f,0.05f,1.0f);
	m_Shader->SetUniform4fv("u_ambientLight",1,ambientLight);
	glm::vec3 lightPositionWorld(0.0f,-0.5f,0.0f);
	m_Shader->SetUniform3fv("u_lightPositionWorld",1,lightPositionWorld);

	//MVP
	glm::mat4 viewToProjectionMatrix =glm::perspective(60.0f,((float)width())/height(),0.1f,50.0f);		//projectionMatrix
	glm::mat4 worldToViewMatrix =camera.getWorldToViewMatrix();											//view matrix
	glm::mat4 worldToProjectionMatrix=viewToProjectionMatrix*worldToViewMatrix;							//p*v

	/// Shape 1 model view & projection matrices sent to shader via uniform.
	glm::mat4 ModelToWorldMatrix1=glm::translate(glm::vec3(-0.0f, -1.0f, -0.0f)) 
										*glm::rotate(0.0f,glm::vec3(1.0f,0.0f,0.0f));
	glm::mat4 modelToProjectionMatrix = worldToProjectionMatrix*ModelToWorldMatrix1;		//MVP matrix P*V*M
	m_Shader->SetUniformMatrix4fv("u_modelToProjectionMatrix",1,GL_FALSE,modelToProjectionMatrix);//sending MVP matrix to shader uniform
	m_Shader->SetUniformMatrix4fv("u_modelToWorldMatrix",1,GL_FALSE,ModelToWorldMatrix1);// sending VP matrix to shader uniform. normal positions needs to be transformed 
	
	renderer.Draw(m_VertexArrayID,m_IndexBufferID,m_Shader);
	
	/// Shape 2 model view & projection matrices sent to shader via uniform.
	glm::mat4 ModelToWorldMatrix=glm::translate(glm::vec3(0.0f, 0.0f, -3.0f))
								*glm::rotate(m_xRotation,glm::vec3(1.0f,0.0f,0.0f))*glm::rotate(m_yRot,glm::vec3(0.0f,1.0f,0.0f));		
	modelToProjectionMatrix = worldToProjectionMatrix*ModelToWorldMatrix;
	m_Shader->SetUniformMatrix4fv("u_modelToProjectionMatrix",1,GL_FALSE,modelToProjectionMatrix);
	m_Shader->SetUniformMatrix4fv("u_modelToWorldMatrix",1,GL_FALSE,ModelToWorldMatrix);
	renderer.Draw(m_VertexArrayID2,m_IndexBufferID2,m_Shader);
	m_Shader->Unbind();
	
}
/// Records the position of the mouse when pressed.
void GLWidget::mousePressEvent(QMouseEvent *event)
{ m_LastPosition = event->pos();}

///This function records the movement of the mouse and determines the angle of rotation & updates camera accordingly.
/// @param dx,dy change in mouse position along x,y of the window coordinates
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
 GLfloat dx = (GLfloat) (event->x() - m_LastPosition.x()) / width();
 GLfloat dy = (GLfloat) (event->y() - m_LastPosition.y()) / height();
 setFocus();
 if (event->buttons() & Qt::LeftButton) {
	camera.mouseUpdate(glm::vec2(event->x(),event->y()));
	repaint();
 } else if (event->buttons() & Qt::RightButton) {
	
	m_xRotation -= 180 * dy;
	m_yRot += 180 * dx;
	repaint();
	
 }
 m_LastPosition = event->pos();
}
/// Camera movements when the following key is pressed
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
		camera.panLeft();
		break;
	case Qt::Key::Key_D:
		camera.panRight();
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
/// Resets the camera and the model to its initial position
void GLWidget::resetFrame()
{
	m_xRotation = 0;
	m_yRot = 0;
	m_zRot = 0;
	camera.resetCam();
	repaint();
}
///loads a custom modal file 
///@param QFileDialog :opens a file QFileDialog to browse through the directory
///@param filename :stores the path to the file
void GLWidget::loadFile()
{
	QString filename = QFileDialog::getOpenFileName(this,tr("Open Stl File"),"C://","Stl(*.stl)");
	QMessageBox::information(this,tr("file name"),filename);
	Geometry model;
	ShapeData shape = model.makeStl(filename);
	VertexBufferLayout layout;
	layout.AddFloat(3);
	layout.AddFloat(3);
	layout.AddFloat(3);
	delete m_VertexArrayID2;
	m_VertexArrayID2 = new VertexArray();
	VertexBuffer vertexBufferID2(shape.vertices, shape.vertexBufferSize());
	m_VertexArrayID2->AddBuffer(vertexBufferID2, layout);
	m_IndexBufferID2= new IndexBuffer(shape.indices, shape.numIndices);
	m_VertexArrayID2->Unbind();

	shape.cleanup();
}


///Deletes all raw pointers to cope up with resource leak, called in destructor
void GLWidget::shutDown()
{
	delete m_Shader;
	delete m_VertexArrayID;
	delete m_IndexBufferID;
	delete m_VertexArrayID2;
	delete m_IndexBufferID2;
}