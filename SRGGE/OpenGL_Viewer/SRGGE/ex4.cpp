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


void ex4::initializeGL()
{

    // initialize GL function resolution for current context
    initializeGLFunctions();

    gShader = new Shader(QString("/Users/Emy/GitHub/FIB/SRGGE/OpenGL_Viewer/shaders/try_ex4.vert"), QString("/Users/Emy/GitHub/FIB/SRGGE/OpenGL_Viewer/shaders/try_ex4.frag"));
    gShader->m_program.bindAttributeLocation("vert", ATTRIB_VERTEX);
    gShader->m_program.bindAttributeLocation("normal" , ATTRIB_NORMAL);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    // init state variables
    glClearColor(1.0, 1.0, 1.0, 1.0);	// set background color
    glColor3f   (1.0, 1.0, 0.0);		// set foreground color

}




float ex4::computeIndivCost(Eigen::Matrix4f view, Eigen::Matrix4f model, int i, int j, int L){

    float size=2*mesh_->max_[0];

    Eigen::Affine3f t(Eigen::Translation3f(Eigen::Vector3f((-copies/2*size)+2*size*float(i),0,(-copies/2*size)+2*size*float(j))));
    Eigen::Matrix4f m = t.matrix();
    model = model * m;

    Eigen::Vector3f diag = mesh_->max_ - mesh_->min_; //Diagonal of bounding box
    Eigen::Vector3f Center = diag/2; //Center of bounding box

    Eigen::Vector4f viewCenter = view*model*Eigen::Vector4f(Center[0],Center[1],Center[2],1.0); //Change the center into view space to get the distance from the viewport
    Eigen::Vector3f C(viewCenter[0],viewCenter[1],viewCenter[2]);

    float d1 = diag.norm();  //diagonal d
    float d2 = C.norm();  //distance D

    //Compute value of cost
    return d1/(pow(2,L)*d2);

}



//Store the 5 levels of details obtained with code of exercise 2
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



//Find the index of the model that has the lowest difference of cost when we reduce its LOD
std::vector<int> ex4::findIDmin(Eigen::Matrix4f view, Eigen::Matrix4f model){

    int n = Levels.rows();

    float minCost = 0;
    std::vector<int> index(2,0);
    index[0]=-1;
    index[1]=-1;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(Levels(i,j)>0){
                if(index[0]==-1){  //First time in the loop : minimum = first value
                    minCost=computeIndivCost(view, model, i,j,Levels(i,j)-1)-computeIndivCost(view, model, i,j,Levels(i,j));
                    index[0]=i;
                    index[1]=j;
                }
                else{
                    //Compute the difference between the cost of the model at the actual level and the cost of the model at the level-1
                    float diff = computeIndivCost(view, model, i,j,Levels(i,j)-1)-computeIndivCost(view, model, i,j,Levels(i,j));
                    if(diff<minCost){
                        minCost = diff;  //Find the smallest cost
                        index[0]=i;  //Keep the index of the model with the smallest cost (i,j) between the NxN models displayed
                        index[1]=j;
                    }
                }
            }
        }
    }

    return index;
}



//Find the index of the model that is the best to increment
std::vector<int> ex4::findIDmax(Eigen::Matrix4f view, Eigen::Matrix4f model){

    int n = Levels.rows();

    float maxCost = 0;
    std::vector<int> index(2,0);
    index[0]=-1;
    index[1]=-1;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(Levels(i,j)>=0 && Levels(i,j)<4){
                if(index[0]==-1){  //First time in the loop : minimum = first value
                    maxCost=computeIndivCost(view, model, i,j,Levels(i,j))-computeIndivCost(view, model, i,j,Levels(i,j)+1);
                    index[0]=i;
                    index[1]=j;
                }
                else{
                    //Compute the difference between the cost of the model at the actual level and the cost of the model at the level+1
                    float diff = computeIndivCost(view, model, i,j,Levels(i,j))-computeIndivCost(view, model, i,j,Levels(i,j)+1);
                    if(diff>maxCost){
                        maxCost = diff;
                        index[0]=i;
                        index[1]=j;
                    }
                }
            }
        }
    }

    return index;
}



//Count the nb of triangles of all the generated models -> control the fps value
void ex4::totTriangles(){

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

    nb_triangles = tot;

}


