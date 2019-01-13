#include "grid.h"


Grid::Grid(int x, int y)
{
    size_x = x;
    size_y = y;
    ground = Plane(glm::vec3(0,0,0),glm::vec3(0,1,0));
    ground.initBuffers();

    map.resize(size_x);
    for ( int i = 0 ; i < size_x ; i++ )
       map[i].resize(size_y);

    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/grid.vert");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/grid.frag");
    program->bindAttributeLocation("position", 0);
    program->link();
    if(!program->isLinked())
    {
            std::cout << "Shader program has not linked" << std::endl << std::endl << "Log: " << endl << endl << program->log().toStdString();
    }

}

Grid::~Grid()
{
}


void Grid::reset(){
    map.clear();

    map.resize(size_x);
    for ( int i = 0 ; i < size_x ; i++ )
       map[i].resize(size_y);

}


void Grid::Display(QMatrix4x4 proj, QMatrix4x4 modelView)
{
    program->bind();

    program->setUniformValue("projection", proj);

    QMatrix4x4 MVtemp = modelView;

    for(int i=0; i<size_x; i++){
        for(int j=0; j<size_y; j++){

            if(map[i][j]==0){
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
                MVtemp.translate(-size_x+2*i+1.0f,0,-size_y+2*j+1.0f);
                program->setUniformValue("modelview",MVtemp);
                program->setUniformValue("color",0.0,0.0,0.0,1.0);

                glBindVertexArray(ground.planeVAO);
                glDrawArrays(GL_TRIANGLE_FAN,0,4);
                glBindVertexArray(0);

                MVtemp=modelView;
            }


            else if(map[i][j]==2){  //COLOR IN BLUE (PATH)
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
                MVtemp.translate(-size_x+2*i+1.0f,0,-size_y+2*j+1.0f);
                program->setUniformValue("modelview",MVtemp);
                program->setUniformValue("color",0.0,0.2,0.8,0.8);

                glBindVertexArray(ground.planeVAO);
                glDrawArrays(GL_TRIANGLE_FAN,0,4);
                glBindVertexArray(0);

                MVtemp=modelView;
            }


            else if(map[i][j]==3){  //COLOR IN GREEN (GOAL)
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
                MVtemp.translate(-size_x+2*i+1.0f,0,-size_y+2*j+1.0f);
                program->setUniformValue("modelview",MVtemp);
                program->setUniformValue("color",0.0,0.8,0.2,1.0);

                glBindVertexArray(ground.planeVAO);
                glDrawArrays(GL_TRIANGLE_FAN,0,4);
                glBindVertexArray(0);

                MVtemp=modelView;
            }

            else{  //COLOR IN RED
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
                MVtemp.translate(-size_x+2*i+1.0f,0,-size_y+2*j+1.0f);
                program->setUniformValue("modelview",MVtemp);
                program->setUniformValue("color",1.0,0.0,0.0,1.0);

                glBindVertexArray(ground.planeVAO);
                glDrawArrays(GL_TRIANGLE_FAN,0,4);
                glBindVertexArray(0);

                MVtemp=modelView;
            }
        }
    }

    program->release();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

}
