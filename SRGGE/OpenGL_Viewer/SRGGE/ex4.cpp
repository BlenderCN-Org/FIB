#include "ex4.h"
#include "Helpers/mesh_importer.h"
#include "Helpers/mesh_io.h"
#include </usr/local/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/include/glm/glm.hpp>
#include </usr/local/include/glm/gtc/type_ptr.hpp>
#include "time.h"
#include <cmath>


ex4::ex4(const QGLFormat &glf, QWidget *parent) : ex2(glf, parent)
{

    // init vars
    setFocusPolicy(Qt::StrongFocus);


}



float ex4::computeIndivCost(int i, int j, int L){

    float size=2*mesh_->max_[0];

    Eigen::Matrix4f view = camera_.SetView();
    Eigen::Matrix4f model = camera_.SetModel();

    Eigen::Affine3f t(Eigen::Translation3f(Eigen::Vector3f((-copies/2*size)+2*size*float(i),0,(-copies/2*size)+2*size*float(j))));
    Eigen::Matrix4f m = t.matrix();
    model = model * m;

    Eigen::Vector3f diag = mesh_->max_ - mesh_->min_;
    Eigen::Vector3f Center = diag/2;
    Eigen::Vector4f viewDiag = view*model*Eigen::Vector4f(diag[0],diag[1],diag[2],1.0);
    Eigen::Vector4f viewCenter = view*model*Eigen::Vector4f(Center[0],Center[1],Center[2],1.0);

    Eigen::Vector3f D(viewDiag[0],viewDiag[1],viewDiag[2]);
    Eigen::Vector3f C(viewCenter[0],viewCenter[1],viewCenter[2]);

    float d1 = D.norm();  //diagonal d
    float d2 = C.norm();  //distance  D

    return d1/(pow(2,L)*d2);

}




void ex4::computeVFN(){

    V.clear();
    F.clear();
    N.clear();

    computeGrid(8);
    V.push_back(new_vertices);
    F.push_back(new_faces);
    N.push_back(new_normals);
    computeGrid(16);
    V.push_back(new_vertices);
    F.push_back(new_faces);
    N.push_back(new_normals);
    computeGrid(32);
    V.push_back(new_vertices);
    F.push_back(new_faces);
    N.push_back(new_normals);
    computeGrid(64);
    V.push_back(new_vertices);
    F.push_back(new_faces);
    N.push_back(new_normals);

    V.push_back(mesh_->vertices_);
    F.push_back(mesh_->faces_);
    N.push_back(mesh_->normals_);


}


Eigen::MatrixXd ex4::computeCost(Eigen::MatrixXd Levels){

    int n=Levels.rows();
    Eigen::MatrixXd Cost = Eigen::MatrixXd::Zero(n,n);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            Cost(i,j)=computeIndivCost(i,j,Levels(i,j));
        }
    }

    return Cost;

}


int ex4::totTriangles(Eigen::MatrixXd Levels){

    int num_faces_0 = F[0].size()/3;
    int num_faces_1 = F[1].size()/3;
    int num_faces_2 = F[2].size()/3;
    int num_faces_3 = F[3].size()/3;
    int num_faces_4 = F[4].size()/3;

    int tot = 0;
    int n = Levels.rows();

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(Levels(i,j)==0)
                tot+=num_faces_0;
            else if(Levels(i,j)==1)
                tot+=num_faces_1;
            else if(Levels(i,j)==2)
                tot+=num_faces_2;
            else if(Levels(i,j)==3)
                tot+=num_faces_3;
            else if(Levels(i,j)==4)
                tot+=num_faces_4;
        }
    }

    return tot;

}



Eigen::MatrixXd ex4::computeLevels(Eigen::MatrixXd Levels){


    Eigen::MatrixXd Cost = computeCost(Levels);

    int it=0;
    while(totTriangles(Levels)>5000000 && Levels!=Eigen::MatrixXd::Zero(copies,copies) && it<5000){

        Eigen::MatrixXf::Index Row,Col;
        float max = Cost.maxCoeff(&Row,&Col);

        if(Levels(Row,Col)!=0){
            Levels(Row,Col)-=1;
            Cost=computeCost(Levels);
            totTriangles(Levels);
        }
        it+=1;
    }


    std::cout << Levels << std::endl;

    return Levels;


}



