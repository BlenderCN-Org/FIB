#include "separable.h"
#include "Helpers/mesh_importer.h"
#include </usr/local/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/include/glm/glm.hpp>
#include </usr/local/include/glm/gtc/type_ptr.hpp>
#include "time.h"

int final_time2, initial_time2=time(NULL), frames2=0;


separable::separable(const QGLFormat &glf, QWidget *parent) : Viewer1(glf, parent)
{
    // init vars
    setFocusPolicy(Qt::StrongFocus);


}

void separable::initializeGL()
{

    // initialize GL function resolution for current context
    initializeGLFunctions();

    gShader = new Shader(QString("/Users/Emy/GitHub/OpenGL_Viewer/shaders/ssao_geom.vert"), QString("/Users/Emy/GitHub/OpenGL_Viewer/shaders/ssao_geom.frag"));
    gShader->m_program.bindAttributeLocation("vert", ATTRIB_VERTEX);
    gShader->m_program.bindAttributeLocation("normal" , ATTRIB_NORMAL);

    ssaoShader = new Shader(QString("/Users/Emy/GitHub/OpenGL_Viewer/shaders/separable.vert"), QString("/Users/Emy/GitHub/OpenGL_Viewer/shaders/separable.frag"));
    ssaoShader->m_program.bindAttributeLocation("vert" , 0);
    ssaoShader->m_program.bindAttributeLocation("texCoords" , 1);

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


void separable::paintGL()
{

    // Render to our framebuffer
    glViewport(0,0,width_,height_); // Render on the whole framebuffer
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mesh_ != nullptr) {

        //----------------------------------------------------------------------------------------------------------------
        //-----------------------------------------   1ST PASS: GEOMETRY    ----------------------------------------------
        //----------------------------------------------------------------------------------------------------------------

        camera_.SetViewport();

        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

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

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindTexture(GL_TEXTURE_2D, gNormal);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, gPosition);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);



        glUseProgram(0);


        //----------------------------------------------------------------------------------------------------------------
        //---------------------------------------------   2ND PASS: SSAO    ----------------------------------------------
        //----------------------------------------------------------------------------------------------------------------


        camera_.SetViewport();
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ssaoShader->use();


        glm::mat4 quad_projection = glm::ortho(-1.0, 1.0, -1.0, 1.0);
        Eigen::Matrix4f quad_view = camera_.SetIdentity().cast<float>();

        ssaoShader->setglm4("q_projection",quad_projection);
        ssaoShader->setMat4("q_view",quad_view);
        ssaoShader->setMat4("projection",projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        ssaoShader->setInt("gNormal",0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        ssaoShader->setInt("gPosition",1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        ssaoShader->setInt("texNoise",2);

        double width = camera_.GetWidth();
        double height = camera_.GetHeight();

        ssaoShader->setFloat("width",width);
        ssaoShader->setFloat("height",height);

        ssaoShader->setFloat("radius",radius);
        //std::cout << radius << std::endl;

        //Send X Kernel
        GLuint kernel_location_x = glGetUniformLocation(ssaoShader->ID, "samples_x");
        glUniform3fv(kernel_location_x, sampleN, &ssaoKernel_x[0][0]);

        //Send Y Kernel
        GLuint kernel_location_y = glGetUniformLocation(ssaoShader->ID, "samples_y");
        glUniform3fv(kernel_location_y, 64, &ssaoKernel_y[0][0]);

        ssaoShader -> setInt("sampleN",sampleN);

        paintQuad();


        glUseProgram(0);


    }

    frames2++;
    final_time2=time(NULL);
    if(final_time2-initial_time2>0)
    {
        emit SetFramerate(QString::number(frames2/(final_time2-initial_time2)));
        frames2=0;
        initial_time2=final_time2;
    }



}






void separable::initVertexBuffer()
{

    t_Timer.start();

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


    // GBuffer ----------------------------------------------------


    //Generate & Bind gBuffers
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);


    //gNormal
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width_, height_, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gNormal, 0);


    //Position
    glGenTextures(1,&gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition); // It gives better results to use GL_LINEAR instead of GL_NEAREST.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width_, height_, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gPosition, 0);



    GLuint attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);


    //DEPTH BUFFER
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);



    //CHECK STATUS
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


    //Unbind gBuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // ssao ----------------------------------------------------
    camera_.SetViewport();
    glGenFramebuffers(1, &ssaoFBO);
    //glGenFramebuffers(1, &ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);



    // SSAO color buffer
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width_, height_, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    //SAMPLE EMISPHERE
    //Create the emisphere sample selector ( in tangent space)
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;

    for (int i = 0; i < 64; ++i)
    {
        glm::vec3 sample_x(
                    randomFloats(generator) * 2.0 - 1.,
                    0.0,
                    randomFloats(generator));

        glm::vec3 sample_y;
        sample_y=glm::cross(sample_x, glm::vec3(0.0,0.0,1.0));
        sample_y+=glm::vec3(0.0,0.0,randomFloats(generator));

        sample_x = glm::normalize(sample_x);
        sample_y = glm::normalize(sample_y);

        sample_x *= randomFloats(generator);
        sample_y *= randomFloats(generator);

        float scale = float(i) / float(64);
        scale = 0.1f + (scale*scale) * (1.0f - 0.1f);

        sample_x *= scale;
        sample_y *= scale;

        ssaoKernel_x.push_back(sample_x);
        ssaoKernel_y.push_back(sample_y);

    }


    //Adding randomness(noise) in rotation to the sample selection
    //in order to do so, we create a small texture of random rotations
    std::vector<Eigen::Vector3f> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++)
    {
        Eigen::Vector3f noise(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0.0f);
        ssaoNoise.push_back(noise);
    }

    camera_.SetViewport();

    //NOISE TEXTURE:

    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



    updateGL();
}




