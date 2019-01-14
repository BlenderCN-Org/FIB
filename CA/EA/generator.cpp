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
    steeringPos.clear();

    // preload models
    loadModels(modelsData);
    int NUM_MODELS = modelsData.size();

    Particle p = Particle(glm::vec3(0,0,0),glm::vec3(10*rand01()-0.5f,0.0f,10*rand01()-0.5f),0.0f,false,rand01()*life,glm::vec3(0));
    particles.push_back(p);


    for(int i=0; i<NUM_MODELS; i++){
        Model* m = new Model();
        m->setCoreModel(modelsData[i]->coreModel);
        m->setAnimationIds(modelsData[i]->animationIds);
        m->setModelScale(modelsData[i]->renderScale);
        m->onInit();
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

    for(int i=0; i<nb_particles; i++){
        steeringPos.push_back(Sphere(glm::vec3(0,0.2f,0),0.2f));
        steeringPos.push_back(Sphere(glm::vec3(0,0.2f,0),0.2f));
    }



}



void Generator::Update(GLfloat dt){

     //Add new particles
    for (int i=0; i<2; i++){
        if(particles.size() < nb_particles){
            Particle newP = Particle(glm::vec3(0,0,0),glm::vec3(rand01()+0.5f,0.0f,rand01()+0.5f),1.0f,false,rand01()*life,glm::vec3(0));
            newP.currentAngle = atan2(newP.getVelocity().x,newP.getVelocity().z)*180/M_PI;
            particles.push_back(newP);
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
                particles[i].checkCollision(pathParticle,radius);
            }

            //Check steering
            checkSteering(particles[i], i);

    }
}




void Generator::Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){

    glClear(GL_DEPTH_BUFFER_BIT);

    program->bind();

    program->setUniformValue("projection", proj);
    program->setUniformValue("normalMatrix", modelView.normalMatrix());

    Update(0.01f);


    QMatrix4x4 MVtemp = modelView;

    for(int i=0; i<particles.size(); i++){  //Update of the walking characters around

        if(i!=0){
            MVtemp.translate(particles[i].getCurrentPosition().x,particles[i].getCurrentPosition().y,particles[i].getCurrentPosition().z);
            MVtemp.rotate(90,QVector3D(-1,0,0));

            float angle = atan2(particles[i].getVelocity().x,particles[i].getVelocity().z)*180/M_PI;
            float newA = updateAngle(particles[i],angle);
            MVtemp.rotate(newA,QVector3D(0,0,1));

            program->setUniformValue("modelview", MVtemp);
            models[i]->setState(2,0.0f);

            models[i]->onUpdate(timer.elapsed() / (1500.0f*glm::length(particles[i].getVelocity())));
            models[i]->onRender();
            MVtemp=modelView;
        }
    }

    if(pathfinding){  //Update of the character moving along the way  (only if pathfinding is activated)
        updatePath();

        MVtemp.translate(pathParticle.getCurrentPosition().x,pathParticle.getCurrentPosition().y,pathParticle.getCurrentPosition().z);
        MVtemp.rotate(90,QVector3D(-1,0,0));

        float angle = atan2(pathParticle.getVelocity().x,pathParticle.getVelocity().z)*180/M_PI;
        MVtemp.rotate(updateAngle(pathParticle,angle),QVector3D(0,0,1));

        program->setUniformValue("modelview", MVtemp);

        models[0]->onRender();
        MVtemp=modelView;
    }


    program->release();

    timer.restart();

}


//Update smoothly the change of directions
float Generator::updateAngle(Particle& P, float angle){
    float CA = P.currentAngle;
    float newA;

    if(abs(angle-CA)>2.0f){
        if(abs(angle-CA)>180)
            newA = CA - (360-(angle-CA))/20.0;
        else
            newA = CA + (angle-CA)/20.0;
        P.currentAngle = newA;
    }
    else{
        newA = angle;
        P.currentAngle=newA;
    }
    return newA;

}



