// Author: Marc Comino 2018

#include <glwidget.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "./triangle_mesh.h"
#include "./mesh_io.h"
#include "time.h"

namespace {

int final_time;
int initial_time=time(NULL);
int frames=0;

const float kFieldOfView = 60;
const float kZNear = 0.0001;
const float kZFar = 10;

const char kReflectionVertexShaderFile[] = "../shaders/reflection.vert";
const char kReflectionFragmentShaderFile[] = "../shaders/reflection.frag";
const char kBRDFVertexShaderFile[] = "../shaders/brdf.vert";
const char kBRDFFragmentShaderFile[] = "../shaders/brdf.frag";
const char kSkyVertexShaderFile[] = "../shaders/sky.vert";
const char kSkyFragmentShaderFile[] = "../shaders/sky.frag";

const int kVertexAttributeIdx = 0;
const int kNormalAttributeIdx = 1;

bool ReadFile(const std::string filename, std::string *shader_source) {
  std::ifstream infile(filename.c_str());

  if (!infile.is_open() || !infile.good()) {
    std::cerr << "Error " + filename + " not found." << std::endl;
    return false;
  }

  std::stringstream stream;
  stream << infile.rdbuf();
  infile.close();

  *shader_source = stream.str();
  return true;
}

bool LoadImage(const std::string &path, GLuint cube_map_pos) {
  QImage image;
  bool res = image.load(path.c_str());
  if (res) {
    QImage gl_image = image.mirrored();
    glTexImage2D(cube_map_pos, 0, GL_SRGB_ALPHA, image.width(), image.height(), 0,
                 GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
  }
  return res;
}

bool LoadCubeMap(const QString &dir) {
  std::string path = dir.toUtf8().constData();
  bool res = LoadImage(path + "/right.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
  res = res && LoadImage(path + "/left.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
  res = res && LoadImage(path + "/top.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
  res = res && LoadImage(path + "/bottom.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
  res = res && LoadImage(path + "/back.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
  res = res && LoadImage(path + "/front.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

  if (res) {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  return res;
}

bool LoadProgram(const std::string &vertex, const std::string &fragment,
                 QGLShaderProgram *program) {
  std::string vertex_shader, fragment_shader;
  bool res =
      ReadFile(vertex, &vertex_shader) && ReadFile(fragment, &fragment_shader);

  if (res) {
    program->addShaderFromSourceCode(QGLShader::Vertex,
                                     vertex_shader.c_str());
    program->addShaderFromSourceCode(QGLShader::Fragment,
                                     fragment_shader.c_str());
    program->bindAttributeLocation("vertex", kVertexAttributeIdx);
    program->bindAttributeLocation("normal", kNormalAttributeIdx);
    program->link();
  }

  return res;
}

}  // namespace

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent),
      initialized_(false),
      width_(0.0),
      height_(0.0),
      reflection_(true),
      fresnel_(0.2, 0.2, 0.2) {
  setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget() {
  if (initialized_) {
    glDeleteTextures(1, &specular_map_);
    glDeleteTextures(1, &diffuse_map_);
  }
}

bool GLWidget::LoadModel(const QString &filename) {
  std::string file = filename.toUtf8().constData();
  uint pos = file.find_last_of(".");
  std::string type = file.substr(pos + 1);

  std::unique_ptr<data_representation::TriangleMesh> mesh
    = std::unique_ptr<data_representation::TriangleMesh>( new data_representation::TriangleMesh());

  bool res = false;
  if (type.compare("ply") == 0) {
    res = data_representation::ReadFromPly(file, mesh.get());
  }

  if (res) {
    mesh_.reset(mesh.release());
    camera_.UpdateModel(mesh_->min_, mesh_->max_);


    glGenVertexArrays(1,&modelVAO);
    glBindVertexArray(modelVAO);

    //Vertex positions
    glGenBuffers(1,&modelVBOVertex);
    glBindBuffer(GL_ARRAY_BUFFER,modelVBOVertex);
    glBufferData(GL_ARRAY_BUFFER,mesh_->vertices_.size() * sizeof(GLfloat),&mesh_->vertices_[0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
    glEnableVertexAttribArray(0);

    //Vertex normals
    glGenBuffers(1,&modelVBONormals);
    glBindBuffer(GL_ARRAY_BUFFER,modelVBONormals);
    glBufferData(GL_ARRAY_BUFFER,mesh_->normals_.size() * sizeof(GLfloat),&mesh_->normals_[0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&modelVBOIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,modelVBOIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh_->faces_.size()*sizeof(GLint),&mesh_->faces_[0],GL_STATIC_DRAW);

    glBindVertexArray(0);



    emit SetFaces(QString(std::to_string(mesh_->faces_.size() / 3).c_str()));
    emit SetVertices(
        QString(std::to_string(mesh_->vertices_.size() / 3).c_str()));
    return true;
  }

  return false;
}

bool GLWidget::LoadSpecularMap(const QString &dir) {
  glBindTexture(GL_TEXTURE_CUBE_MAP, specular_map_);
  bool res = LoadCubeMap(dir);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  return res;
}

bool GLWidget::LoadDiffuseMap(const QString &dir) {
  glBindTexture(GL_TEXTURE_CUBE_MAP, diffuse_map_);
  bool res = LoadCubeMap(dir);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  return res;
}

void GLWidget::initializeGL() {
  glewInit();

  glEnable(GL_NORMALIZE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  glGenTextures(1, &specular_map_);
  glGenTextures(1, &diffuse_map_);

  reflection_program_=std::unique_ptr<QGLShaderProgram>(new(QGLShaderProgram));
  brdf_program_=std::unique_ptr<QGLShaderProgram>(new(QGLShaderProgram));
  sky_program_=std::unique_ptr<QGLShaderProgram>(new(QGLShaderProgram));

  bool res =
      LoadProgram(kReflectionVertexShaderFile, kReflectionFragmentShaderFile,
                  reflection_program_.get());
  res = res && LoadProgram(kBRDFVertexShaderFile, kBRDFFragmentShaderFile,
                           brdf_program_.get());
  res = res && LoadProgram(kSkyVertexShaderFile, kSkyFragmentShaderFile,
                           sky_program_.get());
  if (!res) exit(0);

  initialized_ = true;
}

void GLWidget::resizeGL(int w, int h) {
  if (h == 0) h = 1;
  width_ = w;
  height_ = h;

  camera_.SetViewport(0, 0, w, h);
  camera_.SetProjection(kFieldOfView, kZNear, kZFar);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    camera_.StartRotating(event->x(), event->y());
  }
  if (event->button() == Qt::RightButton) {
    camera_.StartZooming(event->x(), event->y());
  }
  updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  camera_.SetRotationX(event->y());
  camera_.SetRotationY(event->x());
  camera_.SafeZoom(event->y());
  updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
      camera_.StopRotating(event->x(), event->y());
    }
    if (event->button() == Qt::RightButton) {
      camera_.StopZooming(event->x(), event->y());
    }
    updateGL();
}


void GLWidget::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Up) camera_.Zoom(-1);
  if (event->key() == Qt::Key_Down) camera_.Zoom(1);

  if (event->key() == Qt::Key_Left) camera_.Rotate(-1);
  if (event->key() == Qt::Key_Right) camera_.Rotate(1);

  if (event->key() == Qt::Key_W) camera_.Zoom(-1);
  if (event->key() == Qt::Key_S) camera_.Zoom(1);

  if (event->key() == Qt::Key_A) camera_.Rotate(-1);
  if (event->key() == Qt::Key_D) camera_.Rotate(1);

  if (event->key() == Qt::Key_R) {
    reflection_program_.reset();
    reflection_program_ = std::unique_ptr<QGLShaderProgram>(new(QGLShaderProgram));
    LoadProgram(kReflectionVertexShaderFile, kReflectionFragmentShaderFile,
                reflection_program_.get());

    brdf_program_.reset();
    brdf_program_ = std::unique_ptr<QGLShaderProgram>(new(QGLShaderProgram));
    LoadProgram(kBRDFVertexShaderFile, kBRDFFragmentShaderFile,
                brdf_program_.get());

    sky_program_.reset();
    sky_program_ = std::unique_ptr<QGLShaderProgram>(new(QGLShaderProgram));
    LoadProgram(kSkyVertexShaderFile, kSkyFragmentShaderFile,
                sky_program_.get());
  }

  updateGL();
}

void GLWidget::paintGL() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (initialized_) {
    camera_.SetViewport();

    Eigen::Matrix4f projection = camera_.SetProjection();
    Eigen::Matrix4f view = camera_.SetView();
    Eigen::Matrix4f model = camera_.SetModel();

    Eigen::Matrix4f t = view * model;
    Eigen::Matrix3f normal;
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) normal(i, j) = t(i, j);

    normal = normal.inverse().transpose();


    if (mesh_ != nullptr) {
      GLuint projection_location, view_location, model_location,
          normal_matrix_location, specular_map_location, diffuse_map_location,
          fresnel_location;

      if (reflection_) {
        reflection_program_->bind();
        projection_location =
            reflection_program_->uniformLocation("projection");
        view_location = reflection_program_->uniformLocation("view");
        model_location = reflection_program_->uniformLocation("model");
        normal_matrix_location =
            reflection_program_->uniformLocation("normal_matrix");
        specular_map_location =
            reflection_program_->uniformLocation("specular_map");
        diffuse_map_location =
            reflection_program_->uniformLocation("diffuse_map");
        fresnel_location = reflection_program_->uniformLocation("fresnel");
      } else {
        brdf_program_->bind();
        projection_location = brdf_program_->uniformLocation("projection");
        view_location = brdf_program_->uniformLocation("view");
        model_location = brdf_program_->uniformLocation("model");
        normal_matrix_location =
            brdf_program_->uniformLocation("normal_matrix");
        specular_map_location = brdf_program_->uniformLocation("specular_map");
        diffuse_map_location = brdf_program_->uniformLocation("diffuse_map");
        fresnel_location = brdf_program_->uniformLocation("fresnel");
      }

      glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.data());
      glUniformMatrix4fv(view_location, 1, GL_FALSE, view.data());
      glUniformMatrix4fv(model_location, 1, GL_FALSE, model.data());
      glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, normal.data());

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, specular_map_);
      glUniform1i(specular_map_location, 0);

      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_CUBE_MAP, diffuse_map_);
      glUniform1i(diffuse_map_location, 1);

      glUniform3f(fresnel_location, fresnel_[0], fresnel_[1], fresnel_[2]);


      //RENDER MODEL
      glBindVertexArray(modelVAO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,modelVBOIndex);
      glDrawElements(GL_TRIANGLES,mesh_->faces_.size(),GL_UNSIGNED_INT,0);
      glBindVertexArray(0);

      // END.
    }

    model = camera_.SetIdentity();

    sky_program_->bind();
    GLuint projection_location = sky_program_->uniformLocation("projection");
    GLuint view_location = sky_program_->uniformLocation("view");
    GLuint model_location = sky_program_->uniformLocation("model");
    GLuint normal_matrix_location =
        sky_program_->uniformLocation("normal_matrix");
    GLuint specular_map_location =
        sky_program_->uniformLocation("specular_map");

    glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(view_location, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(model_location, 1, GL_FALSE, model.data());
    glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, normal.data());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, specular_map_);
    glUniform1i(specular_map_location, 0);

    glBegin(GL_TRIANGLES);
    // Right Face
    glVertex3f(2, -2, -2);
    glVertex3f(2, -2, 2);
    glVertex3f(2, 2, 2);

    glVertex3f(2, -2, -2);
    glVertex3f(2, 2, 2);
    glVertex3f(2, 2, -2);

    // Left Face
    glVertex3f(-2, -2, -2);
    glVertex3f(-2, 2, 2);
    glVertex3f(-2, -2, 2);

    glVertex3f(-2, -2, -2);
    glVertex3f(-2, 2, -2);
    glVertex3f(-2, 2, 2);

    // Front Face
    glVertex3f(-2, -2, 2);
    glVertex3f(-2, 2, 2);
    glVertex3f(2, 2, 2);

    glVertex3f(-2, -2, 2);
    glVertex3f(2, 2, 2);
    glVertex3f(2, -2, 2);

    // Back Face
    glVertex3f(-2, -2, -2);
    glVertex3f(2, 2, -2);
    glVertex3f(-2, 2, -2);

    glVertex3f(-2, -2, -2);
    glVertex3f(2, -2, -2);
    glVertex3f(2, 2, -2);

    // Top Face
    glVertex3f(-2, 2, -2);
    glVertex3f(2, 2, -2);
    glVertex3f(2, 2, 2);

    glVertex3f(-2, 2, -2);
    glVertex3f(2, 2, 2);
    glVertex3f(-2, 2, 2);

    // Bottom Face
    glVertex3f(-2, -2, -2);
    glVertex3f(2, -2, 2);
    glVertex3f(2, -2, -2);

    glVertex3f(-2, -2, -2);
    glVertex3f(-2, -2, 2);
    glVertex3f(2, -2, 2);

    glEnd();
  }

  //Framerate displaying.

  frames++;
  final_time=time(NULL);
  if(final_time - initial_time >0)
  {
      emit SetFramerate(QString::number(frames/(final_time-initial_time)));
      frames=0;
      initial_time=final_time;
  }


  // END.
}

void GLWidget::SetReflection(bool set) {
  reflection_ = set;
  updateGL();
}

void GLWidget::SetBRDF(bool set) {
  reflection_ = !set;
  updateGL();
}

void GLWidget::SetFresnelR(double r) {
  fresnel_[0] = r;
  updateGL();
}

void GLWidget::SetFresnelG(double g) {
  fresnel_[1] = g;
  updateGL();
}

void GLWidget::SetFresnelB(double b) {
  fresnel_[2] = b;
  updateGL();
}