QGroupBox* separable::controlPanel()
{
    // init group box
    QGroupBox *groupBox = Viewer1::controlPanel();

    //WIDGETS

    QFrame* line0 = new QFrame();
    line0->setFrameShape(QFrame::HLine);
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    QFrame* line2 = new QFrame();
    line->setFrameShape(QFrame::HLine);

    QLabel *title_fps = new QLabel("Framerate:");
    QLabel *print_fps = new QLabel(QString::number(0));

    QDoubleSpinBox *change_radius = new QDoubleSpinBox();
    change_radius -> setSingleStep(0.1);
    change_radius -> setMinimum(0.1);
    change_radius -> setValue(radius);
    change_radius -> setDecimals(1);

    QLabel *title_ssao = new QLabel("<b><u>SSAO - Separable Version</u></b>");
    QLabel *title_radius = new QLabel("Max distance of samples:");
    QLabel *title_samples = new QLabel("Number of samples:");

    auto layout = dynamic_cast<QGridLayout*>(groupBox->layout());
    int row = layout->rowCount() + 1;

    QRadioButton *select1 = new QRadioButton("3+3");
    QRadioButton *select2 = new QRadioButton("4+4");
    QRadioButton *select3 = new QRadioButton("6+6");
    QRadioButton *select4 = new QRadioButton("8+8");
    select4->setChecked(true);


    //SIGNALS

    connect(this,SIGNAL(SetFramerate(QString)),print_fps,SLOT(setText(QString)));
    connect(change_radius, SIGNAL(valueChanged(double)),this,SLOT(setRadius(double)));
    connect(select1,SIGNAL(toggled(bool)),this,SLOT(setFirstSample()));
    connect(select2,SIGNAL(toggled(bool)),this,SLOT(setSecondSample()));
    connect(select3,SIGNAL(toggled(bool)),this,SLOT(setThirdSample()));
    connect(select4,SIGNAL(toggled(bool)),this,SLOT(setFourthSample()));


    //DISPLAY

    row++;
    layout->addWidget(line0,row,0, 1 ,layout->columnCount());
    row++;

    layout->addWidget(title_fps,row,0);
    layout->addWidget(print_fps,row,1);

    row++;
    layout->addWidget(line,row,0, 1 ,layout->columnCount());
    row++;
    layout->addWidget(title_ssao,row,0);
    row++;
    layout -> addWidget(title_radius,row,0);
    layout->addWidget(change_radius,row,1);
    row++;
    layout->addWidget(line2,row,0, 1 ,layout->columnCount());
    row++;
    layout->addWidget(title_samples,row,0);
    row++;
    layout->addWidget(select1,row,0);
    layout->addWidget(select2,row,1);
    row++;
    layout->addWidget(select3,row,0);
    layout->addWidget(select4,row,1);


    return(groupBox);
}



void separable::setRadius(double R)
{
    radius=R;
    paintGL();
}

void separable::setFirstSample()
{
    sampleN=3;
    paintGL();
}

void separable::setSecondSample()
{
    sampleN=4;
    paintGL();
}

void separable::setThirdSample()
{
    sampleN=6;
    paintGL();
}

void separable::setFourthSample()
{
    sampleN=8;
    paintGL();
}



