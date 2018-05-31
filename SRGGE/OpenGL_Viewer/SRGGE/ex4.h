#ifndef ex4_H
#define ex4_H


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
#include "SRGGE/ex2.h"
#include <tuple>


class ex4 : public ex2
{
    Q_OBJECT

public:
    ex4(const QGLFormat &glf, QWidget *parent = 0);
    void                                    initializeGL();
    void                                    paintGL();
    void                                    initVertexBuffer();
    QGroupBox*                              controlPanel();

    float                                   computeIndivCost(Eigen::Matrix4f view, Eigen::Matrix4f model, int i, int j, int L);
    Eigen::MatrixXd                         computeLevels(Eigen::Matrix4f view, Eigen::Matrix4f model, Eigen::MatrixXd Levels);
    void                                    computeVFN();
    int                                     totTriangles(Eigen::MatrixXd Levels);
    std::vector<int>                        findID(Eigen::Matrix4f view, Eigen::Matrix4f model, Eigen::MatrixXd Cost);

    Eigen::MatrixXd Levels;

    std::vector<std::vector<float>>         V;
    std::vector<std::vector<int>>           F;
    std::vector<std::vector<float>>         N;


    GLuint                                  vao, vao0,vao1,vao2,vao3,vao4;
    GLuint                                  vboVertex0, vboNormal0, vboIndex0;
    GLuint                                  vboVertex1, vboNormal1, vboIndex1;
    GLuint                                  vboVertex2, vboNormal2, vboIndex2;
    GLuint                                  vboVertex3, vboNormal3, vboIndex3;
    GLuint                                  vboVertex4, vboNormal4, vboIndex4;


public slots:
    void setNumberCopies(int);


};
#endif // ex4_H
