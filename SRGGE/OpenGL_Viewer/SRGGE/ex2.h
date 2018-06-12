#ifndef ex2_H
#define ex2_H


#include <QtWidgets>
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QtOpenGL>
#include <fstream>
#include <iostream>
#include <memory>
#include <QKeyEvent>
#include <vector>
#include "SRGGE/ex1.h"
#include <tuple>


class ex2 : public ex1
{
    Q_OBJECT

public:
    ex2             (const QGLFormat &glf, QWidget *parent = 0);
    QGroupBox*      controlPanel();
    void            initVertexBuffer();

    void            computeGrid(int level);
    void            computeQuadric();
    int             cellid(int v, int l);

    int             cellnorm(int id);
    void            computeGridShape(int level);

    void            computeQ();

    float           dimx,dimy,dimz;
    int             level=2;
    int             nb_cells;

    std::vector<float>              new_vertices;
    std::vector<int>                new_faces;
    std::vector<float>              new_normals;

    std::vector<Eigen::Matrix4d>    QMatrices;

    bool            LODsimpleON=false;
    bool            QuadricON=false;
    bool            ShapeON=false;


protected:
    void		paintGL		();		// render GL scene


signals:
    void SetVertices(QString);
    void SetFaces(QString);

public slots:
    void setNumberlod(int);
    void setOFF();
    void setOnBasic();
    void setOnQuadric();
    void setOnShape();


};
#endif // ex2_H
