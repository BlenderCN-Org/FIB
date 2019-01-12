#include "generator.h"
#include <math.h>

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

    ModelData* data2;
    // load 'cally' model
    std::cout << "Loading 'cally' model ..." << std::endl;
    data2 = ModelLoader::getModel("cally", "/Users/Emy/GitHub/FIB/CA/EA/data/cally/", "/Users/Emy/GitHub/FIB/CA/EA/data/cally.cfg");
    if (!data2) {
        std::cerr << "Model initialization failed! (cally)" << std::endl;
        return;
    }
    models.push_back(data2);


}

Generator::Generator(int nb_particles) :
    nb_particles(nb_particles)
{

    timer.start();

    particles.clear();
    models.clear();
//    this->initBuffers();

    // preload models
    loadModels(modelsData);
    int NUM_MODELS = modelsData.size();

    Particle p = Particle(glm::vec3(0,0,0),glm::vec3(10*rand01()-0.5f,0.0f,10*rand01()-0.5f),0.0f,false,rand01()*life,glm::vec3(0));
    particles.push_back(p);


    for(int i=0; i<NUM_MODELS; i++){
        Model* m = new Model();
//        glm::vec3 blend = glm::normalize(glm::vec3(rand01(), rand01(), rand01()));
        m->setCoreModel(modelsData[i]->coreModel);
        m->setAnimationIds(modelsData[i]->animationIds);
        m->setModelScale(modelsData[i]->renderScale);
        m->onInit();
////        m->setMotionBlend(&blend[i], 0);
//        m->onUpdate(10*rand01());
        m->setLodLevel(0.5f);
        m_models.push_back(m);
    }

    models.push_back(m_models[0]);

    plane_down = Plane(glm::vec3(0,0,0),glm::vec3(0,1,0));
    plane_up= Plane(glm::vec3(0,10,0), glm::vec3(0,-1,0));
    plane_right = Plane(glm::vec3(10,0,0),glm::vec3(-1,0,0));
    plane_left = Plane(glm::vec3(-10,0,0),glm::vec3(1,0,0));
    plane_front = Plane(glm::vec3(0,0,10),glm::vec3(0,0,-1));
    plane_bottom = Plane(glm::vec3(0,0,-10),glm::vec3(0,0,1));

}



void Generator::initBuffers(){


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
    particle.setVelocity(rand01()+0.5f,0.0f,rand01()+0.5f);
    particle.setLifetime(life);
    particle.setForce(glm::vec3(0));


}




void Generator::Update(GLfloat dt){

     //Add new particles
    for (int i=0; i<2; i++){
        int unusedParticle = firstUnusedParticle();
        respawnParticle(particles[unusedParticle]);
        if(particles.size() < nb_particles){
            particles.push_back(Particle(glm::vec3(0,0,0),glm::vec3(rand01()+0.5f,0.0f,rand01()+0.5f),1.0f,false,rand01()*life,glm::vec3(0)));
            models.push_back(m_models[rand()%(m_models.size())]);
        }
    }

    //Update particles
    for (int i=0; i<particles.size(); i++){
//        particles[i].setLifetime(particles[i].getLifetime()-dt);

//        if (particles[i].getLifetime()>0.0f){


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

            //Check collisions with other particles
            for(int j=1; j<particles.size() ; j++){
                if(i!=j) particles[i].checkCollision(particles[j],radius);
            }


//        }

//        else{
//            respawnParticle(particles[i]);
//        }


    }

}




void Generator::Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){

//    float time = timer.elapsed() / 1000.0f;
//    std::cout << time << std::endl;


    glClear(GL_DEPTH_BUFFER_BIT);

    program->bind();

    program->setUniformValue("projection", proj);
    program->setUniformValue("normalMatrix", modelView.normalMatrix());

    Update(0.01f);

    QMatrix4x4 MVtemp = modelView;

    for(int i =1; i<particles.size(); i++){

        MVtemp.translate(particles[i].getCurrentPosition().x,particles[i].getCurrentPosition().y,particles[i].getCurrentPosition().z);
        MVtemp.rotate(90,QVector3D(-1,0,0));

        float angle = atan2(particles[i].getVelocity().x,particles[i].getVelocity().z)*180/M_PI;
        MVtemp.rotate(angle,QVector3D(0,0,1));


        program->setUniformValue("modelview", MVtemp);
        models[i]->setState(2,0.0f);

        models[i]->onUpdate(timer.elapsed() / (1000.0*glm::length(particles[i].getVelocity())));
        models[i]->onRender();
        MVtemp=modelView;
    }



    program->release();

    timer.restart();

}