void ex4::paintGL()
{


    // Render to our framebuffer
    glViewport(0,0,width_,height_); // Render on the whole framebuffer
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mesh_ != nullptr) {


        camera_.SetViewport();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gShader->use();

        Eigen::Matrix4f projection = camera_.SetProjection();
        Eigen::Matrix4f view = camera_.SetView();
        Eigen::Matrix4f model = camera_.SetModel();

        Eigen::Matrix4f t = view * model;
        Eigen::Matrix3f normal;
        for (int i = 0; i < 3; ++i)
          for (int j = 0; j < 3; ++j) normal(i, j) = t(i, j);

        normal = normal.inverse().transpose();

        float size=2*mesh_->max_[0];

        for(int i =0; i<copies; i++){

            for(int j=0; j<copies; j++){


                //Translation
                Eigen::Affine3f t(Eigen::Translation3f(Eigen::Vector3f((-copies/2*size)+2*size*float(i),0,(-copies/2*size)+2*size*float(j))));
                Eigen::Matrix4f m = t.matrix();
                model = model * m;

                gShader->setMat4("u_projection",projection);
                gShader->setMat4("u_view",view);
                gShader->setMat4("u_model",model);
                gShader->setMat3("u_normal_matrix",normal);

                //RENDER MODEL

                if(Levels(i,j)==0){
                    glBindVertexArray(vao0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex0);
                    glDrawElements(GL_TRIANGLES,F[0].size(),GL_UNSIGNED_INT,0);
                    //std::cout << "level 0" << std::endl;
                }
                if(Levels(i,j)==1){
                    glBindVertexArray(vao1);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex1);
                    glDrawElements(GL_TRIANGLES,F[1].size(),GL_UNSIGNED_INT,0);
                    //std::cout << "level 1" << std::endl;
                }
                if(Levels(i,j)==2){
                    glBindVertexArray(vao2);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex2);
                    glDrawElements(GL_TRIANGLES,F[2].size(),GL_UNSIGNED_INT,0);
                    //std::cout << "level 2" << std::endl;
                }
                if(Levels(i,j)==3){
                    glBindVertexArray(vao3);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex3);
                    glDrawElements(GL_TRIANGLES,F[3].size(),GL_UNSIGNED_INT,0);
                    //std::cout << "level 3" << std::endl;
                }
                if(Levels(i,j)==4){
                    glBindVertexArray(vao4);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex4);
                    glDrawElements(GL_TRIANGLES,F[4].size(),GL_UNSIGNED_INT,0);
                    //std::cout << "level 4" << std::endl;
                }

                //Reset ModelView
                model=camera_.SetModel();
            }
        }

        glUseProgram(0);

    }


    //Set Framerate

    frames++;
    final_time=time(NULL);
    if(final_time-initial_time>0)
    {
        emit SetFramerate(QString::number(frames/(final_time-initial_time)));
        frames=0;
        initial_time=final_time;
    }

}