//Computes the list of LOD for each NxN models
void ex4::computeLevels(Eigen::Matrix4f view, Eigen::Matrix4f model){

    totTriangles();

    //While the nb of triangles is bigger than 5 000 000 to assure a framerate of 60fps
    if(nb_triangles>5000000 && Levels!=Eigen::MatrixXd::Zero(copies,copies)){

        std::vector<int> index = findIDmin(view, model);  //Find the model with the smallest cost
        int Row = index[0];
        int Col = index[1];

        if(Row==-1 || Col==-1)
            return;

        if(Levels(Row,Col)!=0){
            Levels(Row,Col)-=1;  //If the model is not already minimum then decrease it
            totTriangles();  //Update the total numbers of triangles
        }


    }

    else{

        std::vector<int> index = findIDmax(view, model);  //Find the model with the smallest cost
        int Row = index[0];
        int Col = index[1];

        if(Row==-1 || Col==-1)
            return;

        if(Levels(Row,Col)!=4){
            Levels(Row,Col)+=1;  //If the model is not already minimum then decrease it
            totTriangles();
        }

    }

}



//-----------------------------------------------------------------------------------------------
// -------------------------------- Advanced - hysteresis ---------------------------------------
//-----------------------------------------------------------------------------------------------


std::vector<int> ex4::IDAdvanced(Eigen::Matrix4f view, Eigen::Matrix4f model, int frame){

    int n = Levels.rows();

    float minCost = 0;
    std::vector<int> index(2,0);
    index[0]=-1;
    index[1]=-1;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(Levels(i,j)>0){
                if(index[0]==-1){  //First time in the loop : minimum = first value
                    minCost=computeIndivCost(view, model, i,j,Levels(i,j)-1)-computeIndivCost(view, model, i,j,Levels(i,j));
                    index[0]=i;
                    index[1]=j;
                }
                else{
                    //Compute the difference between the cost of the model at the actual level and the cost of the model at the level-1
                    float diff = computeIndivCost(view, model, i,j,Levels(i,j)-1)-computeIndivCost(view, model, i,j,Levels(i,j));
                    if(diff<minCost && frame-Model_Frames(i,j)>10){
                        if(Model_Frames(i,j)!=0){
                            std::cout << "here" << std::endl;
                        }
                        minCost = diff;  //Find the smallest cost
                        index[0]=i;  //Keep the index of the model with the smallest cost (i,j) between the NxN models displayed
                        index[1]=j;
                    }
                }
            }
        }
    }

    return index;
}




