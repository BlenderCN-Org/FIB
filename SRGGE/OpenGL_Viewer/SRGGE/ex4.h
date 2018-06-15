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
    void                                    computeLevels(Eigen::Matrix4f view, Eigen::Matrix4f model);
    void                                    computeAdvanced(Eigen::Matrix4f view, Eigen::Matrix4f model, int frames);
    void                                    computeVFN();
    void                                    totTriangles();

    std::vector<int>                        findIDmin(Eigen::Matrix4f view, Eigen::Matrix4f model);
    std::vector<int>                        findIDmax(Eigen::Matrix4f view, Eigen::Matrix4f model);

    int                                     nb_triangles;

    Eigen::MatrixXd                         Levels;
    Eigen::MatrixXd                         Model_Frames;


    std::vector<std::vector<float>>         V;
    std::vector<std::vector<int>>           F;
    std::vector<std::vector<float>>         N;


    GLuint                                  vao, vao0,vao1,vao2,vao3,vao4;
    GLuint                                  vboVertex0, vboNormal0, vboIndex0;
    GLuint                                  vboVertex1, vboNormal1, vboIndex1;
    GLuint                                  vboVertex2, vboNormal2, vboIndex2;
    GLuint                                  vboVertex3, vboNormal3, vboIndex3;
    GLuint                                  vboVertex4, vboNormal4, vboIndex4;

    bool                                    AdvancedON=false;


public slots:
    void setNumberCopies(int);
    void setAdvancedON();
    void setAdvancedOFF();


};
#endif // ex4_H
