#include "glwidget.h"
#include <iostream>
#include <QApplication>
#include <QMatrix4x4>
#include <QMouseEvent>

using namespace std;


const float rotationFactor = 0.5f;
const float maxRotationCamera = 75.0f;
const float minDistanceCamera = 1.0f;
const float maxDistanceCamera = 100.0f;


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), angleX(0.0f), angleY(0.0f), distance(20.0f), width(0.0), height(0.0)
{
    program = nullptr;
    programParticles = nullptr;

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    setFormat(format);

    QTimer::singleShot(15, this, SLOT(_tick()));


}

GLWidget::~GLWidget()
{
    if(program) delete program;
    if(programParticles) delete programParticles;
}


void GLWidget::createGenerator(){

    G = Generator(nb_particles);
    initializeOpenGLFunctions();

    //Init particles
    G.initBuffers();
    //Init planes
    G.plane_down.initBuffers();
    G.plane_up.initBuffers();
    G.plane_bottom.initBuffers();
    G.plane_front.initBuffers();
    G.plane_left.initBuffers();
    G.plane_right.initBuffers();
    //Init triangle
    G.triangle.initBuffers();
    //Init sphere
    G.sphere.initBuffers();
}

void GLWidget::initializeGL()
{

    initializeOpenGLFunctions();

    createGenerator();

    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simpleshader.vert");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simpleshader.frag");
    program->bindAttributeLocation("position", 0);
    program->link();
    if(!program->isLinked())
    {
            cout << "Shader program has not linked" << endl << endl << "Log: " << endl << endl << program->log().toStdString();
            QApplication::quit();
    }


    programParticles = new QOpenGLShaderProgram();
    programParticles->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/particles.vert");
    programParticles->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/particles.frag");
    programParticles->bindAttributeLocation("position", 0);
    programParticles->bindAttributeLocation("value", 2);
    programParticles->link();
    if(!programParticles->isLinked())
    {
            cout << "Shader program has not linked" << endl << endl << "Log: " << endl << endl << program->log().toStdString();
            QApplication::quit();
    }


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    programParticles->bind();
    program->bind();
}




void GLWidget::resizeGL(int w, int h)
{
    width=w;
    height=h;
	glViewport(0,0,w,h);
	setProjection((float)w/h);
	setModelview();
}


void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 proj = setProjection(width/height);
    QMatrix4x4 modelView = setModelview();

    //Display planes
    G.plane_down.Display(program,proj,modelView);
    G.plane_up.Display(program,proj,modelView);
    G.plane_left.Display(program,proj,modelView);
    G.plane_right.Display(program,proj,modelView);
    G.plane_bottom.Display(program,proj,modelView);
    G.plane_front.Display(program,proj,modelView);
    G.triangle.Display(program,proj,modelView);
    G.sphere.Display(programParticles,proj,modelView);


    //Display particles
    G.Display(programParticles,proj,modelView);


}

void GLWidget::initVertexBuffer()
{


}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	// Rotation
	if(event->buttons() & Qt::LeftButton)
	{
		angleX += rotationFactor * (event->y() - lastMousePos.y());
		angleX = max(-maxRotationCamera, min(angleX, maxRotationCamera));
		angleY += rotationFactor * (event->x() - lastMousePos.x());
	}
	// Zoom
	if(event->buttons() & Qt::RightButton)
	{
		distance += 0.01f * (event->y() - lastMousePos.y());
		distance = max(minDistanceCamera, min(distance, maxDistanceCamera));
	}

	lastMousePos = event->pos();

	makeCurrent();
	setModelview();
	doneCurrent();
	update();
}

QMatrix4x4 GLWidget::setProjection(float aspect)
{
    QMatrix4x4 projectionMatrix;

    projectionMatrix.perspective(45, aspect, 0.1, 100.0);

    return projectionMatrix;


}


QMatrix4x4 GLWidget::setModelview()
{
    QMatrix4x4 modelviewMatrix;

    modelviewMatrix.translate(0, 0, -distance);
    modelviewMatrix.rotate(angleX, 1.0f, 0.0f, 0.0f);
	modelviewMatrix.rotate(angleY, 0.0f, 1.0f, 0.0f);

    return modelviewMatrix;

}




void GLWidget::_tick()
{
    update();
    QTimer::singleShot(15, this, SLOT(_tick()));

}


void GLWidget::setParticleSize(float size){
    G.radius = size;
}


void GLWidget::setMethod(Particle::UpdateMethod met){
    G.method = met;
    reset();
}


void GLWidget::reset(){
//    if(program) delete program;
//    if(programParticles) delete programParticles;
//    initializeGL();
//    paintGL();

    createGenerator();
    update();

}


void GLWidget::setSpring1D(){
    G.spring1D_selected=true;
    G.spring2D_selected=false;
    reset();
}



void GLWidget::setSpring2D(){
    G.spring1D_selected=false;
    G.spring2D_selected=true;
    reset();
}

