#include "pathfinding.h"

#include <random>
#define rand01() ((float)std::rand()/RAND_MAX)


int transformCoord(int i, int j,int size_x, int size_y){
    return j+i*size_y;
}


pathFinding::pathFinding(int x, int y)
{
    size_x=x;
    size_y=y;

    grid = Grid(x,y);
    G = Generator(2);
    startx = 0;
    starty=0;
    goalx=5;
    goaly=5;


}



void pathFinding::Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){
    grid.Display(proj,modelView);
    G.Display(program,proj,modelView);
}


void pathFinding::initNodes(){

    NodeList.clear();
    NodeList.resize(size_x);
    for ( int i = 0 ; i < size_x ; i++ )
       NodeList[i].resize(size_y);



    for(int i=0; i<size_x; i++){
        for(int j=0; j<size_y;j++){
            struct node addNode;
            addNode.x = i;
            addNode.y = j;
            addNode.hCost = abs(goalx-i)+abs(goaly-j); //Manhattan distance for each cell
            addNode.Parentx = NULL;
            addNode.Parenty = NULL;
            if(i==startx && j == starty){
                addNode.open = 0;
                pQueue.push(addNode);
            }
            NodeList[i][j]=addNode;

        }
    }
}

void testNeighbor(node* current, node* neighbor, std::priority_queue<node>* pQ){

    float cost = current->gCost + neighbor->movementCost;

    if(neighbor->open>0 && cost < neighbor->gCost){ //IN OPEN LIST
            neighbor->open=0;
    }

    if(neighbor->open<0 && cost < neighbor->gCost){ //IN CLOSED LIST
            neighbor->open=0;
    }

    if(neighbor->open==0){  //Not in the open list
        neighbor->gCost = cost;
        neighbor->open = neighbor->gCost + neighbor->hCost;
        neighbor->Parentx = current->x;
        neighbor->Parenty = current->y;
        pQ->push(*neighbor);
    }


}


void pathFinding::aStar(){
    initNodes();

    node lowestNode = pQueue.top();

    while (lowestNode.x != goalx || lowestNode.y != goaly){

        node CurrentNode = lowestNode;
        pQueue.pop();
        CurrentNode.open = -1;

//        std::cout << "x: " << CurrentNode.x << "  y:  " << CurrentNode.y << std::endl;

        //NEIGHBOR 1 - LEFT
//        std::cout << "LEFT" << std::endl;
        if(CurrentNode.x>0){
            int Nx = CurrentNode.x -1;
            int Ny = CurrentNode.y;
            node neighbor = NodeList[Nx][Ny];
            NodeList[Nx][Ny].movementCost = 10;
            testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
//            NodeList[Nx][Ny].Parentx = neighbor.Parentx;
//            NodeList[Nx][Ny].Parenty = neighbor.Parenty;
        }

        //NEIGHBOR 2 - RIGHT
//        std::cout << "RIGHT" << std::endl;
        if(CurrentNode.x<(size_x-1)){
            int Nx = CurrentNode.x+1;
            int Ny = CurrentNode.y;
            node neighbor = NodeList[Nx][Ny];
            NodeList[Nx][Ny].movementCost = 10;
            testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
//            NodeList[Nx][Ny].Parentx = neighbor.Parentx;
//            NodeList[Nx][Ny].Parenty = neighbor.Parenty;
        }

        //NEIGHBOR 3 - UP
//        std::cout << "UP" << std::endl;
        if(CurrentNode.y<(size_y-1)){
            int Nx = CurrentNode.x;
            int Ny = CurrentNode.y+1;
            node neighbor = NodeList[Nx][Ny];
            NodeList[Nx][Ny].movementCost = 10;
            testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
//            NodeList[Nx][Ny].Parentx = neighbor.Parentx;
//            NodeList[Nx][Ny].Parenty = neighbor.Parenty;
        }

        //NEIGHBOR 4 - DOWN
//        std::cout << "DOWN" << std::endl;
        if(CurrentNode.y>0){
            int Nx = CurrentNode.x;
            int Ny = CurrentNode.y-1;
            node neighbor = NodeList[Nx][Ny];
            NodeList[Nx][Ny].movementCost = 10;
            testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
//            NodeList[Nx][Ny].Parentx = neighbor.Parentx;
//            NodeList[Nx][Ny].Parenty = neighbor.Parenty;
        }


        //NEIGHBOR 5 - TOP LEFT
//        std::cout << "TOP LEFT" << std::endl;
        if(CurrentNode.x>0 && CurrentNode.y<(size_y-1)){
            int Nx = CurrentNode.x -1;
            int Ny = CurrentNode.y +1;
            node neighbor = NodeList[Nx][Ny];
            NodeList[Nx][Ny].movementCost = 14;
            testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
//            NodeList[Nx][Ny].Parentx = neighbor.Parentx;
//            NodeList[Nx][Ny].Parenty = neighbor.Parenty;
        }

        //NEIGHBOR 6 - TOP RIGHT
//        std::cout << "TOP RIGHT" << std::endl;
        if(CurrentNode.x<(size_x-1) && CurrentNode.y<(size_y-1)){
            int Nx = CurrentNode.x+1;
            int Ny = CurrentNode.y+1;
            node neighbor = NodeList[Nx][Ny];
            NodeList[Nx][Ny].movementCost = 14;
            testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
//            NodeList[Nx][Ny].Parentx = neighbor.Parentx;
//            NodeList[Nx][Ny].Parenty = neighbor.Parenty;
        }

        //NEIGHBOR 7 - BOTTOM LEFT
//        std::cout << "BOTTOM LEFT" << std::endl;
        if(CurrentNode.x>0 && CurrentNode.y>0){
            int Nx = CurrentNode.x-1;
            int Ny = CurrentNode.y-1;
            node neighbor = NodeList[Nx][Ny];
            NodeList[Nx][Ny].movementCost = 14;
            testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
//            NodeList[Nx][Ny].Parentx = neighbor.Parentx;
//            NodeList[Nx][Ny].Parenty = neighbor.Parenty;
        }

        //NEIGHBOR 8 - BOTTOM RIGHT
//        std::cout << "BOTTOM RIGHT" << std::endl;
        if(CurrentNode.x<(size_x-1) && CurrentNode.y>0){
            int Nx = CurrentNode.x+1;
            int Ny = CurrentNode.y-1;
            node neighbor = NodeList[Nx][Ny];
            NodeList[Nx][Ny].movementCost = 14;
            testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
//            NodeList[Nx][Ny].Parentx = neighbor.Parentx;
//            NodeList[Nx][Ny].Parenty = neighbor.Parenty;
        }


        lowestNode = pQueue.top();


    }

    while(lowestNode.x != startx || lowestNode.y != starty){
        std::cout << "x : " << lowestNode.Parentx << "  -  y: " << lowestNode.Parenty << std::endl;
        lowestNode = NodeList[lowestNode.Parentx][lowestNode.Parenty];
    }


//    for(int i=0; i<size_x; i++){
//        for(int j=0; j<size_y; j++){
//            std::cout << NodeList[i][j].Parentx << "," << NodeList[i][j].Parenty << "  ";
//        }
//        std::cout << std::endl;
//    }


}




