#include "ex2.h"
#include "Helpers/mesh_importer.h"
#include "Helpers/mesh_io.h"
#include </usr/local/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/include/glm/glm.hpp>
#include </usr/local/include/glm/gtc/type_ptr.hpp>
#include "time.h"
#include <cmath>

int final_time4, initial_time4=time(NULL), frames4=0;



namespace data_representation {

namespace {

void ComputeVertexNormals(const std::vector<float> &vertices,
                          const std::vector<int> &faces,
                          std::vector<float> *normals) {
  const int kFaces = faces.size();
  std::vector<float> face_normals(kFaces, 0);

  for (int i = 0; i < kFaces; i += 3) {
    Eigen::Vector3d v1(vertices[faces[i] * 3], vertices[faces[i] * 3 + 1],
                       vertices[faces[i] * 3 + 2]);
    Eigen::Vector3d v2(vertices[faces[i + 1] * 3],
                       vertices[faces[i + 1] * 3 + 1],
                       vertices[faces[i + 1] * 3 + 2]);
    Eigen::Vector3d v3(vertices[faces[i + 2] * 3],
                       vertices[faces[i + 2] * 3 + 1],
                       vertices[faces[i + 2] * 3 + 2]);
    Eigen::Vector3d v1v2 = v2 - v1;
    Eigen::Vector3d v1v3 = v3 - v1;
    Eigen::Vector3d normal = v1v2.cross(v1v3);

    if (normal.norm() < 0.0000000001) {
      normal = Eigen::Vector3d(0.0, 0.0, 0.0);
    } else {
      normal.normalize();
    }

    for (int j = 0; j < 3; ++j) face_normals[i + j] = normal[j];
  }

  const int kVertices = vertices.size();
  normals->resize(kVertices, 0);
  for (int i = 0; i < kFaces; i += 3) {
    for (int j = 0; j < 3; ++j) {
      int idx = faces[i + j];
      Eigen::Vector3d v1(vertices[faces[i + j] * 3],
                         vertices[faces[i + j] * 3 + 1],
                         vertices[faces[i + j] * 3 + 2]);
      Eigen::Vector3d v2(vertices[faces[i + (j + 1) % 3] * 3],
                         vertices[faces[i + (j + 1) % 3] * 3 + 1],
                         vertices[faces[i + (j + 1) % 3] * 3 + 2]);
      Eigen::Vector3d v3(vertices[faces[i + (j + 2) % 3] * 3],
                         vertices[faces[i + (j + 2) % 3] * 3 + 1],
                         vertices[faces[i + (j + 2) % 3] * 3 + 2]);

      Eigen::Vector3d v1v2 = v2 - v1;
      Eigen::Vector3d v1v3 = v3 - v1;
      double angle = acos(v1v2.dot(v1v3) / (v1v2.norm() * v1v3.norm()));

      if (angle == angle) {
        for (int k = 0; k < 3; ++k) {
          (*normals)[idx * 3 + k] += face_normals[i + k] * angle;
        }
      }
    }
  }

  const int kNormals = normals->size();
  for (int i = 0; i < kNormals; i += 3) {
    Eigen::Vector3d normal((*normals)[i], (*normals)[i + 1], (*normals)[i + 2]);
    if (normal.norm() > 0.00001) {
      normal.normalize();
    } else {
      normal = Eigen::Vector3d(0, 0, 0);
    }

    for (int j = 0; j < 3; ++j) (*normals)[i + j] = normal[j];
  }
}

  }  // namespace

}  // namespace data_representation




ex2::ex2(const QGLFormat &glf, QWidget *parent) : ex1(glf, parent)
{

    // init vars
    setFocusPolicy(Qt::StrongFocus);


}


void ex2::initializeGL()
{

    // initialize GL function resolution for current context
    initializeGLFunctions();

    gShader = new Shader(QString("/Users/Emy/Documents/Cours/SRGGE/LAB/SRGGE/shaders/try.vert"), QString("/Users/Emy/Documents/Cours/SRGGE/LAB/SRGGE/shaders/try.frag"));
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



void ex2::paintGL()
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
                glBindVertexArray(vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex);

                if(LODsimpleON)
                {
                    glDrawElements(GL_TRIANGLES,new_faces.size(),GL_UNSIGNED_INT,0);
                }
                else
                {
                    glDrawElements(GL_TRIANGLES,mesh_->faces_.size(),GL_UNSIGNED_INT,0);
                }


                glBindVertexArray(0);


                //Reset ModelView
                model=camera_.SetModel();

                }

            }


        glUseProgram(0);


    }


    //Set Framerate

    frames4++;
    final_time4=time(NULL);
    if(final_time4-initial_time4>0)
    {
        emit SetFramerate(QString::number(frames4/(final_time4-initial_time4)));
        frames4=0;
        initial_time4=final_time4;
    }



}


