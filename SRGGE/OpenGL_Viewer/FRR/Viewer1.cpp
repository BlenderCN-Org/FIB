#include "Viewer1.h"
#include "Helpers/mesh_importer.h"
#include </usr/local/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/include/glm/glm.hpp>


Viewer1::Viewer1(const QGLFormat &glf, QWidget *parent) : HW(glf, parent) , mesh_importer(this)
{
    // init vars
    setFocusPolicy(Qt::StrongFocus);


}

void Viewer1::initializeGL()
{

    // initialize GL function resolution for current context
    initializeGLFunctions();

    gShader = new Shader(QString("/Users/Emy/GitHub/FIB/SRGGE/OpenGL_Viewer/shaders/ssao_geom.vert"), QString("/Users/Emy/GitHub/FIB/SRGGE/OpenGL_Viewer/shaders/ssao_geom.frag"));
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


void Viewer1::paintGL()
{

    // Render to our framebuffer
    glViewport(0,0,width_,height_); // Render on the whole framebuffer
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mesh_ != nullptr) {

        // RENDER GEOMETRY

        camera_.SetViewport();

        gShader->use();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Eigen::Matrix4f projection = camera_.SetProjection();
        Eigen::Matrix4f view = camera_.SetView();
        Eigen::Matrix4f model = camera_.SetModel();

        Eigen::Matrix4f t = view * model;
        Eigen::Matrix3f normal;
        for (int i = 0; i < 3; ++i)
          for (int j = 0; j < 3; ++j) normal(i, j) = t(i, j);

        normal = normal.inverse().transpose();

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


        glUseProgram(0);



    }

}



void Viewer1::initVertexBuffer()
{

    glBindVertexArray(vao);
    if (mesh_ == nullptr) return;


    // VBO ------------------------------------------------------

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

    updateGL();
}



void Viewer1::paintQuad()
{
    //inizialization
    if (quadVBO == 0){

        std::cout << "defined the quad" << std::endl;
        GLfloat quadVertices[] =
        {
             // Positions
            -1.0f,  1.0f, -0.05f,
            -1.0f, -1.0f, -0.05f,
             1.0f,  1.0f, -0.05f,
             1.0f, -1.0f, -0.05f,
        };
        GLfloat quadTextureCoord[] =
        {
            // Texture Coords
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
        };


        //position
        glGenBuffers(1, &quadVBO);
        //Generate, bind and fill VBO for vertices
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

        glGenBuffers(1, &quadTextCoordVBO);
        //Generate, bind and fill VBO for texCoords
        glBindBuffer(GL_ARRAY_BUFFER, quadTextCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadTextureCoord), &quadTextureCoord, GL_STATIC_DRAW);

    }

    enum {
        VERTICES,
        TEXTURE_COORDS
    };



    //render time

    glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
    glEnableVertexAttribArray(VERTICES);
    glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER,quadTextCoordVBO);
    glEnableVertexAttribArray(TEXTURE_COORDS);
    glVertexAttribPointer(TEXTURE_COORDS, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //unbind
    glBindBuffer(GL_ARRAY_BUFFER,0);

}



void Viewer1::resizeGL(int w, int h)
{

    if (h == 0) h = 1;
    width_ = w;
    height_ = h;

    camera_.SetViewport(0, 0, w, h);
    camera_.SetProjection(kFieldOfView, kZNear, kZFar);

    m_winW = w;
    m_winH = h;

    return;
}

void Viewer1::reset()
{
    mesh_ = nullptr;	// recompute geometry
    initVertexBuffer();
    // draw
    updateGL();
}





QGroupBox* Viewer1::controlPanel()
{
    // init group box
    QGroupBox *groupBox = HW::controlPanel();
    groupBox->setStyleSheet(GroupBoxStyle);
    mesh_importer.AddImportExportPLY(groupBox);

    return(groupBox);
}



void Viewer1::importModel()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this, tr("Load model"), "./",
                                              tr("PLY Files ( *.ply )"));
    if (!filename.isNull()) {
         if(!mesh_importer.LoadModel(filename))
              QMessageBox::warning(this, tr("Error"),
                              tr("The file could not be opened"));
      }

}

void Viewer1::exportModel(){
    QString filename;

    filename = QFileDialog::getSaveFileName(this, "Save file",tr("PLY Files (*.ply)"));

    if(filename.isNull())
        filename = "untitled";
}


void Viewer1::setNumberCopies(int N)
{
    copies=N;
    paintGL();
}


