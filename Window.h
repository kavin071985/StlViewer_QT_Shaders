#pragma once
#include <QWidget>
class GLWidget;
class Window: public QWidget
{
	Q_OBJECT
private:
	GLWidget *glWidget;
public:
	Window();
	~Window();
};

