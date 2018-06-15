#include "ex5.h"
#include "Helpers/mesh_importer.h"
#include </usr/local/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/include/glm/glm.hpp>
#include </usr/local/include/glm/gtc/type_ptr.hpp>




ex5::ex5(const QGLFormat &glf, QWidget *parent) : Viewer1(glf, parent)
{
    // init vars
    setFocusPolicy(Qt::StrongFocus);
}



void ex5::initializeGL()
{


    camera_.setWalking();

    // initialize GL function resolution for current context
    initializeGLFunctions();
    initVertexBuffer();

    gShader = new Shader(QString("/Users/Emy/GitHub/FIB/SRGGE/OpenGL_Viewer/shaders/phong.vert"), QString("/Users/Emy/GitHub/FIB/SRGGE/OpenGL_Viewer/shaders/phong.frag"));
    gShader->m_program.bindAttributeLocation("vert", ATTRIB_VERTEX);
    gShader->m_program.bindAttributeLocation("normal" , ATTRIB_NORMAL);


    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glEnable(GL_NORMALIZE);
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    // init state variables
    glClearColor(1.0, 1.0, 1.0, 1.0);	// set background color
    glColor3f   (1.0, 1.0, 0.0);		// set foreground color


    V.clear();
    F.clear();
    N.clear();
    max.clear();
    min.clear();

    num_models=0;


}



