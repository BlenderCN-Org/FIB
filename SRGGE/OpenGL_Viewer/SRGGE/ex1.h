#ifndef ex1_H
#define ex1_H


#include <QtWidgets>
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QtOpenGL>
#include <fstream>
#include <iostream>
#include <memory>
#include <QKeyEvent>


#include "FRR/Viewer1.h"

class ex1 : public Viewer1
{
    Q_OBJECT

public:
    ex1		(const QGLFormat &glf, QWidget *parent = 0);
    void            initVertexBuffer();		// init vertices
    QGroupBox*      controlPanel();

    GLuint          depthrenderbuffer;
    int             copies=1;

    Shader          *gShader;

    QLabel          *print_fps;

    int final_time, initial_time=time(NULL), frames=0, count_frames=0;


protected:
    void		initializeGL	();		// init GL state
    void		paintGL		();		// render GL scene


public slots:
    void setNumberCopies(int);


signals:
    void SetFramerate(QString);

};
#endif // ex1_H
