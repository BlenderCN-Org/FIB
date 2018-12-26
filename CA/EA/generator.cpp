#include "generator.h"
#include "cal3dExt/modelloader.h"

#include <random>
#define rand01() ((float)std::rand()/RAND_MAX)

Generator::Generator(){
}


void loadModels(std::vector<ModelData*>& models)
{
    ModelData* data;

    // load 'skeleton' model
    std::cout << "Loading 'skeleton' model ..." << std::endl;
    data = ModelLoader::getModel("skeleton", "/Users/Emy/GitHub/FIB/CA/EA/data/skeleton/", "/Users/Emy/GitHub/FIB/CA/EA/data/skeleton.cfg");
    if (!data) {
        std::cerr << "Model initialization failed! (skeleton)" << std::endl;
        return;
    }
    models.push_back(data);
    std::cout << std::endl;

}

Generator::Generator(int nb_particles) :
    nb_particles(nb_particles)
{
    particles.clear();
    this->initBuffers();

    // preload models
    std::vector<ModelData*> modelsData;
    loadModels(modelsData);
    int NUM_MODELS = modelsData.size();

    Particle p = Particle(glm::vec3(0,0,0),glm::vec3(5*rand01()-0.5f,0.0f,5*rand01()-0.5f),0.95,false,rand01()*life,glm::vec3(0));
    particles.push_back(p);
    Model* m = new Model();
    glm::vec3 blend = glm::normalize(glm::vec3(rand01(), rand01(), rand01()));
    m->setCoreModel(modelsData[0]->coreModel);
    m->setAnimationIds(modelsData[0]->animationIds);
    m->setModelScale(modelsData[0]->renderScale);
    m->onInit();
    m->setMotionBlend(&blend[0], 0);
    m->onUpdate(10*rand01());
    m_models.push_back(m);

    plane_down = Plane(glm::vec3(0,0,0),glm::vec3(0,1,0));
    plane_up= Plane(glm::vec3(0,5,0), glm::vec3(0,-1,0));
    plane_right = Plane(glm::vec3(5,0,0),glm::vec3(-1,0,0));
    plane_left = Plane(glm::vec3(-5,0,0),glm::vec3(1,0,0));
    plane_front = Plane(glm::vec3(0,0,5),glm::vec3(0,0,-1));
    plane_bottom = Plane(glm::vec3(0,0,-5),glm::vec3(0,0,1));

}



void Generator::initBuffers(){


    GLfloat particle_quad[20] = {
        //x, y, z (OpenGL coordinates system), lx, ly (local coordinates)
        0.5f, -0.5f, 0.0f, 1.0f, -1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, -1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, -1.0f, -1.0f,
    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &particleBuffer);
    glBindVertexArray(VAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*20, particle_quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, (GLvoid*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2 , 2, GL_FLOAT, GL_FALSE, 20, (GLvoid*)12);

    glBindVertexArray(0);


}


//Find the index of the first unused particle
int lastUsedParticle = 0;
int Generator::firstUnusedParticle(){

    //Search from last used particle
    for(int i=lastUsedParticle; i < particles.size(); i++){
        if(particles[i].getLifetime() < 0){
            lastUsedParticle=i;
            return i;
        }
    }

    for(int i=0; i<lastUsedParticle; i++){
        if (particles[i].getLifetime() < 0){
            lastUsedParticle = i;
            return i;
        }
    }

    //If all particles are alive, override the first one
    lastUsedParticle = 0;
    return 0;

}


void Generator::respawnParticle(Particle &particle){

    particle.setPosition(glm::vec3(0,0,0));
    particle.setVelocity(5*rand01()-0.5f,0.0f,5*rand01()-0.5f);
    particle.setLifetime(life);
    particle.setForce(glm::vec3(0));


}




void Generator::Update(GLfloat dt){

        //Add new particles
    for (int i=0; i<2; i++){
        int unusedParticle = firstUnusedParticle();
        respawnParticle(particles[unusedParticle]);
        if(particles.size() < nb_particles)
            particles.push_back(Particle(glm::vec3(0,0,0),glm::vec3(5*rand01()-0.5f,0.0f,5*rand01()-0.5f),0.95,false,rand01()*life,glm::vec3(0)));
    }

    //Update particles
    for (int i=0; i<particles.size(); i++){
        particles[i].setLifetime(particles[i].getLifetime()-dt);

        if (particles[i].getLifetime()>0.0f){


            if(method == Particle::UpdateMethod::Verlet)
            {
                if(particles[i].getLifetime() > (life - dt))
                {
                    glm::vec3 ini_pos = particles[i].getCurrentPosition();
                    glm::vec3 ini_vel = particles[i].getVelocity();
                    particles[i].setPreviousPosition(ini_pos.x - (ini_vel.x)*dt, ini_pos.y - (ini_vel.y)*dt, ini_pos.z - (ini_vel.z)*dt);
                }
            }

            particles[i].updateParticle(dt,method);

            //Check for collisions with the geometry
            plane_down.checkCollision(particles[i]);
            plane_up.checkCollision(particles[i]);
            plane_left.checkCollision(particles[i]);
            plane_right.checkCollision(particles[i]);
            plane_bottom.checkCollision(particles[i]);
            plane_front.checkCollision(particles[i]);

        }

        else{
            respawnParticle(particles[i]);
        }
    }

}




void Generator::Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){

    program->bind();

    program->setUniformValue("projection", proj);
    program->setUniformValue("normalMatrix", modelView.normalMatrix());

    Update(0.01f);

    int i=0;
    QMatrix4x4 MVtemp = modelView;

    for (Particle particle : particles){
        if(i>0){
            MVtemp.translate(particle.getCurrentPosition().x,particle.getCurrentPosition().y,particle.getCurrentPosition().z);
            MVtemp.rotate(90,QVector3D(-1,0,0));
            program->setUniformValue("modelview", MVtemp);
            m_models[0]->onRender();
            MVtemp=modelView;
        }
        i++;
    }

    program->release();

}