void ex5::initVertexBuffer(){


    VAO_M.clear();
    vboVertex.clear();
    vboNormal.clear();
    vboIndex.clear();

    //Compute quads for ground
    computeQuad();

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    //Vertex positions
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()* sizeof(GLfloat),&vertices[0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,faces.size()* sizeof(int),&faces[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    //Compute Walls
    computeWall();

    glGenVertexArrays(1,&WVAO);
    glBindVertexArray(WVAO);

    glGenBuffers(1,&WVBO);
    glBindBuffer(GL_ARRAY_BUFFER,WVBO);
    glBufferData(GL_ARRAY_BUFFER,Wvertices.size()* sizeof(GLfloat),&Wvertices[0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&WEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,WEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,Wfaces.size()* sizeof(int),&Wfaces[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);



    if (V.size() == 0) return;

    std::cout << "loading" << std::endl;


    for(int i=0; i<(int) V.size(); i++){

        GLuint VAO_Mod, vboV, vboN, vboI;

        glGenVertexArrays(1,&VAO_Mod);
        glBindVertexArray(VAO_Mod);

        glGenBuffers(1, &vboV);
        glBindBuffer(GL_ARRAY_BUFFER, vboV);
        glBufferData(GL_ARRAY_BUFFER, V[i].size() * sizeof(float), &V[i][0], GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &vboN);
        glBindBuffer(GL_ARRAY_BUFFER, vboN);
        glBufferData(GL_ARRAY_BUFFER, N[i].size() * sizeof(float), &N[i][0], GL_STATIC_DRAW);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &vboI);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboI);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, F[i].size() * sizeof(int), &F[i][0], GL_STATIC_DRAW);

        VAO_M.push_back(VAO_Mod);
        vboVertex.push_back(vboV);
        vboNormal.push_back(vboN);
        vboIndex.push_back(vboI);

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glBindVertexArray(0);


        }

    num_models=V.size();

}


//Create a square
void ex5::computeQuad(){

    vertices.clear();
    faces.clear();
    normals.clear();

    //Add vertices
    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(0);

    vertices.push_back(1);
    vertices.push_back(0);
    vertices.push_back(0);

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(1);

    vertices.push_back(1);
    vertices.push_back(0);
    vertices.push_back(1);

    //Add faces
    faces.push_back(1);
    faces.push_back(0);
    faces.push_back(2);

    faces.push_back(1);
    faces.push_back(2);
    faces.push_back(3);

    //Add normals facing up
    for(int i = 0; i<4; i++)
    {
        normals.push_back(0);
        normals.push_back(1);
        normals.push_back(0);

    }

}


void ex5::computeWall(){

    Wvertices.clear();
    Wnormals.clear();
    Wfaces.clear();

    Wvertices.push_back(0);
    Wvertices.push_back(0);
    Wvertices.push_back(0);

    Wvertices.push_back(1);
    Wvertices.push_back(0);
    Wvertices.push_back(0);

    Wvertices.push_back(0);
    Wvertices.push_back(1);
    Wvertices.push_back(0);

    Wvertices.push_back(1);
    Wvertices.push_back(1);
    Wvertices.push_back(0);

    Wvertices.push_back(0);
    Wvertices.push_back(0);
    Wvertices.push_back(0);

    Wfaces.push_back(1);
    Wfaces.push_back(0);
    Wfaces.push_back(2);

    Wfaces.push_back(1);
    Wfaces.push_back(2);
    Wfaces.push_back(3);

    for(int i = 0; i<4; i++)
    {
        Wnormals.push_back(0);
        Wnormals.push_back(0);
        Wnormals.push_back(1);

    }


}


//--------------------------------------------------------------------------------

//Check if a 0 is surronded by 4 0 up, down, left & right
bool ex5::checkIE_indiv(std::vector<std::vector<int>> map, int row, int line){

    bool flag_up=false;
    bool flag_down=false;
    bool flag_left=false;
    bool flag_right=false;

    int i=row;
    int j=row;
    int k=line;
    int l=line;

    //UP
    if(row>0){
        while(!flag_up && i>0){
            flag_up=(map[line][i-1]==1);
            i-=1;
        }
    }

    //DOWN
    if(row<((int) map.size())){
        while(!flag_down && j<((int) map.size()-1)){
            flag_down=(map[line][j+1]==1);
            j+=1;
        }
    }

    //LEFT
    if(line>0){
        while(!flag_left && k>0){
            flag_left=(map[k-1][row]==1);
            k-=1;
        }
    }

    //RIGHT
    if(line<((int) map[0].size())){
        while(!flag_right && l<((int) map[0].size()-1)){
            flag_right=(map[l+1][row]==1);
            l+=1;
        }
    }


    return (flag_up&&flag_down&&flag_left&&flag_right);


}



//List all Interior / exterior 0 - double check the interior
void ex5::checkIE(std::vector<std::vector<int>> map){

    IE=Eigen::MatrixXd::Zero(map[0].size(),map.size());

    for(int row=0; row<((int) map.size() -1); row++){
        for(int line=0; line<((int) map[0].size()-1); line++){

            if(map[line][row]!=1){

                bool flag=checkIE_indiv(map,row,line); //True if 0 is found interior, false otherwise

                //If the 0 is exterior, the test is always correct. We check the neighbours of interior ones
                if(flag){

                    //Flags stay false if we don't find any exterior 0 as neighbour (we stop when we find a 1)
                    bool flag_up=false;
                    bool flag_down=false;
                    bool flag_left=false;
                    bool flag_right=false;

                    int i=row;
                    int j=row;
                    int k=line;
                    int l=line;

                    //UP
                    if(row>0){
                        while(map[line][i-1]!=1 && i>0){
                            flag_up=(!checkIE_indiv(map,i-1,line));
                            i-=1;
                        }
                    }

                    //DOWN
                    if(row<((int) map.size())){
                        while(map[line][j+1]!=1 && j<((int) map.size()-1)){
                            flag_down=(!checkIE_indiv(map,j+1,line));
                            j+=1;
                        }
                    }

                    //LEFT
                    if(line>0){
                        while(map[k-1][row]!=1 && k>0){
                            flag_left=(!checkIE_indiv(map,row,k-1));
                            k-=1;
                        }
                    }

                    //RIGHT
                    if(line<((int) map[0].size())){
                        while(map[l+1][row]!=1 && l<((int) map[0].size()-1)){
                            flag_right=(!checkIE_indiv(map,row,l+1));
                            l+=1;
                        }
                    }


                    if(row==2&&line==1){
                        std::cout << flag_up << std::endl;
                        std::cout << flag_down << std::endl;
                        std::cout << flag_left << std::endl;
                        std::cout << flag_right << std::endl;

                    }

                    if(flag_up||flag_down||flag_left||flag_right){  //If we find a 0 exterior neighbour
                        flag=!flag;
                    }
                }

                if(flag)
                    IE(line,row)=1;  //1 if the 0 is a true interior

            }
        }
    }





}


//--------------------------------------------------------------------------------




//Check which walls should be created depending on the neighbours
std::vector<int> ex5::checkWall(std::vector<std::vector<int>> map, int line, int row){

    std::vector<int> wall_map(4);  // UP - DOWN - LEFT - RIGHT


    if(line==0){
        wall_map[0]=1;
    }
    if(line==(int) map.size()-1){
        wall_map[1]=1;
    }
    if(row==0){
        wall_map[2]=1;
    }
    if(row==(int) map[0].size()-1){
        wall_map[3]=1;
    }

    //UP
    if(line!=0 && map[line-1][row]==0 && IE(line-1,row)==0){
        wall_map[0]=1;
    }

    //DOWN
    if(line!=((int) map.size()-1) && map[line+1][row]==0 && IE(line+1,row)==0){
        wall_map[1]=1;
    }

    //LEFT
    if(row!=0 && map[line][row-1]==0 && IE(line,row-1)==0){
         wall_map[2]=1;
    }

    //RIGHT
    if(row!=((int) map[0].size()-1) && map[line][row+1]==0 && IE(line,row+1)==0){
        wall_map[3]=1;
    }


    return wall_map;

}



void ex5::paintGL()
{
    // Render to our framebuffer
    glViewport(0,0,width_,height_); // Render on the whole framebuffer
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if(!museum.empty()){

        // RENDER GEOMETRY

        camera_.SetViewport();

        gShader->use();


        Eigen::Matrix4f projection = camera_.SetProjection();
        Eigen::Matrix4f view = camera_.SetView();
        Eigen::Matrix4f model = camera_.SetModel();

        Eigen::Matrix4f t = view * model;
        Eigen::Matrix3f normal;
        for (int i = 0; i < 3; ++i)
          for (int j = 0; j < 3; ++j) normal(i, j) = t(i, j);

        normal = normal.inverse().transpose();

        for(int i=0; i<(int) museum.size(); i++){
            for(int j=0; j<(int) museum[0].size(); j++){

                //Translation
                Eigen::Affine3f t(Eigen::Translation3f(Eigen::Vector3f(-float(i),-1,-float(j))));
                Eigen::Matrix4f m = t.matrix();
                model = model * m;

                Eigen::Vector3f w = Eigen::Vector3f(0.0,1.0,0.0); // rotation axis
                Eigen::Vector3f Center = Eigen::Vector3f(0.0,0.0,0.0); // center of rotation
                Eigen::Affine3f A = Eigen::Translation3f(Center) * Eigen::AngleAxisf(M_PI/2, w) * Eigen::Translation3f(-Center);


                gShader->setMat4("u_projection",projection);
                gShader->setMat4("u_view",view);
                gShader->setMat4("u_model",model);
                gShader->setMat3("u_normal_matrix",normal);


                //DISPLAY

                if(museum[i][j]==0){
                    Eigen::Vector3f c(3);
                    if(IE(i,j)==1){
                        c[0]=1.0;
                        c[1]=0.0;
                        c[2]=0.0;
                    }
                    else{
                        c[0]=0.0;
                        c[1]=1.0;
                        c[2]=0.0;
                    }
                    gShader->setVec3("color",c);
                    glBindVertexArray(VAO);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
                    glDrawElements(GL_TRIANGLES,faces.size(),GL_UNSIGNED_INT,0);
                    glBindVertexArray(0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
                }


                //Print walls
                if(museum[i][j]==1){

                    Eigen::Vector3f c(1.0,0.0,0.0);
                    gShader->setVec3("color",c);
                    glBindVertexArray(VAO);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
                    glDrawElements(GL_TRIANGLES,faces.size(),GL_UNSIGNED_INT,0);
                    glBindVertexArray(0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


                    c[0]=0.5;
                    c[1]=0.5;
                    c[2]=0.5;
                    gShader->setVec3("color",c);

                    //Get the position of the walls around the tile
                    std::vector<int> Walls = checkWall(museum, i, j);
                    for(int k=0; k<4; k++){

                        //Apply rotations to the quads and display them as walls
                        if(Walls[0]==1){
                            Eigen::Affine3f t(Eigen::Translation3f(Eigen::Vector3f(-1,0,1)));
                            Eigen::Matrix4f R1 = model*A.matrix()*t.matrix();
                            gShader->setMat4("u_model",R1);
                            glBindVertexArray(WVAO);
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,WEBO);
                            glDrawElements(GL_TRIANGLES,Wfaces.size(),GL_UNSIGNED_INT,0);
                            glBindVertexArray(0);
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

                        }

                        if(Walls[1]==1){
                            Eigen::Affine3f t(Eigen::Translation3f(Eigen::Vector3f(-1,0,0)));
                            Eigen::Matrix4f R2 = model*A.matrix()*t.matrix();
                            gShader->setMat4("u_model",R2);
                            glBindVertexArray(WVAO);
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,WEBO);
                            glDrawElements(GL_TRIANGLES,Wfaces.size(),GL_UNSIGNED_INT,0);
                            glBindVertexArray(0);
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

                        }

                        if(Walls[2]==1){
                            Eigen::Affine3f t(Eigen::Translation3f(Eigen::Vector3f(0,0,1)));
                            Eigen::Matrix4f R3=model*t.matrix();
                            gShader->setMat4("u_model",R3);
                            glBindVertexArray(WVAO);
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,WEBO);
                            glDrawElements(GL_TRIANGLES,Wfaces.size(),GL_UNSIGNED_INT,0);
                            glBindVertexArray(0);
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
                        }

                        if(Walls[3]==1){
                            gShader->setMat4("u_model",model);
                            glBindVertexArray(WVAO);
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,WEBO);
                            glDrawElements(GL_TRIANGLES,Wfaces.size(),GL_UNSIGNED_INT,0);
                            glBindVertexArray(0);
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
                        }

                    }
                }


                //Print models
                if(V.size()!=0 && museum[i][j]>1){

                    if (museum[i][j]-1 <= (int) V.size()){

                        //std::cout << VAO_M.size() << std::endl;

                        Eigen::Vector3f c(0.0,0.0,1.0);
                        gShader->setVec3("color",c);
                        glBindVertexArray(VAO);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
                        glDrawElements(GL_TRIANGLES,faces.size(),GL_UNSIGNED_INT,0);
                        glBindVertexArray(0);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


                        float size=fmax(2.5*max[museum[i][j]-2][0],2.5*max[museum[i][j]-2][2]);
                        Eigen::Matrix4f modelMatrix = Eigen::Matrix4f::Identity();
                        Eigen::Affine3f t(Eigen::Translation3f(Eigen::Vector3f(-1*min[museum[i][j]-2][0],-min[museum[i][j]-2][1],-1*min[museum[i][j]-2][2])));
                        modelMatrix(0,0)=1/size;
                        modelMatrix(1,1)=1/size;
                        modelMatrix(2,2)=1/size;
                        modelMatrix = model* modelMatrix*t.matrix();

                        c[0]=1;
                        c[1]=1;
                        c[2]=1;
                        gShader->setVec3("color",c);

                        gShader->setMat4("u_model",modelMatrix);
                        glBindVertexArray(VAO_M[museum[i][j]-2]);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex[museum[i][j]-2]);
                        glDrawElements(GL_TRIANGLES,F[museum[i][j]-2].size(),GL_UNSIGNED_INT,0);
                        glBindVertexArray(0);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


                    }
                }


                //Reset ModelView
                model=camera_.SetModel();


            }

        }

        glUseProgram(0);

    }

}