//Given a vertices number, we return the id of the cell it is contained in
int ex2::cellid(int v,int l){

    int i = static_cast<int>(((mesh_->vertices_[v*3])-(mesh_->min_[0]))/(dimx/l));
    int j = static_cast<int>(((mesh_->vertices_[v*3+1])-(mesh_->min_[1]))/(dimy/l));
    int k = static_cast<int>(((mesh_->vertices_[v*3+2])-(mesh_->min_[2]))/(dimz/l));

    if(i==l)
        i-=1;
    if(j==l)
        j-=1;
    if(k==l)
        k-=1;

    return (i+l*(j+k*l));

}



void ex2::computeGrid(){


    new_vertices.clear();
    new_faces.clear();
    new_normals.clear();


    dimx=((mesh_->max_[0])-(mesh_->min_[0]));
    dimy=((mesh_->max_[1])-(mesh_->min_[1]));
    dimz=((mesh_->max_[2])-(mesh_->min_[2]));


    //Number of cells depending on level of details
    nb_cells=level*level*level;


    //Put each vertex into the cell it is contained in
    std::vector<std::vector<int>> grid(nb_cells);

    for (int i=0; i<((int) mesh_->vertices_.size()/3);i++){
        grid[cellid(i,level)].push_back(i);

    }

    //Store the new id of the vertices
    std::vector<float> id_vertices((int) (mesh_->vertices_.size())/3);

    int iter=0;

    //Compute the vertex mean in each cell
    for(int i=0; i<nb_cells;i++){

        float sumx=0.;
        float sumy=0.;
        float sumz=0.;
        int nb_vertices = grid[i].size();


        if(nb_vertices!=0){

            // Sum all the coordinates of all the vertices in the cell
            for (int j=0; j<nb_vertices;j++)
            {
                sumx+=mesh_->vertices_[grid[i][j]*3];
                sumy+=mesh_->vertices_[grid[i][j]*3+1];
                sumz+=mesh_->vertices_[grid[i][j]*3+2];
                id_vertices[grid[i][j]]=iter;

            }

            //Divide by number of vertices
            sumx=sumx/nb_vertices;
            sumy=sumy/nb_vertices;
            sumz=sumz/nb_vertices;

            //Add the new vertex to the list of new vertices
            new_vertices.push_back(sumx);
            new_vertices.push_back(sumy);
            new_vertices.push_back(sumz);

            iter+=1;


         }


    }



    //Compute the faces - loop on all faces
    for (int i=0; i<((int) (mesh_->faces_.size())/3);i++){

        //We get the vertices of the previous triangles
        int a = mesh_->faces_[3*i];
        int b = mesh_->faces_[3*i+1];
        int c = mesh_->faces_[3*i+2];

        //ID of the cell to which the vertices belong
        int ida = id_vertices[a];
        int idb = id_vertices[b];
        int idc = id_vertices[c];


        //Check if the vertices belong to the same cell before adding the new face
        if (!(ida == idb || ida==idc || idb==idc))
        {

            //Replace vertex indices by the index of the cell vertex
            new_faces.push_back(ida);
            new_faces.push_back(idb);
            new_faces.push_back(idc);


        }


    }

     data_representation::ComputeVertexNormals(new_vertices,new_faces,&new_normals);

}



void ex2::computeQ()
{

    QMatrices.clear();

   for(int id=0; id < (int) mesh_->vertices_.size()/3; id++){

        // Vertex v
       Eigen::Vector3d v(mesh_->vertices_[id*3],mesh_->vertices_[id*3+1],mesh_->vertices_[id*3+2]);

       Eigen::Vector3d normal(mesh_->normals_[id*3],mesh_->normals_[id*3+1],mesh_->normals_[id*3+2]);
       Eigen::Vector4d pv(normal[0],normal[1],normal[2],-normal.dot(v));

       Eigen::Matrix4d Qv = pv * pv.transpose();

       QMatrices.push_back(Qv);

   }
}





