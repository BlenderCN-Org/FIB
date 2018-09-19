#include "HW.h"


QString GroupBoxStyle = "QGroupBox {				\
			border: 1px solid gray;			\
			border-radius: 9px;			\
			font-weight: bold;			\
			margin-top: 0.5em;}			\
			QGroupBox::title {			\
			subcontrol-origin: margin;		\
			left: 10px;				\
			padding: 0 3px 0 3px;			\
			}";


HW::HW(const QGLFormat &glf, QWidget *parent) : QGLWidget (glf, parent),
    initialized_(false), width_(0.0), height_(0.0) {}


QGroupBox* HW::controlPanel()
{
    //ELEMENTS ON TOP
    QGroupBox *groupBox = new QGroupBox("Menu");
    groupBox->setStyleSheet(GroupBoxStyle);
    QGridLayout *layout = new QGridLayout;
    groupBox->setLayout(layout);
    return(groupBox);
}


void HW::reset() {}

void HW::initVertexBuffer(){}


void HW::initShader(QString vshaderName, QString fshaderName)
{

    //----------------------------------------------------
	uint vao;

	typedef void (APIENTRY *_glGenVertexArrays) (GLsizei, GLuint*);
	typedef void (APIENTRY *_glBindVertexArray) (GLuint);

	_glGenVertexArrays glGenVertexArrays;
	_glBindVertexArray glBindVertexArray;

	glGenVertexArrays = (_glGenVertexArrays) QGLWidget::context()->getProcAddress("glGenVertexArrays");
	glBindVertexArray = (_glBindVertexArray) QGLWidget::context()->getProcAddress("glBindVertexArray");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
    //----------------------------------------------------

    //Create QGL Program
    program_ = std::unique_ptr<QGLShaderProgram>(new QGLShaderProgram);

    //Compile Vertex shader
    bool flag = program_->addShaderFromSourceFile(QGLShader::Vertex, vshaderName);
	if(!flag) {
		QMessageBox::critical(0, "Error", "Vertex shader error: " + vshaderName + "\n" +
                    program_->log(), QMessageBox::Ok);
		exit(-1);
	}

    //Compile Fragment shader
    if(!program_->addShaderFromSourceFile(QGLShader::Fragment, fshaderName)) {
		QMessageBox::critical(0, "Error", "Fragment shader error: " + fshaderName + "\n" +
                    program_->log(), QMessageBox::Ok);
		exit(-1);
	}

    //Link Program to vertices and normals info
    //program_->bindAttributeLocation("vert", ATTRIB_VERTEX);
    //program_->bindAttributeLocation("normal" , ATTRIB_NORMAL);

    //Link shader pipeline
    if(!program_->link()) {
		QMessageBox::critical(0, "Error", "Could not link shader: " + vshaderName + "\n" +
                    program_->log(), QMessageBox::Ok);
        qDebug() << program_->log();
		exit(-1);
    }

    initialized_ = true;

}



//
// CAMERA

void HW::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Up) camera_.Zoom(-5);
  if (event->key() == Qt::Key_Down) camera_.Zoom(5);

  if (event->key() == Qt::Key_Left) camera_.Rotate(-1);
  if (event->key() == Qt::Key_Right) camera_.Rotate(1);

  if (event->key() == Qt::Key_W) camera_.Zoom(-5);
  if (event->key() == Qt::Key_S) camera_.Zoom(5);

  if (event->key() == Qt::Key_A) camera_.Rotate(-1);
  if (event->key() == Qt::Key_D) camera_.Rotate(1);

  updateGL();
}


void HW::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    camera_.StartRotating(event->x(), event->y());
  }
  if (event->button() == Qt::RightButton) {
    camera_.StartZooming(event->x(), event->y());
  }
  updateGL();
}

void HW::mouseMoveEvent(QMouseEvent *event) {
  camera_.SetRotationX(event->y());
  camera_.SetRotationY(event->x());
  camera_.SafeZoom(event->y());
  updateGL();
}

void HW::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    camera_.StopRotating(event->x(), event->y());
  }
  if (event->button() == Qt::RightButton) {
    camera_.StopZooming(event->x(), event->y());
  }
  updateGL();
}
