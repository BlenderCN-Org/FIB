#include "ex1.h"
#include "Helpers/mesh_importer.h"
#include </usr/local/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/include/glm/glm.hpp>
#include </usr/local/include/glm/gtc/type_ptr.hpp>
#include "time.h"


ex1::ex1(const QGLFormat &glf, QWidget *parent) : Viewer1(glf, parent)
{
    // init vars
    setFocusPolicy(Qt::StrongFocus);


}

void ex1::initializeGL()
{

    // initialize GL function resolution for current context
    initializeGLFunctions();

    gShader = new Shader(QString("/Users/Emy/GitHub/FIB/SRGGE/OpenGL_Viewer/shaders/try.vert"), QString("/Users/Emy/GitHub/FIB/SRGGE/OpenGL_Viewer/shaders/try.frag"));
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


void ex1::paintGL()
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

                //Display

                glBindBuffer(GL_ARRAY_BUFFER,vboVertex);
                glEnableVertexAttribArray(ATTRIB_VERTEX);
                glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, NULL);

                glBindBuffer(GL_ARRAY_BUFFER,vboNormal);
                glEnableVertexAttribArray(ATTRIB_NORMAL);
                glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboIndex);

                glDrawElements(GL_TRIANGLES,mesh_->faces_.size(),GL_UNSIGNED_INT,0);


                glBindBuffer(GL_ARRAY_BUFFER,0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


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






void ex1::initVertexBuffer()
{


    glBindVertexArray(vao);
    if (mesh_ == nullptr) return;


    //VBO ------------------------------------------------------

    static bool flag = 1;

    if(flag) {      // create vertex and color buffers
        glGenBuffers(1, &vboVertex);
        glGenBuffers(1, &vboNormal);
        glGenBuffers(1, &vboIndex);

        flag = 0;       // reset flag
    }

    //Generate, bind and fill VBO for vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
    glBufferData(GL_ARRAY_BUFFER, mesh_->vertices_.size() * sizeof(float), &mesh_->vertices_[0], GL_STATIC_DRAW);

    //Generate, bind and fill VBO for normals
    glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
    glBufferData(GL_ARRAY_BUFFER, mesh_->normals_.size() * sizeof(float), &mesh_->normals_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Generate, bind and fill VBO for indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_->faces_.size() * sizeof(int), &mesh_->faces_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // -----------------------------------------------------------


}





QGroupBox* ex1::controlPanel()
{
    // init group box
    QGroupBox *groupBox = Viewer1::controlPanel();
    groupBox->setStyleSheet(GroupBoxStyle);

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    QFrame* line1 = new QFrame();
    line1->setFrameShape(QFrame::HLine);
    QFrame* line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);

    QLabel *title_fps = new QLabel("Framerate:");
    print_fps = new QLabel(QString::number(0));

    //Selection number of copies
    QLabel *title_copies = new QLabel("Number of copies:");
    QSpinBox *num_copies = new QSpinBox();
    num_copies -> setMinimum(1);

    auto layout = dynamic_cast<QGridLayout*>(groupBox->layout());
    int row = layout->rowCount() + 1;

    connect(num_copies, SIGNAL(valueChanged(int)),this,SLOT(setNumberCopies(int)));
    connect(this,SIGNAL(SetFramerate(QString)),print_fps,SLOT(setText(QString)));

    row++;
    layout->addWidget(line,row,0, 1 ,layout->columnCount());
    row++;

    layout->addWidget(title_fps,row,0);
    layout->addWidget(print_fps,row,1);

    row++;
    layout->addWidget(line1,row,0, 1 ,layout->columnCount());
    row++;

    layout->addWidget(title_copies,row,0);
    layout->addWidget(num_copies,row,1);

    row++;
    layout->addWidget(line2,row,0, 1 ,layout->columnCount());
    row++;

    layout->setRowStretch(row,1);


    return(groupBox);

}




void ex1::setNumberCopies(int N)
{
    copies=N;
    paintGL();
}



