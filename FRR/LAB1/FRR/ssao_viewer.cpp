#include "ssao_viewer.h"
#include "Helpers/mesh_importer.h"
#include </usr/local/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/include/glm/glm.hpp>
#include </usr/local/include/glm/gtc/type_ptr.hpp>
#include "time.h"

int final_time, initial_time=time(NULL), frames=0;


ssao_viewer::ssao_viewer(const QGLFormat &glf, QWidget *parent) : Viewer1(glf, parent)
{
    // init vars
    setFocusPolicy(Qt::StrongFocus);


}

void ssao_viewer::initializeGL()
{

    // initialize GL function resolution for current context
    initializeGLFunctions();

    gShader = new Shader(QString("/Users/Emy/GitHub/OpenGL_Viewer/shaders/ssao_geom.vert"), QString("/Users/Emy/GitHub/OpenGL_Viewer/shaders/ssao_geom.frag"));
    gShader->m_program.bindAttributeLocation("vert", ATTRIB_VERTEX);
    gShader->m_program.bindAttributeLocation("normal" , ATTRIB_NORMAL);

    ssaoShader = new Shader(QString("/Users/Emy/GitHub/OpenGL_Viewer/shaders/ssao.vert"), QString("/Users/Emy/GitHub/OpenGL_Viewer/shaders/ssao.frag"));
    ssaoShader->m_program.bindAttributeLocation("vert" , 0);

    blurShader = new Shader(QString("/Users/Emy/GitHub/OpenGL_Viewer/shaders/blur.vert"), QString("/Users/Emy/GitHub/OpenGL_Viewer/shaders/blur.frag"));
    blurShader->m_program.bindAttributeLocation("vert" , 0);
    blurShader->m_program.bindAttributeLocation("texCoords" , 1);


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


void ssao_viewer::paintGL()
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

        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

        ssaoShader->use();

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

        //Send SSAO Kernel
        GLuint kernel_location = glGetUniformLocation(ssaoShader->ID, "samples");
        glUniform3fv(kernel_location, 64, &ssaoKernel[0][0]);

        ssaoShader -> setInt("sampleN",sampleN);


        paintQuad();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindTexture(GL_TEXTURE_2D, ssaoTexture);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);



        glUseProgram(0);



        //----------------------------------------------------------------------------------------------------------------
        //---------------------------------------------  3RD PASS: BLUR   ------------------------------------------------
        //----------------------------------------------------------------------------------------------------------------


        //RENDER TEXTURES
        camera_.SetViewport();
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        blurShader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssaoTexture);
        blurShader->setInt("ssaoInput",0);

        glm::mat4 quad_projection = glm::ortho(-1.0, 1.0, -1.0, 1.0);
        Eigen::Matrix4f quad_view = camera_.SetIdentity().cast<float>();

        blurShader->setglm4("q_projection",quad_projection);
        blurShader->setMat4("q_view",quad_view);

        width = camera_.GetWidth();
        height = camera_.GetHeight();

        blurShader->setFloat("width",float(width));
        blurShader->setFloat("height",float(height));

        float sigma = 20.0;
        blurShader->setFloat("sigma",sigma);

        blurShader->setFloat("blurRadius",blurRadius);

        blurShader->setBool("blur_flag",flag_blur);

        paintQuad();

        glUseProgram(0);


    }


    frames++;
    final_time=time(NULL);
    if(final_time-initial_time>0)
    {
        emit SetFramerate(QString::number(frames/(final_time-initial_time)));
        frames=0;
        initial_time=final_time;
    }



}






