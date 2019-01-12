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

    int size_x;
    int size_y;

    void Display(QMatrix4x4 proj, QMatrix4x4 modelView);

private:
    Plane ground;
    std::vector<std::vector<int>> map;
    QOpenGLShaderProgram *program;
};

#endif // GRID_H