void ex2::computeQuadric(){

    new_vertices.clear();
    new_faces.clear();
    new_normals.clear();


    dimx=((mesh_->max_[0])-(mesh_->min_[0]));
    dimy=((mesh_->max_[1])-(mesh_->min_[1]));
    dimz=((mesh_->max_[2])-(mesh_->min_[2]));


    //Number of cells depending on level of details
    nb_cells=level*level*level;


    //Put each vertex into the cell it is contained in
    std::vector<std::vector<int>> grid(nb_cells);

    for (int i=0; i<((int) mesh_->vertices_.size()/3);i++){
        grid[cellid(i,level)].push_back(i);

    }

    //Store the new id of the vertices
    std::vector<float> id_vertices((int) (mesh_->vertices_.size())/3);

    int iter=0;

    //Compute the Q matrix in each cell
    for(int i=0; i<nb_cells;i++){

        int nb_vertices = grid[i].size();
        Eigen::Matrix4d Q = Eigen::Matrix4d::Zero();

        if(nb_vertices!=0){

            // Sum all the coordinates of all the vertices in the cell
            for (int j=0; j<nb_vertices;j++)
            {
                Q+=QMatrices[grid[i][j]];
                id_vertices[grid[i][j]]=iter;

            }


            //Compute the new vertex from the matrix
            Q.row(3) << 0, 0, 0, 1;

            Eigen::Matrix4d inverse;
            bool invertible;
            Q.computeInverseWithCheck(inverse,invertible,0.1);

            Eigen::Vector4d new_V;

            if(invertible){
                new_V = inverse * Eigen::Vector4d(0,0,0,1);

                //Add the new vertex to the list of new vertices
                new_vertices.push_back(new_V[0]);
                new_vertices.push_back(new_V[1]);
                new_vertices.push_back(new_V[2]);

            }

            else{

                new_vertices.push_back(mesh_->vertices_[grid[i][0]*3]);
                new_vertices.push_back(mesh_->vertices_[grid[i][0]*3+1]);
                new_vertices.push_back(mesh_->vertices_[grid[i][0]*3+2]);

            }


            iter+=1;


         }


    }


    //Compute the faces - loop on all faces
    for (int i=0; i<((int) (mesh_->faces_.size())/3);i++){

        //We get the vertices of the previous triangles
        int a = mesh_->faces_[3*i];
        int b = mesh_->faces_[3*i+1];
        int c = mesh_->faces_[3*i+2];

        //ID of the cell to which the vertices belong
        int ida = id_vertices[a];
        int idb = id_vertices[b];
        int idc = id_vertices[c];


        //Check if the vertices belong to the same cell before adding the new face
        if (!(ida == idb))
        {

            //Replace vertex indices by the index of the cell vertex
            new_faces.push_back(ida);
            new_faces.push_back(idb);
            new_faces.push_back(idc);


        }


    }

     data_representation::ComputeVertexNormals(new_vertices,new_faces,&new_normals);


}




void ex2::initVertexBuffer()
{


    if (mesh_ == nullptr) return;


    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    static bool flag=1;

    if(flag){
        glGenBuffers(1,&vboVertex);
        glGenBuffers(1,&vboNormal);
        glGenBuffers(1,&vboIndex);

        flag=0;
    }


    if(LODsimpleON||QuadricON){

        if(LODsimpleON)
            computeGrid();
        else{
            computeQuadric();
        }

        //computeGrid();

        //Vertex positions
        glBindBuffer(GL_ARRAY_BUFFER,vboVertex);
        glBufferData(GL_ARRAY_BUFFER,new_vertices.size()* sizeof(GLfloat),&new_vertices[0],GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
        glEnableVertexAttribArray(0);

        //Vertex normals
        glBindBuffer(GL_ARRAY_BUFFER,vboNormal);
        glBufferData(GL_ARRAY_BUFFER,new_normals.size() * sizeof(float),&new_normals[0],GL_STATIC_DRAW);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,new_faces.size()* sizeof(int),&new_faces[0],GL_STATIC_DRAW);


        emit SetFaces(QString::number(new_faces.size() / 3));
        emit SetVertices(QString::number(new_vertices.size() / 3));


    }

    else
    {
        //Vertex positions
        glBindBuffer(GL_ARRAY_BUFFER,vboVertex);
        glBufferData(GL_ARRAY_BUFFER,mesh_->vertices_.size()* sizeof(GLfloat),&mesh_->vertices_[0],GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0 ,0);
        glEnableVertexAttribArray(0);

        //Vertex normals
        glBindBuffer(GL_ARRAY_BUFFER,vboNormal);
        glBufferData(GL_ARRAY_BUFFER,mesh_->normals_.size() * sizeof(float),&mesh_->normals_[0],GL_STATIC_DRAW);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh_->faces_.size()* sizeof(int),&mesh_->faces_[0],GL_STATIC_DRAW);


    }




    glBindVertexArray(0);




}







