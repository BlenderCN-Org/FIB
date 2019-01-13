#ifndef GRID_H
#define GRID_H

#include "geometry.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class Grid
{
public:
    Grid(){}
    Grid(int x, int y);
    ~Grid();
    void reset();
    void Display(QMatrix4x4 proj, QMatrix4x4 modelView);
    std::vector<std::vector<int>> map;

private:
    Plane ground;
    int size_x;
    int size_y;

    QOpenGLShaderProgram *program;
};

#endif // GRID_H
