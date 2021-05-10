#include "Window.h"
#include <QtGui>
#include "GLWidget.h"

Window::Window()
{
	glWidget = new GLWidget;
	QHBoxLayout* mainLayout= new QHBoxLayout;
	mainLayout->addWidget(glWidget);
	setLayout(mainLayout);
	setWindowTitle("STL Viewer");
	QPushButton *reset_button = new QPushButton(this);
	reset_button->setText(tr("Reset"));
	reset_button->move(10, 10);
	connect(reset_button, SIGNAL( clicked()), glWidget,SLOT( resetFrame()));
	reset_button->show();
}


Window::~Window()
{
}