QGroupBox* ex2::controlPanel()
{
    // init group box
    QGroupBox *groupBox = ex1::controlPanel();
    groupBox->setStyleSheet(GroupBoxStyle);

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);

    //Unable LOD
    QButtonGroup *buttonGroup = new QButtonGroup;
    QRadioButton *LODoff = new QRadioButton("Normal");
    QRadioButton *OnBasic = new QRadioButton("LOD Basic");
    QRadioButton *OnAdvanced = new QRadioButton("LOD Octree");
    QRadioButton *OnQuadric = new QRadioButton("LOD Quadric");
    LODoff -> setChecked(true);
    buttonGroup -> addButton(LODoff);
    buttonGroup -> addButton(OnBasic);
    buttonGroup -> addButton(OnAdvanced);
    buttonGroup -> addButton(OnQuadric);


    //Selection level
    QLabel *title_lod = new QLabel("<b>LOD</b>");
    QLabel *select_level = new QLabel("Select level:");
    QSlider *num_div = new QSlider(Qt::Horizontal);
    num_div -> setMinimum(2);
    num_div -> setMaximum(100);
    num_div -> setValue(2);
    num_div -> setTickInterval(5);
    num_div -> setTickPosition(QSlider::TicksBelow);

    //LOD counters
    QLabel *title_v = new QLabel("LOD Vertices:    ");
    QLabel *title_f = new QLabel("LOD Faces:    ");
    QLabel *v_count = new QLabel("0");
    QLabel *f_count = new QLabel("0");


    auto layout = dynamic_cast<QGridLayout*>(groupBox->layout());
    int row = layout->rowCount() + 1;

    connect(num_div, SIGNAL(valueChanged(int)),this,SLOT(setNumberlod(int)));
    connect(LODoff,SIGNAL(toggled(bool)),this,SLOT(setOFF()));
    connect(OnBasic,SIGNAL(toggled(bool)),this,SLOT(setOnBasic()));
    connect(OnQuadric,SIGNAL(toggled(bool)),this,SLOT(setOnQuadric()));
    connect(this,SIGNAL(SetVertices(QString)),v_count,SLOT(setText(QString)));
    connect(this,SIGNAL(SetFaces(QString)),f_count,SLOT(setText(QString)));


    //Layout

    row++;
    layout->addWidget(LODoff,row,0,1,2);
    layout->addWidget(OnBasic,row,1);
    row++;
    layout->addWidget(OnAdvanced,row,0);
    layout->addWidget(OnQuadric,row,1);
    row++;

    row++;
    layout->addWidget(line,row,0, 1 ,layout->columnCount());
    row++;

    layout->addWidget(title_lod,row,0);
    row++;
    layout->addWidget(select_level,row,0);
    row++;
    layout->addWidget(num_div,row,0,1,2);

    row++;
    layout->addWidget(title_v,row,0);
    layout->addWidget(v_count,row,1);
    row++;
    layout->addWidget(title_f,row,0);
    layout->addWidget(f_count,row,1);
    row++;


    return(groupBox);

}



void ex2::setNumberlod(int n){
    if(LODsimpleON||QuadricON){
        level=n;
        initializeGL();
        initVertexBuffer();
        paintGL();
    }
}

void ex2::setOFF(){
    LODsimpleON=false;
    QuadricON=false;
    initVertexBuffer();
    paintGL();
}

void ex2::setOnBasic(){
    LODsimpleON=true;
    QuadricON=false;
    initVertexBuffer();
    paintGL();
}

void ex2::setOnQuadric(){
    QuadricON=true;
    LODsimpleON=false;
    computeQ();
    initVertexBuffer();
    paintGL();
}
