#include "generator.h"
#include <math.h>

#include <random>
#define rand01() ((float)std::rand()/RAND_MAX)

Generator::Generator(){
}


void Generator::loadModels(std::vector<ModelData*>& models)
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
    obstacles.clear();
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
        if(particles.size() < nb_particles){
            particles.push_back(Particle(glm::vec3(0,0,0),glm::vec3(rand01()+0.5f,0.0f,rand01()+0.5f),1.0f,false,rand01()*life,glm::vec3(0)));
            models.push_back(m_models[rand()%(m_models.size())]);
        }
    }

    //Update particles
    for (int i=0; i<particles.size(); i++){

            particles[i].updateParticle(dt,method);

            //Check for collisions with the geometry
            plane_down.checkCollision(particles[i]);
            plane_up.checkCollision(particles[i]);
            plane_left.checkCollision(particles[i]);
            plane_right.checkCollision(particles[i]);
            plane_bottom.checkCollision(particles[i]);
            plane_front.checkCollision(particles[i]);

            for(int j=0; j<obstacles.size(); j++){
                obstacles[j].checkCollision(particles[i]);
            }

            //Check collisions with other particles
            for(int j=1; j<particles.size() ; j++){
                if(i!=j) particles[i].checkCollision(particles[j],radius);
            }

    }
}




void Generator::Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){

    glClear(GL_DEPTH_BUFFER_BIT);

    program->bind();

    program->setUniformValue("projection", proj);
    program->setUniformValue("normalMatrix", modelView.normalMatrix());

    Update(0.01f);


    QMatrix4x4 MVtemp = modelView;

    for(int i=0; i<particles.size(); i++){

        if(i!=0){
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
    }

    if(pathfinding){
        updatePath();

        MVtemp.translate(pathParticle.getCurrentPosition().x,pathParticle.getCurrentPosition().y,pathParticle.getCurrentPosition().z);
        MVtemp.rotate(90,QVector3D(-1,0,0));

        float angle = atan2(pathParticle.getVelocity().x,pathParticle.getVelocity().z)*180/M_PI;
        MVtemp.rotate(angle,QVector3D(0,0,1));

        program->setUniformValue("modelview", MVtemp);

        models[0]->onRender();
        MVtemp=modelView;
    }


    program->release();

    timer.restart();

}



void Generator::DisplayObstacles(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){

    for(int i=0; i<obstacles.size();i++){
        obstacles[i].Display(program,proj,modelView);
    }
}




void Generator::addPathCharacter(int x, int y){
    pathfinding = true;
    pathParticle=Particle(glm::vec3(-size_x+2*x+1.0f,0,-size_y+2*y+1.0f),glm::vec3(1,0,0),1.0f,false,1.0,glm::vec3(0));
    updatePath();

}



void Generator::updatePath(){

    int next_x = Path[Path.size()-2*(current_id+1)];
    int next_y = Path[Path.size()-2*(current_id+1)+1];

    std::cout << "x:  " << next_x << " -  y:  " << next_y << std::endl;

    glm::vec3 currentPos = pathParticle.getCurrentPosition();

    glm::vec3 targetPos = glm::vec3(-size_x+2*next_x+1.0f,0,-size_y+2*next_y+1.0f);
    float prevDist = glm::length(targetPos-currentPos);
    pathParticle.setVelocity(glm::normalize(targetPos-currentPos));

    pathParticle.updateParticle(0.01f,method);

    currentPos = pathParticle.getCurrentPosition();
    float distance = glm::length(targetPos-currentPos);

    if(distance-prevDist >0)
        std::cout << "muy mal" << std::endl;
//    std::cout << distance << std::endl;
    if(distance < 0.5f){
        if(current_id < (Path.size()/2-1))
            current_id+=1;
        else
            current_id=0;
    }

//    std::cout << current_id << std::endl;

}





