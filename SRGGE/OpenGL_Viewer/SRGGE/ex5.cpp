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

    // initialize GL function resolution for current context
    initializeGLFunctions();
    initVertexBuffer();

    gShader = new Shader(QString("/Users/Emy/GitHub/FIB/SRGGE/OpenGL_Viewer/shaders/phong.vert"), QString("/Users/Emy/GitHub/FIB/SRGGE/OpenGL_Viewer/shaders/phong.frag"));
    gShader->m_program.bindAttributeLocation("vert", ATTRIB_VERTEX);
    gShader->m_program.bindAttributeLocation("normal" , ATTRIB_NORMAL);


    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    // init state variables
    glClearColor(1.0, 1.0, 1.0, 1.0);	// set background color
    glColor3f   (1.0, 1.0, 0.0);		// set foreground color


}



void ex5::initVertexBuffer(){

    computeQuad(); //Compute vertices, faces & normals of a quad (2 triangles)

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    //Vertex positions
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()* sizeof(GLfloat),&vertices[0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
    glEnableVertexAttribArray(0);

    //Vertex normals
    glGenBuffers(1,&NVBO);
    glBindBuffer(GL_ARRAY_BUFFER,NVBO);
    glBufferData(GL_ARRAY_BUFFER,normals.size()* sizeof(float),&normals[0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,faces.size()* sizeof(int),&faces[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}


//Create a square
void ex5::computeQuad(){

    vertices.clear();
    faces.clear();
    normals.clear();

    //Add vertices
    vertices.push_back(0);
    vertices.push_back(-1);
    vertices.push_back(0);

    vertices.push_back(1);
    vertices.push_back(-1);
    vertices.push_back(0);

    vertices.push_back(0);
    vertices.push_back(-1);
    vertices.push_back(1);

    vertices.push_back(1);
    vertices.push_back(-1);
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



//Check if 0 is interior or exterior
bool ex5::checkIE(std::vector<std::vector<int>> map, int line, int row){

    //Count the nb of 1 on each side
    int count_left=0;
    int count_right=0;

    for(int i=0; i<row;i++){
        if(map[line][i]==1)
            count_left+=1;
    }

    for(int j=row; j<(int) map[0].size(); j++){
        if(map[line][j]==1)
                count_right+=1;
    }

    return (count_left%2==0 && count_right%2==0);

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

                gShader->setMat4("u_projection",projection);
                gShader->setMat4("u_view",view);
                gShader->setMat4("u_model",model);
                gShader->setMat3("u_normal_matrix",normal);


                //DISPLAY

                if(museum[i][j]==0){
                    Eigen::Vector3f c;
                    if(!checkIE(museum,i,j))
                        c[0]=1.0;
                    else
                        c[1]=1.0;
                    gShader->setVec3("color",c);
                    glBindVertexArray(VAO);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
                    glDrawElements(GL_TRIANGLES,faces.size(),GL_UNSIGNED_INT,0);
                    glBindVertexArray(0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
                }


                if(museum[i][j]>0){
                    Eigen::Vector3f c(1.0,0.0,0.0);
                    gShader->setVec3("color",c);
                    glBindVertexArray(VAO);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
                    glDrawElements(GL_TRIANGLES,faces.size(),GL_UNSIGNED_INT,0);
                    glBindVertexArray(0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
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
    paintGL();

    return true;


}

//--------------------------------------------------------------------------------


QGroupBox* ex5::controlPanel()
{
    // init group box
    QGroupBox *groupBox = HW::controlPanel();
    groupBox->setStyleSheet(GroupBoxStyle);


    QPushButton *Load = new QPushButton("Load Map");

    connect(Load,SIGNAL(clicked()),this,SLOT(loadMap()));

    //Display
    auto layout = dynamic_cast<QGridLayout*>(groupBox->layout());
    int row = layout->rowCount() + 1;
    row++;
    layout->addWidget(Load,row,0);
    row++;

    groupBox->setLayout(layout);

    return(groupBox);

}







