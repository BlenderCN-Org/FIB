#ifndef SSAO_VIEWER_H
#define SSAO_VIEWER_H


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

class ssao_viewer : public Viewer1
{
    Q_OBJECT

public:
    ssao_viewer		(const QGLFormat &glf, QWidget *parent = 0);
    QGroupBox*	controlPanel	();		// create control panel
    void		initVertexBuffer();		// init vertices
    void		initShaders	();		// init shaders


    GLuint          gBuffer, gPosition, gNormal;
    GLuint          depthrenderbuffer;
    GLuint          noiseTexture, ssaoFBO, ssaoTexture;
    int             copies=1;
    QElapsedTimer   t_Timer;
    double          lastTime;
    int             nbFrames=0;

    Shader          *gShader;
    Shader          *ssaoShader;
    Shader          *blurShader;

    std::vector<glm::vec3>  ssaoKernel;

    float           radius=0.5;

    int             sampleN = 64;
    float           blurRadius = 0.5;
    float           sigma = 20;
    bool            flag_blur=false;

protected:
    void		initializeGL	();		// init GL state
    void		paintGL		();		// render GL scene


public slots:
    void setRadius(double);
    void setBlurRadius(double);
    void setBlurOn();
    void setBlurOff();
    void setFirstSample();
    void setSecondSample();
    void setThirdSample();
    void setFourthSample();

signals:
    void SetFramerate(QString);

};
#endif // SSAO_VIEWER_H
