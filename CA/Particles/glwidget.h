#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QObject>

#include "particle.h"
#include "generator.h"


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
	GLWidget(QWidget *parent);
	~GLWidget();


    float time=5.0f;

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
    void initVertexBuffer();

    void createGenerator();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);


private:
    QMatrix4x4 setProjection(float aspect);
    QMatrix4x4 setModelview();

    QTimer *timer;

	float angleX, angleY, distance;
	QPoint lastMousePos;

    float width;
    float height;

    QOpenGLShaderProgram *program;
    QOpenGLShaderProgram *programParticles;


    Generator G;

    GLuint nb_particles = 300;
    //std::vector<Particle> particles;
    GLuint VAO;
    GLuint particleBuffer = 0;

public slots:
    void _tick();
    void setParticleSize(float size);
    void setMethod(Particle::UpdateMethod met);
    void reset();
    void setSpring1D();
    void setSpring2D();

};

#endif // GLWIDGET_H