//Add the character that follows the path
void Generator::addPathCharacter(int x, int y){
    pathfinding = true;
    pathParticle=Particle(glm::vec3(-size_x+2*x+1.0f,0,-size_y+2*y+1.0f),glm::vec3(1,0,0),1.0f,false,1.0,glm::vec3(0));
    pathParticle.currentAngle=atan2(pathParticle.getVelocity().x,pathParticle.getVelocity().z)*180/M_PI;
    startx=x;
    starty=y;
    updatePath();

}


//Follow the waypoints along the path
void Generator::updatePath(){

    int next_x = Path[Path.size()-2*(current_id+1)];
    int next_y = Path[Path.size()-2*(current_id+1)+1];

    glm::vec3 currentPos = pathParticle.getCurrentPosition();

    glm::vec3 targetPos = glm::vec3(-size_x+2*next_x+1.0f,0,-size_y+2*next_y+1.0f);
    float prevDist = glm::length(targetPos-currentPos);
    pathParticle.setVelocity(glm::normalize(targetPos-currentPos));

    pathParticle.updateParticle(0.01f,method);

    currentPos = pathParticle.getCurrentPosition();
    float distance = glm::length(targetPos-currentPos);

    if(distance-prevDist >0)
    if(distance < 0.5f){
        if(current_id < (Path.size()/2-1))
            current_id+=1;
        else{
            current_id=0;
            pathParticle.setPosition(glm::vec3(-size_x+2*startx+1.0f,0,-size_y+2*starty+1.0f));
        }
    }
}



//Update steering behavior
void Generator::checkSteering(Particle &P, int id){

    glm::vec3 farPos = P.getCurrentPosition() + glm::normalize(P.getVelocity())*MaxAhead;
    glm::vec3 closePos = P.getCurrentPosition() + glm::normalize(P.getVelocity())*(MaxAhead/2.0f);

    steeringPos[id+1].center = farPos;
    steeringPos[id].center = closePos;

    bool flagParticles = false;
    bool flagObstacles = false;
    int idParticle, idObstacle;

    for(int i=0; i<obstacles.size(); i++){

        float radius = obstacles[i].radi + 0.5f;
        glm::vec3 obsPos = obstacles[i].center;
        float dist1 = glm::length(farPos - obsPos);
        float dist2 = glm::length(closePos - obsPos);

        if(dist2<radius){
            flagObstacles=true;
            idObstacle = i;
        }

        if(dist1<radius){
            flagObstacles=true;
            idObstacle = i;
        }

    }

    for(int i=1; i<particles.size(); i++){

        if(i!=id){
            float radius = 1.0f;
            glm::vec3 obsPos = particles[i].getCurrentPosition();
            float dist1 = glm::length(farPos - obsPos);
            float dist2 = glm::length(closePos - obsPos);

            if(dist2<radius){ //If there is no collision (far detection)
                flagParticles=true;
                idParticle = i;
            }

            if(dist1<radius){  //If there is collision (close detection)
                flagParticles=true;
                idParticle = i;
            }
        }

    }


    if(flagObstacles){
        float normV = glm::length(P.getVelocity());
        glm::vec3 newV = normV*glm::normalize(P.getVelocity() + (farPos - obstacles[idObstacle].center)*MaxDeviance);
        newV[1]=0.0f;
        P.setVelocity(newV);
    }


    if(flagParticles){
        float normV = glm::length(P.getVelocity());
        glm::vec3 newV = normV*glm::normalize(P.getVelocity() + (farPos - particles[idParticle].getCurrentPosition())*MaxDeviance);
        newV[1]=0.0f;
        P.setVelocity(newV);
    }

}



void Generator::DisplayObstacles(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){

//    for(int i=2; i<steeringPos.size()-1;i+=2){
//        steeringPos[i].initBuffers();
//        steeringPos[i].Display(program,proj,modelView);
//    }
}