//--------------------------------------------------------------------------------
//Import txt file with museum data : 0 for ground / 1 for walls

void ex5::loadMap(){

    QString filename;

    filename = QFileDialog::getOpenFileName(this, tr("Load map"), "/Users/Emy/Documents/Cours/Models",
                                              tr("TXT Files ( *.txt )"));
    if (!filename.isNull()) {

        std::string file = filename.toUtf8().constData();
        uint pos = file.find_last_of(".");
        std::string type = file.substr(pos + 1);

         if(!importMap(file))
              QMessageBox::warning(this, tr("Error"),
                              tr("The file could not be opened"));
      }
}



bool ex5::importMap(const std::string &filename){

    museum.clear();


    std::ifstream fin;
    fin.open(filename.c_str());
    if (!fin.is_open() || !fin.good()) return false;

    std::string line;
    for(int i=0; std::getline(fin,line);i++){

        std::stringstream ss;
        ss << line;
        std::vector<int> temp;

        museum.push_back(temp);

        int val;
        while(ss >> val){
            museum[i].push_back(val);
        }
    }

    fin.close();

    initVertexBuffer();
    checkIE(museum);
    paintGL();

    return true;


}


//--------------------------------------------------------------------------------



bool ex5::LoadModel(QString filename) {
  std::string file = filename.toUtf8().constData();
  uint pos = file.find_last_of(".");
  std::string type = file.substr(pos + 1);

  std::unique_ptr<data_representation::TriangleMesh> mesh_temp= std::unique_ptr<data_representation::TriangleMesh>(new data_representation::TriangleMesh);

  bool res = false;
  if (type.compare("ply") == 0) {
    res = data_representation::ReadFromPly(file, mesh_temp.get());
  }

  if (res) {
      V.push_back(mesh_temp->vertices_);
      F.push_back(mesh_temp->faces_);
      N.push_back(mesh_temp->normals_);
      max.push_back(mesh_temp->max_);
      min.push_back(mesh_temp->min_);
      num_models+=1;
      initVertexBuffer();
      paintGL();

      return true;


  }

  return false;
}


