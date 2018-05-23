// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Dummy.cpp - Dummy class
//
// Written by: George Wolberg, 2017
// ===============================================================

#include "dummy.h"


Dummy::Dummy(const QGLFormat &glf, QWidget *parent) : HW(glf, parent) {}


void Dummy::initializeGL()
{
	// init state variables
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);	// set background color
	glColor3f   (1.0f, 1.0f, 1.0f);		// set foreground color
}



void Dummy::resizeGL(int w, int h)
{
	// save window dimensions
	m_winW = w;
	m_winH = h;

	// compute aspect ratio
	float ar = (float) w / h;

	// set xmax, ymax
	float xmax, ymax;
	if(ar > 1.0) {		// wide screen
		xmax = ar;
		ymax = 1.;
	} else {		// tall screen
		xmax = 1.;
		ymax = 1/ar;
	}

	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);

	// init viewing coordinates for orthographic projection
	glLoadIdentity();
	glOrtho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);
}



void Dummy::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
}


QGroupBox* Dummy::controlPanel()
{
	// init group box
    QGroupBox *groupBox = new QGroupBox("Menu");
	groupBox->setStyleSheet(GroupBoxStyle);
	return(groupBox);
}
