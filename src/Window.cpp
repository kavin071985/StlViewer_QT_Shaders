#include "Window.h"
#include <QtGui>
#include "GLWidget.h"

/// This class creates a window layout with buttons for displaying the openGL projections
///
///Creates boxed layout for the openGL window.
///Adds an openGL widget to the layout.
///Sets the Layout to current context. 
///Adds a reset push button to the vlayout.
///Adds a loadFile push button to the vlayout.
///~~~~~~~~.cpp
/// glWidget = new GLWidget;
/// QHBoxLayout *mainLayout = new QHBoxLayout;
/// QVBoxLayout* vLayout= new QVBoxLayout; 
/// mainLayout->addWidget(glWidget);
/// setLayout(mainLayout);
/// QPushButton *reset_button = new QPushButton(this);
/// vLayout->addWidget(reset_button);
///~~~~~~~~
/// @see GLWidget::resetFrame()

Window::Window()
{
	glWidget = new GLWidget;
	QHBoxLayout* mainLayout= new QHBoxLayout;
	QVBoxLayout* vLayout= new QVBoxLayout;
	mainLayout->addWidget(glWidget);
	setLayout(mainLayout);
	setWindowTitle("STL Viewer");
	QPushButton *reset_button = new QPushButton(this);
	vLayout->addWidget(reset_button);
	reset_button->setText(tr("Reset"));
	reset_button->move(10, 10);
	connect(reset_button, SIGNAL( clicked()), glWidget,SLOT( resetFrame()));
	reset_button->show();

	QPushButton *loadFile_button = new QPushButton(this);
	vLayout->addWidget(loadFile_button);
	loadFile_button->setText(tr("Load File"));
	loadFile_button->move(100, 10);
	connect(loadFile_button, SIGNAL( clicked()), glWidget,SLOT( loadFile()));
	loadFile_button->show();
}


Window::~Window()
{
}
