#ifndef ex5_H
#define ex5_H


#include <QtWidgets>
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QtOpenGL>
#include <fstream>
#include <iostream>
#include <memory>
#include <QKeyEvent>


#include "FRR/Viewer1.h"

class ex5 : public Viewer1
{
    Q_OBJECT

public:
    ex5		(const QGLFormat &glf, QWidget *parent = 0);
    QGroupBox*              controlPanel();

    void                    paintGL();
    void                    initializeGL();
    void                    initVertexBuffer();
    void                    computeQuad();
    void                    computeWall();

    bool                    checkIE(std::vector<std::vector<int>> map, int line, int row);
    std::vector<int>        checkWall(std::vector<std::vector<int>> map, int line, int row);
    bool                    importMap(const std::string &filename);

    GLuint                  VAO, VBO, NVBO, EBO;
    GLuint                  WVAO, WVBO, WNVBO, WEBO;

    std::vector<int>        faces;
    std::vector<float>      vertices;
    std::vector<float>      normals;

    std::vector<int>        Wfaces;
    std::vector<float>      Wvertices;
    std::vector<float>      Wnormals;


    std::vector<std::vector<int>>   museum;

public slots:
    void loadMap();


};
#endif // ex5_H
