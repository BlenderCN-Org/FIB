#ifndef ex6_H
#define ex6_H


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
#include <random>
#include "SRGGE/ex5.h"


class ex6 : public ex5
{
    Q_OBJECT

public:
    ex6		(const QGLFormat &glf, QWidget *parent = 0);

    void                paintGL();
    bool                importMap(const std::string &filename);
    QGroupBox*          controlPanel();

    std::vector<int>    ReturnCoords(int side);
    void                SelectSides();
    void                Rays(int x1, int y1, int x2, int y2);
    void                addPoint(int x, int y);
    void                sendSet();
    void                addAndCheck(int i, int j, int a, int b);
    void                Visibility(int r);


    bool    last=false;
    std::vector<int>  visibility;
    std::vector<std::vector<std::vector<int>>>   VCells;



public slots:
    void loadMap();


};
#endif // ex6_H
