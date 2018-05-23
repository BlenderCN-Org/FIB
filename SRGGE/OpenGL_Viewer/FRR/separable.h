#ifndef SEPARABLE_H
#define SEPARABLE_H


#include <QtWidgets>
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QtOpenGL>
#include <fstream>
#include <iostream>
#include <memory>
#include <QKeyEvent>


#include "Viewer1.h"

class separable : public Viewer1
{
    Q_OBJECT

public:
    separable		(const QGLFormat &glf, QWidget *parent = 0);
    QGroupBox*	controlPanel	();		// create control panel
    void		initVertexBuffer();		// init vertices
    void		initShaders	();		// init shaders


    GLuint          gBuffer, gPosition, gNormal;
    GLuint          depthrenderbuffer;
    GLuint          noiseTexture, ssaoFBO, ssaoBlurFBO, ssaoColorBuffer,ssaoTexture;
    int             copies=1;
    QElapsedTimer   t_Timer;
    double          lastTime;
    int             nbFrames=0;

    Shader          *gShader;
    Shader          *ssaoShader;

    std::vector<glm::vec3>  ssaoKernel_x;
    std::vector<glm::vec3>  ssaoKernel_y;

    float radius=0.5;
    int   sampleN = 9;

protected:
    void		initializeGL	();		// init GL state
    void		paintGL		();		// render GL scene


public slots:
    void setRadius(double);
    void setFirstSample();
    void setSecondSample();
    void setThirdSample();
    void setFourthSample();

signals:
    void SetFramerate(QString);



};
#endif // SEPARABLE_H