void ex4::initVertexBuffer()
{


    if (mesh_ == nullptr) return;


//    glGenVertexArrays(1,&vao);
//    glBindVertexArray(vao);

//    static bool flag=1;

//    if(flag){


//        glGenBuffers(1,&vboVertex1);
//        glGenBuffers(1,&vboVertex2);
//        glGenBuffers(1,&vboVertex3);
//        glGenBuffers(1,&vboVertex4);

//        glGenBuffers(1,&vboNormal1);
//        glGenBuffers(1,&vboNormal2);
//        glGenBuffers(1,&vboNormal3);
//        glGenBuffers(1,&vboNormal4);

//        glGenBuffers(1,&vboIndex1);
//        glGenBuffers(1,&vboIndex2);
//        glGenBuffers(1,&vboIndex3);
//        glGenBuffers(1,&vboIndex4);

//        flag=0;
//    }


    computeVFN();

    Levels=Eigen::MatrixXd::Ones(copies,copies);
    Levels=4*Levels;

    //std::cout << Levels << std::endl;


    //------------- 0 -----------------

    glGenVertexArrays(1,&vao0);
    glBindVertexArray(vao0);

    //Vertex positions
    glGenBuffers(1,&vboVertex0);
    glBindBuffer(GL_ARRAY_BUFFER,vboVertex0);
    glBufferData(GL_ARRAY_BUFFER,V[0].size()* sizeof(GLfloat),&V[0][0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
    glEnableVertexAttribArray(0);

    //Vertex normals
    glGenBuffers(1,&vboNormal0);
    glBindBuffer(GL_ARRAY_BUFFER,vboNormal0);
    glBufferData(GL_ARRAY_BUFFER,N[0].size()* sizeof(float),&N[0][0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&vboIndex0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,F[0].size()* sizeof(int),&F[0][0],GL_STATIC_DRAW);


    //------------- 1 -----------------

    glGenVertexArrays(1,&vao1);
    glBindVertexArray(vao1);

    //Vertex positions
    glBindBuffer(GL_ARRAY_BUFFER,vboVertex1);
    glBufferData(GL_ARRAY_BUFFER,V[1].size()* sizeof(GLfloat),&V[1][0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
    glEnableVertexAttribArray(0);

    //Vertex normals
    glBindBuffer(GL_ARRAY_BUFFER,vboNormal1);
    glBufferData(GL_ARRAY_BUFFER,N[1].size()* sizeof(float),&N[1][0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,F[1].size()* sizeof(int),&F[1][0],GL_STATIC_DRAW);

    //------------- 2 -----------------

    glGenVertexArrays(1,&vao2);
    glBindVertexArray(vao2);

    //Vertex positions
    glBindBuffer(GL_ARRAY_BUFFER,vboVertex2);
    glBufferData(GL_ARRAY_BUFFER,V[2].size()* sizeof(GLfloat),&V[2][0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
    glEnableVertexAttribArray(0);

    //Vertex normals
    glBindBuffer(GL_ARRAY_BUFFER,vboNormal2);
    glBufferData(GL_ARRAY_BUFFER,N[2].size()* sizeof(float),&N[2][0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,F[2].size()* sizeof(int),&F[2][0],GL_STATIC_DRAW);

    //------------- 3 -----------------

    glGenVertexArrays(1,&vao3);
    glBindVertexArray(vao3);

    //Vertex positions
    glGenBuffers(1,&vboVertex3);
    glBindBuffer(GL_ARRAY_BUFFER,vboVertex3);
    glBufferData(GL_ARRAY_BUFFER,V[3].size()* sizeof(GLfloat),&V[3][0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
    glEnableVertexAttribArray(0);

    //Vertex normals
    glGenBuffers(1,&vboNormal3);
    glBindBuffer(GL_ARRAY_BUFFER,vboNormal3);
    glBufferData(GL_ARRAY_BUFFER,N[3].size()* sizeof(float),&N[3][0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&vboIndex3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,F[3].size()* sizeof(int),&F[3][0],GL_STATIC_DRAW);

//    //------------- 4 -----------------


    glGenVertexArrays(1,&vao4);
    glBindVertexArray(vao4);

    //Vertex positions
    glGenBuffers(1,&vboVertex4);
    glBindBuffer(GL_ARRAY_BUFFER,vboVertex4);
    glBufferData(GL_ARRAY_BUFFER,V[4].size()* sizeof(GLfloat),&V[4][0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
    glEnableVertexAttribArray(0);

    //Vertex normals
    glGenBuffers(1,&vboNormal4);
    glBindBuffer(GL_ARRAY_BUFFER,vboNormal4);
    glBufferData(GL_ARRAY_BUFFER,N[4].size()* sizeof(float),&N[4][0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&vboIndex4);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex4);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,F[4].size()* sizeof(int),&F[4][0],GL_STATIC_DRAW);


    glBindVertexArray(0);

}






QGroupBox* ex4::controlPanel()
{
    // init group box
    QGroupBox *groupBox = ex1::controlPanel();
    groupBox->setStyleSheet(GroupBoxStyle);


    return(groupBox);

}




void ex4::setNumberCopies(int N)
{
    copies=N;
    Levels=4*Eigen::MatrixXd::Ones(copies,copies);
    paintGL();
}

