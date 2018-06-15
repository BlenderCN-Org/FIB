#include "ex6.h"
#include "ex5.h"
#include "Helpers/mesh_importer.h"
#include </usr/local/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/include/glm/glm.hpp>
#include </usr/local/include/glm/gtc/type_ptr.hpp>



ex6::ex6(const QGLFormat &glf, QWidget *parent) : ex5(glf, parent)
{
    // init vars
    setFocusPolicy(Qt::StrongFocus);
}





//Select one side between 0 & 3 and return random coordinates on this side
std::vector<int> ex6::ReturnCoords(int side){

    int n=museum.size();
    int m=museum[0].size();
    std::vector<int> x(2,0);

    //0 Up
    if(side==0){
        x[0]=0;
        x[1]=std::rand()%n;
    }

    //1 Left
    if(side==1){
        x[0]=std::rand()%m;
        x[1]=0;
    }

    //2 Down
    if(side==2){
        x[0]=m-1;
        x[1]=std::rand()%n;
    }

    //3 Right
    if(side==3){
        x[0]=std::rand()%m;
        x[1]=n-1;
    }

    return x;
}




//Select 2 different sides and gives the random coordinates of 2 points on each side to ray function
void ex6::SelectSides(){

    std::vector<std::vector<int>> Coords;
    std::vector<int> S(2,0);

    do{
        S[0] = std::rand()%4;
        S[1] = std::rand()%4;
    } while(S[0]==S[1]);

    Coords.push_back(ReturnCoords(S[0]));
    Coords.push_back(ReturnCoords(S[1]));

    Rays(Coords[0][0],Coords[0][1],Coords[1][0],Coords[1][1]);
}



void ex6::Rays(int x1, int y1, int x2, int y2){

    int i;
    int ystep, xstep;
    int error;
    int errorprev;
    int y = y1, x = x1;
    int ddy, ddx;
    int dx = x2 - x1;
    int dy = y2 - y1;
    visibility.clear();
    last=false;

    //point 1
    addPoint(x1, y1);

    if (dy < 0){
      ystep = -1;
      dy = -dy;
    }else
      ystep = 1;

    if (dx < 0){
      xstep = -1;
      dx = -dx;
    }else
      xstep = 1;

    ddy = 2 * dy;
    ddx = 2 * dx;

    if (ddx >= ddy){  // first octant (0 <= slope <= 1)

        error = dx;
        errorprev = error;  // start in the middle of the square
        for (i=0 ; i < dx ; i++){  // do not use the first point (already done)
          x += xstep;
          error += ddy;
          if (error > ddx){  // increment y if AFTER the middle ( > )
            y += ystep;
            error -= ddx;
            // three cases (octant == right->right-top for directions below):
            if (error + errorprev < ddx)  // bottom square also
                addPoint(x, y-ystep);
            else if (error + errorprev > ddx)  // left square also
                addPoint(x-xstep, y);
            else{  // corner: bottom and left squares also
                addPoint(x, y-ystep);
                addPoint(x-xstep, y);
            }
          }
          addPoint(x, y);
          errorprev = error;
        }
      }else{  // the same as above
        errorprev = error = dy;
        for (i=0 ; i < dy ; i++){
          y += ystep;
          error += ddx;
          if (error > ddy){
            x += xstep;
            error -= ddy;
            if (error + errorprev < ddy)
                addPoint(x-xstep, y);
            else if (error + errorprev > ddy)
                addPoint(x, y-ystep);
            else{
                addPoint(x-xstep, y);
                addPoint(x, y-ystep);
            }
          }
          addPoint(x, y);
          errorprev = error;
        }
      }

    //adding again the last point to compute his visibility
    if((y == y2) && (x == x2))
    {
        last = true;
        addPoint(x,y);
    }


}



//Add point to visibility set
void ex6::addPoint(int x, int y){

    //Add points to actual set
    visibility.push_back(x);
    visibility.push_back(y);


    //If we reach a wall, we create a new visibility set
    if(museum[x][y]==1 || last){
        sendSet();
        visibility.clear();
        visibility.push_back(x);
        visibility.push_back(y);

    }
}


//Update set of each cell
void ex6::sendSet(){

    for(int k=0; k<((int) visibility.size()-1);k+=2){

        int i = visibility[k];
        int j = visibility[k+1];

        for(int id=0; id<((int) visibility.size()-1);id+=2){
            addAndCheck(i,j,visibility[id],visibility[id+1]);
        }
    }

    visibility.clear();


}


//Add values to visibility and check for repetition
void ex6::addAndCheck(int i, int j, int a, int b){

    for(int k=0; k<((int) visibility.size()-1); k+=2){

        if(VCells[i][j][k]!=a && VCells[i][j][k+1]!=b){
            VCells[i][j].push_back(a);
            VCells[i][j].push_back(b);
        }
    }



}



//Compute global visibility
void ex6::Visibility(int r){

    VCells.clear();
    visibility.clear();

    //Create VCell vector : Visibility sets for each cell of the museum
    int n = museum.size();
    int m = museum[0].size();

    for(int i=0; i<n; i++){

        std::vector<std::vector<int>> vec;

        for(int j=0; j<m; j++){

            //Initialize each visibility cell by coordinates of the cell
            std::vector<int> vec_temp;
            vec_temp.push_back(i);
            vec_temp.push_back(j);
            vec.push_back(vec_temp);

        }

        VCells.push_back(vec);

    }


    //Trace r rays
    for(int ray=0; ray<r; ray++){
        SelectSides();
    }

    std::cout << "Visibility computed" << std::endl;

}







void ex6::paintGL()
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
        Eigen::Vector2f position = camera_.GetPosition();
        int camera_x = (int) position[0];
        int camera_y = (int) position[1];

        Eigen::Matrix4f t = view * model;
        Eigen::Matrix3f normal;
        for (int i = 0; i < 3; ++i)
          for (int j = 0; j < 3; ++j) normal(i, j) = t(i, j);

        normal = normal.inverse().transpose();

        if(camera_x < 0 || camera_y < 0 || camera_x >= museum.size() || camera_y >= museum[0].size()){
            ex5::paintGL();
        }
        else{

            if(VCells[camera_x][camera_y].size()==0)
                std::cout << "No visibility" << std::endl;


            else{


                for(int k=0; k<((int) VCells[camera_x][camera_y].size()-1); k+=2){

                    int i=VCells[camera_x][camera_y][k];
                    int j=VCells[camera_x][camera_y][k+1];

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

}











void ex6::loadMap(){

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


bool ex6::importMap(const std::string &filename){

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
    Visibility(100);
    paintGL();

    return true;


}




QGroupBox* ex6::controlPanel()
{
    // init group box
    QGroupBox *groupBox = HW::controlPanel();
    groupBox->setStyleSheet(GroupBoxStyle);

    //QPushButton *buttonImport  = new QPushButton("Import model");

    QPushButton *Load = new QPushButton("Load Map");

    connect(Load,SIGNAL(clicked()),this,SLOT(loadMap()));
    //connect(buttonImport, SIGNAL(clicked()), this, SLOT(importModel()));

    //Display
    auto layout = dynamic_cast<QGridLayout*>(groupBox->layout());
    int row = layout->rowCount() + 1;
//    row++;
//    layout->addWidget(buttonImport,row,0);
    row++;
    row++;
    layout->addWidget(Load,row,0);
    row++;

    layout->setRowStretch(row,1);

    return(groupBox);

}


