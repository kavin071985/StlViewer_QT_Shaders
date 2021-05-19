#pragma once
#include <QWidget>
class GLWidget;
///Creates a window layout for openGL &  push buttons
class Window: public QWidget
{
	Q_OBJECT
private:
	GLWidget *glWidget;
public:
	Window();
	~Window();
};

