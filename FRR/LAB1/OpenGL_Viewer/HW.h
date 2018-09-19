#ifndef HW_H
#define HW_H


#include <QtWidgets>
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QtOpenGL>
#include <Helpers/mesh_io.h>
#include <QMouseEvent>
#include <fstream>
#include <iostream>
#include <memory>
#include <camera/Camera.h>
#include <QKeyEvent>

enum {
    ATTRIB_VERTEX,
    ATTRIB_NORMAL

};


extern QString GroupBoxStyle;

// ----------------------------------------------------------------------
// standard include files
//
class HW : public QGLWidget, protected QGLFunctions {

public:
    const float kFieldOfView = 60;
    const float kZNear = 0.0001;
    const float kZFar = 100;

	HW(const QGLFormat &glf, QWidget *parent = 0);
	virtual QGroupBox*	controlPanel	();		// create control panel
    QLabel *m_vertexCount;
    QLabel *m_facesCount;

    virtual void    reset		();		// reset parameters
    void			initShader	(QString, QString);
    virtual void    initVertexBuffer();

    bool    initialized_;
    float   width_;
    float   height_;
    int     m_winW;			// window width
    int     m_winH;			// window heights

    std::unique_ptr<QGLShaderProgram> program_;
    data_visualization::Camera camera_;
    std::unique_ptr<data_representation::TriangleMesh> mesh_;

protected:
    //GLint		  m_uniform[MXPROGRAMS][MXUNIFORMS];	// uniform vars for each program
    //GLint		  const_uniform[MXPROGRAMS][MXUNIFORMS];	// uniform vars for each program
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);



};




#endif // HW_H
