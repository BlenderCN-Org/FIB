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
    void		initVertexBuffer();		// init vertices
    QGroupBox*  controlPanel();

    GLuint          depthrenderbuffer;
    int             copies=1;

    QElapsedTimer   t_Timer;
    double          lastTime;
    int             nbFrames=0;

    Shader          *gShader;

    QLabel          *print_fps;



protected:
    void		initializeGL	();		// init GL state
    void		paintGL		();		// render GL scene


public slots:
    void setNumberCopies(int);


signals:
    void SetFramerate(QString);

};
#endif // ex1_H
