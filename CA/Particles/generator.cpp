#include "generator.h"

#include <random>
#define rand01() ((float)std::rand()/RAND_MAX)

Generator::Generator(int nb_particles) :
    nb_particles(nb_particles)
{
    particles.clear();
    this->initBuffers();
    for(int i=0; i<nb_particles; i++)
    {
        particles.push_back(Particle(glm::vec3(0,0,0),glm::vec3(rand01()-0.5f,5.0f,rand01()-0.5f),0.95,false,rand01()*life,glm::vec3(0,-9.8f,0)));

    }

    plane_down = Plane(glm::vec3(0,-5,0),glm::vec3(0,1,0));
    plane_up= Plane(glm::vec3(0,5,0), glm::vec3(0,-1,0));
    plane_right = Plane(glm::vec3(5,0,0),glm::vec3(-1,0,0));
    plane_left = Plane(glm::vec3(-5,0,0),glm::vec3(1,0,0));
    plane_front = Plane(glm::vec3(0,0,5),glm::vec3(0,0,-1));
    plane_bottom = Plane(glm::vec3(0,0,-5),glm::vec3(0,0,1));
    triangle = Triangle(glm::vec3(-2,0,0),glm::vec3(-2,0,4),glm::vec3(4,0,2));
    //sphere = Sphere(glm::vec3(-3,-3,-3),3);
    sphere = Sphere(glm::vec3(-3,-3,0),2);

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


//    glGenVertexArrays(1, &planeVAO);
//    glGenBuffers(1, &planeBuffer);
//    glBindVertexArray(planeVAO);
//    // Fill mesh buffer
//    glBindBuffer(GL_ARRAY_BUFFER, planeBuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*20, particle_quad, GL_STATIC_DRAW);

//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, (GLvoid*)0);

//    glBindVertexArray(0);


}


//Find the index of the first unused particle
int lastUsedParticle = 0;
int Generator::firstUnusedParticle(){

    //Search from last used particle
    for(int i=lastUsedParticle; i < nb_particles; i++){
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
    particle.setVelocity(rand01()-0.5f,5.0f,rand01()-0.5f);
    particle.setLifetime(life);


}




void Generator::Update(GLfloat dt){

    //Add new particles
    for (int i=0; i<50; i++){
        int unusedParticle = firstUnusedParticle();
        respawnParticle(particles[unusedParticle]);
    }

    //Update particles
    for (int i=0; i<nb_particles; i++){
        particles[i].setLifetime(particles[i].getLifetime()-dt);

        float disact, disant;
//        disact = plane_down.distPoint2Plane(particles[i].getCurrentPosition());
        if (particles[i].getLifetime()>0.0f){

//            particles[i].updateParticle(dt,Particle::UpdateMethod::EulerOrig);
            particles[i].updateParticle(dt,method);


            plane_down.checkCollision(particles[i]);
            plane_up.checkCollision(particles[i]);
            plane_left.checkCollision(particles[i]);
            plane_right.checkCollision(particles[i]);
            plane_bottom.checkCollision(particles[i]);
            plane_front.checkCollision(particles[i]);
            triangle.checkCollision(particles[i]);
            sphere.checkCollision(particles[i]);

        }

        else{
            respawnParticle(particles[i]);
        }
    }

}




void Generator::Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){

    program->bind();

    program->setUniformValue("projection", proj);
    program->setUniformValue("modelview", modelView);
    program->setUniformValue("normalMatrix", modelView.normalMatrix());

    Update(0.01f);

    int i=0;
    for (Particle particle : particles)
    {
        if(i>0){
            program->setUniformValue("offset", particle.getCurrentPosition().x,particle.getCurrentPosition().y,particle.getCurrentPosition().z);
            //program->setUniformValue("color", abs(particle.getCurrentPosition().x),abs(particle.getCurrentPosition().y),abs(particle.getCurrentPosition().z),1.0);
            program->setUniformValue("color",0.8,0.4,0.2,1.0);
            program->setUniformValue("radius",radius);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindVertexArray(0);
        }
        i++;

    }

    program->release();

}
