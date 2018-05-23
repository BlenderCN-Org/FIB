#ifndef VIEWER1_H
#define VIEWER1_H

#include <QtWidgets>
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QtOpenGL>
#include <fstream>
#include <iostream>
#include <memory>
#include <QKeyEvent>

#include "HW.h"
#include "Helpers/mesh_importer.h"
#include "Helpers/shader.h"

class Viewer1 : public HW
{
    Q_OBJECT

public:
    Viewer1		(const QGLFormat &glf, QWidget *parent = 0);
    QGroupBox*	controlPanel	();		// create control panel
    void		reset		();		// reset parameters
    void		initVertexBuffer();		// init vertices
    void		initShaders	();		// init shaders

    MI              mesh_importer;
    GLuint          vao, vboVertex, vboNormal, vboIndex;
    //GLuint          gBuffer,gTexture;
    //gPosition, gNormal, gAlbedoSpec,
//    GLuint          depthrenderbuffer;
    GLuint          quadVAO, quadVBO=0, quadTextCoordVBO, quadID;
//    GLuint          noiseTexture, ssaoFBO, ssaoBlurFBO, ssaoColorBuffer, ssaoColorBufferBlur;
    QElapsedTimer   t_Timer;
    double          lastTime;
    int             nbFrames=0;
    int             copies=1;

    Shader *gShader;

protected:
    void		initializeGL	();		// init GL state
    void		resizeGL	(int, int);	// resize GL widget
    void		paintGL		();		// render GL scene
    void        paintQuad();




public slots:
    void importModel ();
    void exportModel ();
    void setNumberCopies(int);

signals:
    void SetFramerate(QString);

};

#endif // VIEWER1_H