void ex5::importModel()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this, tr("Load model"), "/Users/Emy/Documents/Cours/Models",
                                              tr("PLY Files ( *.ply )"));
    if (!filename.isNull()) {
         if(!LoadModel(filename))
              QMessageBox::warning(this, tr("Error"),
                              tr("The file could not be opened"));
      }

}




QGroupBox* ex5::controlPanel()
{
    // init group box
    QGroupBox *groupBox = HW::controlPanel();
    groupBox->setStyleSheet(GroupBoxStyle);

    QPushButton *buttonImport  = new QPushButton("Import model");
    //QPushButton *buttonExport  = new QPushButton("Export model");

//    mesh_importer.AddImportExportPLY(groupBox);

    QPushButton *Load = new QPushButton("Load Map");

    connect(Load,SIGNAL(clicked()),this,SLOT(loadMap()));
    connect(buttonImport, SIGNAL(clicked()), this, SLOT(importModel()));

    //Display
    auto layout = dynamic_cast<QGridLayout*>(groupBox->layout());
    int row = layout->rowCount() + 1;
    row++;
    layout->addWidget(buttonImport,row,0);
    row++;
    row++;
    layout->addWidget(Load,row,0);
    row++;

    layout->setRowStretch(row,1);

    return(groupBox);

}







