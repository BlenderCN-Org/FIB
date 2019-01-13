#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "grid.h"
#include "generator.h"
#include <queue>

struct node{
    int x;
    int y;
    float gCost=0;
    float hCost;
    float movementCost=0;
    float open=0;  //If open==0, node is in close list - otherwise open=fvalue
    int Parentx;
    int Parenty; //coords of Parent node


    bool operator<(const node& other) const
    {
    return (this->gCost+this->hCost) > (other.gCost + other.hCost);
    }
};




class pathFinding
{
public:
    pathFinding(){}
    pathFinding(int x, int y, Generator Gene);

    Grid grid;
    Generator G;

    void Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView);
    void initNodes();
    void aStar();

    bool testValueX(int x);
    bool testValueY(int y);

    int startx;
    int starty;
    int goalx;
    int goaly;
    int size_x;
    int size_y;

private:

    std::priority_queue<node> pQueue;
    std::vector<std::vector<node>> NodeList;
//    std::vector<float> gCosts;

};

#endif // PATHFINDING_H
