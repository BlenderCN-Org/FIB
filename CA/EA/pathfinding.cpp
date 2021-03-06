#include "pathfinding.h"

#include <random>
#define rand01() ((float)std::rand()/RAND_MAX)


int transformCoord(int i, int j,int size_x, int size_y){
    return j+i*size_y;
}


pathFinding::pathFinding(int x, int y, Generator Gene)
{
    size_x=x;
    size_y=y;

    grid = Grid(x,y);

    startx = 0;
    starty=0;
    goalx=5;
    goaly=5;

    G = Gene;

    programParticles = new QOpenGLShaderProgram();
    programParticles->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/particles.vert");
    programParticles->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/particles.frag");
    programParticles->bindAttributeLocation("position", 0);
    programParticles->bindAttributeLocation("value", 2);
    programParticles->link();


}


void pathFinding::Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){
    G.DisplayObstacles(programParticles,proj,modelView);
    grid.Display(proj,modelView);  //Display the grid
    G.Display(program,proj,modelView);  //Display the characters

}


void pathFinding::initNodes(){ //Create a list with all the nodes of the grid

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

    if(neighbor->open==0){
        neighbor->gCost = cost;
        neighbor->open = neighbor->gCost + neighbor->hCost;
        neighbor->Parentx = current->x;
        neighbor->Parenty = current->y;
        pQ->push(*neighbor);
    }


}


void pathFinding::aStar(){
    initNodes();
    grid.reset();
    G.Path.clear();

    //Initialize beginning & end points
    grid.map[goalx][goaly] = 3;
    grid.map[startx][starty]=2;
    //Create obstacles
    grid.map[3][2] = 1; G.obstacles.push_back(Sphere(glm::vec3(-size_x+1.0f+2*3,0,-size_y+1.0f+2*2),1.2f));
    grid.map[7][5] = 1; G.obstacles.push_back(Sphere(glm::vec3(-size_x+1.0f+2*7,0,-size_y+1.0f+2*5),1.2f));
    grid.map[8][3] = 1; G.obstacles.push_back(Sphere(glm::vec3(-size_x+1.0f+2*8,0,-size_y+1.0f+2*3),1.2f));
    grid.map[4][4] = 1; G.obstacles.push_back(Sphere(glm::vec3(-size_x+1.0f+2*4,0,-size_y+1.0f+2*4),1.2f));

    node lowestNode = pQueue.top();

    while (lowestNode.x != goalx || lowestNode.y != goaly){

        node CurrentNode = lowestNode;
        pQueue.pop();
        CurrentNode.open = -1;


        //NEIGHBOR 1 - LEFT
        if(CurrentNode.x>0){
            int Nx = CurrentNode.x -1;
            int Ny = CurrentNode.y;
            if(grid.map[Nx][Ny] !=1){
                NodeList[Nx][Ny].movementCost = 10;
                testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
            }
        }

        //NEIGHBOR 2 - RIGHT
        if(CurrentNode.x<(size_x-1)){
            int Nx = CurrentNode.x+1;
            int Ny = CurrentNode.y;
            if(grid.map[Nx][Ny] !=1){
                NodeList[Nx][Ny].movementCost = 10;
                testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
            }
        }

        //NEIGHBOR 3 - UP
        if(CurrentNode.y<(size_y-1)){
            int Nx = CurrentNode.x;
            int Ny = CurrentNode.y+1;
            if(grid.map[Nx][Ny] !=1){
                NodeList[Nx][Ny].movementCost = 10;
                testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
            }
        }

        //NEIGHBOR 4 - DOWN
        if(CurrentNode.y>0){
            int Nx = CurrentNode.x;
            int Ny = CurrentNode.y-1;
            if(grid.map[Nx][Ny] !=1){
                NodeList[Nx][Ny].movementCost = 10;
                testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
            }
        }


        //NEIGHBOR 5 - TOP LEFT
        if(CurrentNode.x>0 && CurrentNode.y<(size_y-1)){
            int Nx = CurrentNode.x -1;
            int Ny = CurrentNode.y +1;
            if(grid.map[Nx][Ny] !=1){
                NodeList[Nx][Ny].movementCost = 14;
                testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
            }
        }

        //NEIGHBOR 6 - TOP RIGHT
        if(CurrentNode.x<(size_x-1) && CurrentNode.y<(size_y-1)){
            int Nx = CurrentNode.x+1;
            int Ny = CurrentNode.y+1;
            if(grid.map[Nx][Ny] !=1){
                NodeList[Nx][Ny].movementCost = 14;
                testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
            }
        }

        //NEIGHBOR 7 - BOTTOM LEFT
        if(CurrentNode.x>0 && CurrentNode.y>0){
            int Nx = CurrentNode.x-1;
            int Ny = CurrentNode.y-1;
            if(grid.map[Nx][Ny] !=1){
                NodeList[Nx][Ny].movementCost = 14;
                testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
            }
        }

        //NEIGHBOR 8 - BOTTOM RIGHT
        if(CurrentNode.x<(size_x-1) && CurrentNode.y>0){
            int Nx = CurrentNode.x+1;
            int Ny = CurrentNode.y-1;
            if(grid.map[Nx][Ny] !=1){
                NodeList[Nx][Ny].movementCost = 14;
                testNeighbor(&CurrentNode,&NodeList[Nx][Ny],&pQueue);
            }
        }


        lowestNode = pQueue.top();
    }

    G.Path.push_back(goalx);
    G.Path.push_back(goaly);

    while(lowestNode.Parentx != startx || lowestNode.Parenty != starty){
        grid.map[lowestNode.Parentx][lowestNode.Parenty] = 2;  //Show path on the grid
        G.Path.push_back(lowestNode.Parentx);
        G.Path.push_back(lowestNode.Parenty);
        lowestNode = NodeList[lowestNode.Parentx][lowestNode.Parenty];
    }

    G.size_x = this->size_x;
    G.size_y = this->size_y;
    G.addPathCharacter(startx,starty);
}



//Test if values chosen in the interface are correct
bool pathFinding::testValueX(int x){
    return (x>=0 && x<size_x);
}

bool pathFinding::testValueY(int y){
    return (y>=0 && y<size_y);
}