//Computes the list of LOD for each NxN models with freezing changes of models for 10 frames
void ex4::computeAdvanced(Eigen::Matrix4f view, Eigen::Matrix4f model,int frame){

    totTriangles();

    //While the nb of triangles is bigger than 5 000 000 to assure a framerate of 60fps
    if(nb_triangles>5000000 && Levels!=Eigen::MatrixXd::Zero(copies,copies)){

        std::vector<int> index = findIDmin(view, model);  //Find the model with the smallest cost
        int Row = index[0];
        int Col = index[1];

        if(Row==-1 || Col==-1)
            return;

        if(Levels(Row,Col)!=0 && frame-Model_Frames(Row,Col)>10){
            Levels(Row,Col)-=1;  //If the model is not already minimum then decrease it
            totTriangles();  //Update the total numbers of triangles
            Model_Frames(Row,Col)=frame;  //Save last update of the model
        }


    }

    else{

        std::vector<int> index = findIDmax(view, model);  //Find the model with the smallest cost
        int Row = index[0];
        int Col = index[1];

        if(Row==-1 || Col==-1)
            return;

        if(Levels(Row,Col)!=4 && frame-Model_Frames(Row,Col)>10){
            Levels(Row,Col)+=1;  //If the model is not already minimum then decrease it
            totTriangles();
            Model_Frames(Row,Col)=frame;  //Save last update of the model
        }

    }


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


        //Levels=4*Eigen::MatrixXd::Ones(copies,copies);
        //Model_Frames=Eigen::MatrixXd::Zero(copies,copies);

        if(!AdvancedON)
            computeLevels(view, model);
        else
            computeAdvanced(view,model,count_frames);



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

                //RENDER MODELS depending on the level of details they should have

                if(Levels(i,j)==0){
                    gShader->setInt("level",Levels(i,j));
                    glBindVertexArray(vao0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex0);
                    glDrawElements(GL_TRIANGLES,F[0].size(),GL_UNSIGNED_INT,0);
                    glBindVertexArray(0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
                }
                if(Levels(i,j)==1){
                    gShader->setInt("level",Levels(i,j));
                    glBindVertexArray(vao1);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex1);
                    glDrawElements(GL_TRIANGLES,F[1].size(),GL_UNSIGNED_INT,0);
                    glBindVertexArray(0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
                }
                if(Levels(i,j)==2){
                    gShader->setInt("level",Levels(i,j));
                    glBindVertexArray(vao2);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex2);
                    glDrawElements(GL_TRIANGLES,F[2].size(),GL_UNSIGNED_INT,0);
                    glBindVertexArray(0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
                }
                if(Levels(i,j)==3){
                    gShader->setInt("level",Levels(i,j));
                    glBindVertexArray(vao3);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex3);
                    glDrawElements(GL_TRIANGLES,F[3].size(),GL_UNSIGNED_INT,0);
                    glBindVertexArray(0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
                }
                if(Levels(i,j)==4){
                    gShader->setInt("level",Levels(i,j));
                    glBindVertexArray(vao4);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex4);
                    glDrawElements(GL_TRIANGLES,F[4].size(),GL_UNSIGNED_INT,0);
                    glBindVertexArray(0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
                }


                //Reset ModelView
                model=camera_.SetModel();
            }
        }

        glUseProgram(0);

    }


    //Set Framerate

    frames++;
    count_frames++;
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


    computeVFN();
    Levels=Eigen::MatrixXd::Ones(copies,copies);
    Levels=4*Levels;

    //Send the 5 LOD to 5 different VAO

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

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    //------------- 1 -----------------

    glGenVertexArrays(1,&vao1);
    glBindVertexArray(vao1);

    //Vertex positions
    glGenBuffers(1,&vboVertex1);
    glBindBuffer(GL_ARRAY_BUFFER,vboVertex1);
    glBufferData(GL_ARRAY_BUFFER,V[1].size()* sizeof(GLfloat),&V[1][0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
    glEnableVertexAttribArray(0);

    //Vertex normals
    glGenBuffers(1,&vboNormal1);
    glBindBuffer(GL_ARRAY_BUFFER,vboNormal1);
    glBufferData(GL_ARRAY_BUFFER,N[1].size()* sizeof(float),&N[1][0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&vboIndex1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,F[1].size()* sizeof(int),&F[1][0],GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    //------------- 2 -----------------

    glGenVertexArrays(1,&vao2);
    glBindVertexArray(vao2);

    //Vertex positions
    glGenBuffers(1,&vboVertex2);
    glBindBuffer(GL_ARRAY_BUFFER,vboVertex2);
    glBufferData(GL_ARRAY_BUFFER,V[2].size()* sizeof(GLfloat),&V[2][0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
    glEnableVertexAttribArray(0);

    //Vertex normals
    glGenBuffers(1,&vboNormal2);
    glBindBuffer(GL_ARRAY_BUFFER,vboNormal2);
    glBufferData(GL_ARRAY_BUFFER,N[2].size()* sizeof(float),&N[2][0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&vboIndex2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,F[2].size()* sizeof(int),&F[2][0],GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);

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


    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    //------------- 4 -----------------


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

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}






QGroupBox* ex4::controlPanel()
{
    // init group box
    QGroupBox *groupBox = ex1::controlPanel();
    groupBox->setStyleSheet(GroupBoxStyle);

    QButtonGroup *buttonGroup = new QButtonGroup;
    QRadioButton *Basic = new QRadioButton("Basic");
    QRadioButton *Advanced = new QRadioButton("Advanced");
    Basic -> setChecked(true);
    buttonGroup -> addButton(Basic);
    buttonGroup -> addButton(Advanced);

    //SIGNALS
    connect(Basic,SIGNAL(toggled(bool)),this,SLOT(setAdvancedOFF()));
    connect(Advanced,SIGNAL(toggled(bool)),this,SLOT(setAdvancedON()));


    //LAYOUT
    auto layout = dynamic_cast<QGridLayout*>(groupBox->layout());
    int row = layout->rowCount() + 1;

    row++;
    layout->addWidget(Basic,row,0);
    layout->addWidget(Advanced,row,1);
    row++;
    layout->setRowStretch(row,1);


    return(groupBox);

}




void ex4::setNumberCopies(int N)
{
    copies=N;
    Levels=4*Eigen::MatrixXd::Ones(copies,copies);
    Model_Frames=Eigen::MatrixXd::Zero(copies,copies);
    //count_frames=0;
    paintGL();
}


void ex4::setAdvancedON()
{
    AdvancedON=true;
    //count_frames=0;
    Levels=4*Eigen::MatrixXd::Ones(copies,copies);
    Model_Frames=Eigen::MatrixXd::Zero(copies,copies);
    paintGL();
}

void ex4::setAdvancedOFF()
{
    AdvancedON=false;
    //count_frames=0;
    Levels=4*Eigen::MatrixXd::Ones(copies,copies);
    Model_Frames=Eigen::MatrixXd::Zero(copies,copies);
    paintGL();
}