void ssao_viewer::initVertexBuffer()
{

    t_Timer.start();

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
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);


    // SSAO texture
    glGenTextures(1, &ssaoTexture);
    glBindTexture(GL_TEXTURE_2D, ssaoTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width_, height_, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoTexture, 0);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    //SAMPLE EMISPHERE
    //Create the emisphere sample selector ( in tangent space)
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;

    for (unsigned int i = 0; i < 64; ++i)
    {
        glm::vec3 sample(
                    randomFloats(generator) * 2.0 - 1.,
                    randomFloats(generator) * 2.0 - 1.0,
                    randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0;
        scale = 0.1f + (scale*scale) * (1.0f - 0.1f);
        sample *= scale;
        ssaoKernel.push_back(sample);
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




QGroupBox* ssao_viewer::controlPanel()
{
    // INIT GROUPBOX
    QGroupBox *groupBox = Viewer1::controlPanel();


    //WIDGETS

    QFrame* line0 = new QFrame();
    line0->setFrameShape(QFrame::HLine);
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    QFrame* line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);

    QLabel *title_fps = new QLabel("Framerate:");
    QLabel *print_fps = new QLabel(QString::number(0));

    QLabel *title_ssao = new QLabel("<b><u>SSAO</u></b>");

    QLabel *title_radius = new QLabel("Radius of Kernel Hemisphere:");

    QDoubleSpinBox *change_radius = new QDoubleSpinBox();
    change_radius -> setSingleStep(0.1);
    change_radius -> setValue(radius);
    change_radius -> setDecimals(1);


    QRadioButton *select_no_blur = new QRadioButton(tr("Simple SSAO"));
    select_no_blur->setChecked(true);
    QRadioButton *select_blur = new QRadioButton(tr("Blurred SSAO"));

    QLabel *title_blur_radius = new QLabel("Blur Radius:");
    QLabel *title_samples = new QLabel("Number of samples:");

    QDoubleSpinBox *change_blurRadius = new QDoubleSpinBox();
    change_blurRadius -> setSingleStep(0.1);
    change_blurRadius -> setMaximum(3.0);
    change_blurRadius -> setMinimum(0.1);
    change_blurRadius -> setValue(blurRadius);
    change_blurRadius -> setDecimals(1);

    QSpinBox *change_sampleN = new QSpinBox();
    change_sampleN -> setSingleStep(2);
    change_sampleN -> setMinimum(10);
    change_sampleN -> setMaximum(64);
    change_sampleN -> setValue(sampleN);



    //LAYOUT

    auto layout = dynamic_cast<QGridLayout*>(groupBox->layout());
    int row = layout->rowCount() + 1;


    QRadioButton *select1 = new QRadioButton("3x3");
    QRadioButton *select2 = new QRadioButton("4x4");
    QRadioButton *select3 = new QRadioButton("6x6");
    QRadioButton *select4 = new QRadioButton("8x8");
    select4->setChecked(true);

    //SIGNALS

    connect(this,SIGNAL(SetFramerate(QString)),print_fps,SLOT(setText(QString)));
    connect(change_radius, SIGNAL(valueChanged(double)),this,SLOT(setRadius(double)));
    connect(select_no_blur,SIGNAL(toggled(bool)),this,SLOT(setBlurOff()));
    connect(select_blur,SIGNAL(toggled(bool)),this,SLOT(setBlurOn()));
    connect(change_blurRadius, SIGNAL(valueChanged(double)),this,SLOT(setBlurRadius(double)));
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
    layout->addWidget(select_no_blur,row,0);
    layout->addWidget(select_blur,row,1);
    row++;
    layout->addWidget(title_blur_radius,row,0);
    layout->addWidget(change_blurRadius,row,1);
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



void ssao_viewer::setRadius(double R)
{
    radius=R;
    paintGL();
}


void ssao_viewer::setBlurRadius(double R)
{
    blurRadius=R;
    paintGL();
}



void ssao_viewer::setBlurOn()
{
    flag_blur=true;
    paintGL();

}


void ssao_viewer::setBlurOff()
{
    flag_blur=false;
    paintGL();

}



void ssao_viewer::setFirstSample()
{
    sampleN=9;
    paintGL();
}

void ssao_viewer::setSecondSample()
{
    sampleN=16;
    paintGL();
}

void ssao_viewer::setThirdSample()
{
    sampleN=36;
    paintGL();
}

void ssao_viewer::setFourthSample()
{
    sampleN=64;
    paintGL();
}



