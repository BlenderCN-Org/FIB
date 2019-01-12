#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QObject>
#include <ctime>

#include "particle.h"
#include "generator.h"
#include "pathfinding.h"


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
	GLWidget(QWidget *parent);
	~GLWidget();


protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
    void initVertexBuffer();
    void postFrame();

    void createGenerator();

	void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


private:
    QMatrix4x4 setProjection(float aspect);
    QMatrix4x4 setModelview();

    QElapsedTimer *timer;

	float angleX, angleY, distance;
	QPoint lastMousePos;

    float width;
    float height;

    QOpenGLShaderProgram *program;
    QOpenGLShaderProgram *programParticles, *programCharacters;


    Generator G;
    pathFinding P;

    GLuint nb_particles = 10;
    //std::vector<Particle> particles;
    GLuint VAO;
    GLuint particleBuffer = 0;

    bool pathfinding = true;

public slots:
    void _tick();


};

#endif // GLWIDGET_H
