#include "generator.h"

#include <random>
#define rand01() ((float)std::rand()/RAND_MAX)

Generator::Generator(){
}

Generator::Generator(int nb_particles) :
    nb_particles(nb_particles)
{
    particles.clear();
    this->initBuffers();

    if(spring1D_selected){
        for(int i=0; i<particles1D; i++)
            particles.push_back(Particle(glm::vec3(0,0,i*l_0),glm::vec3(0.0,0.0,0.0),0.70,false,life,glm::vec3(0,0,0)));
    }

    else if(spring2D_selected){
        for(int i=0; i<particles2D_row; i++)
        {
            for(int j=0; j<particles2D_col; j++)
                particles.push_back(Particle(glm::vec3(-i*l_0,0,-j*l_0),glm::vec3(0.0,0.0,0.0),0.70,false,life,glm::vec3(0,0,0)));

        }
    }

    else
        particles.push_back(Particle(glm::vec3(0,0,0),glm::vec3(rand01()-0.5f,5.0f,rand01()-0.5f),0.95,false,rand01()*life,glm::vec3(0,-9.8f,0)));


    plane_down = Plane(glm::vec3(0,-5,0),glm::vec3(0,1,0));
    plane_up= Plane(glm::vec3(0,5,0), glm::vec3(0,-1,0));
    plane_right = Plane(glm::vec3(5,0,0),glm::vec3(-1,0,0));
    plane_left = Plane(glm::vec3(-5,0,0),glm::vec3(1,0,0));
    plane_front = Plane(glm::vec3(0,0,5),glm::vec3(0,0,-1));
    plane_bottom = Plane(glm::vec3(0,0,-5),glm::vec3(0,0,1));
    triangle = Triangle(glm::vec3(-2,0,0),glm::vec3(-2,0,4),glm::vec3(4,0,2));
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
    particle.setVelocity(rand01()-0.5f,5.0f,rand01()-0.5f);
    particle.setLifetime(life);
    particle.setForce(glm::vec3(0,-9.8f,0));


}




void Generator::Update(GLfloat dt){

    if(spring1D_selected){
        spring1D();
    }

    else if(spring2D_selected){
        spring2D();
    }

    else{
        //Add new particles
        for (int i=0; i<2; i++){
            int unusedParticle = firstUnusedParticle();
            respawnParticle(particles[unusedParticle]);
            if(particles.size() < nb_particles)
                particles.push_back(Particle(glm::vec3(0,0,0),glm::vec3(rand01()-0.5f,5.0f,rand01()-0.5f),0.95,false,rand01()*life,glm::vec3(0,-9.8f,0)));
        }
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
        if(spring1D_selected||spring2D_selected){
            program->setUniformValue("offset", particle.getCurrentPosition().x,particle.getCurrentPosition().y,particle.getCurrentPosition().z);
            program->setUniformValue("color",0.4,0.0,0.8,1.0);
            program->setUniformValue("radius",radius);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindVertexArray(0);
        }

        else if(i>0){
            program->setUniformValue("offset", particle.getCurrentPosition().x,particle.getCurrentPosition().y,particle.getCurrentPosition().z);
            program->setUniformValue("color",0.4,0.0,0.8,1.0);
            program->setUniformValue("radius",radius);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindVertexArray(0);
        }
        i++;

    }

    program->release();

}




void Generator::spring1D(){

    glm::vec3 F_spring; //force of the current spring

    for(int i=0; i<particles.size(); i++){

        particles[i].setForce(glm::vec3(0.0,0.0,0.0));

        if(i==0){
            particles[i].setFixed(true);
            F_spring = -spring_force(i,i+1,l_0,ke,kd);
        }

        else if(i==particles.size()-1){
            particles[i].addForce(F_spring);
        }

        else{
            particles[i].addForce(F_spring);
            F_spring = spring_force(i,i+1,l_0,ke,kd);
            particles[i].addForce(F_spring);
            F_spring = -F_spring;
        }

        particles[i].addForce(glm::vec3(0.0,-9.8,0.0));

    }

}


void Generator::spring2D(){

    for(int k=0; k<particles.size(); k++){

        int i = k/particles2D_row;
        int j = k%particles2D_col;

        particles[k].setForce(glm::vec3(0.0,0.0,0.0));

        if(k==0){
            particles[k].setFixed(true);
        }

        else if(k==particles.size()-1){
            //Streach
            particles[k].addForce(spring_force(k,i*particles2D_col+(j-1)%particles2D_col,l_0,ke,kd));
            particles[k].addForce(spring_force(k,(i-1)*particles2D_col+j%particles2D_col,l_0,ke,kd));

            //Shear
            particles[k].addForce(spring_force(k,(i-1)*particles2D_col+(j-1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
        }

        else if(i==0 && j==particles2D_col-1){
            particles[k].setFixed(true);
        }


        else if(i==particles2D_row-1 && j==0){
            particles[k].addForce(spring_force(k,i*particles2D_row+(j+1)%particles2D_col,l_0,ke,kd));
            particles[k].addForce(spring_force(k,(i-1)*particles2D_row+j%particles2D_col,l_0,ke,kd));

            //Shear
            particles[k].addForce(spring_force(k,(i-1)*particles2D_row+(j+1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));

        }


        else{

            if(i==0){
                //Streach
                particles[k].addForce(spring_force(k,i*particles2D_row+(j+1)%particles2D_col,l_0,ke,kd));
                particles[k].addForce(spring_force(k,i*particles2D_row+(j-1)%particles2D_col,l_0,ke,kd));
                particles[k].addForce(spring_force(k,(i+1)*particles2D_row+j%particles2D_col,l_0,ke,kd));
                //Shear
                particles[k].addForce(spring_force(k,(i+1)*particles2D_row+(j+1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
                particles[k].addForce(spring_force(k,(i+1)*particles2D_row+(j-1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
            }

            if(i==particles2D_row-1){
                //Streach
                particles[k].addForce(spring_force(k,i*particles2D_row+(j+1)%particles2D_col,l_0,ke,kd));
                particles[k].addForce(spring_force(k,i*particles2D_row+(j-1)%particles2D_col,l_0,ke,kd));
                particles[k].addForce(spring_force(k,(i-1)*particles2D_row+j%particles2D_col,l_0,ke,kd));

                //Shear
                particles[k].addForce(spring_force(k,(i-1)*particles2D_row+(j-1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
                particles[k].addForce(spring_force(k,(i-1)*particles2D_row+(j+1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
            }

            if(j==0){
                //Streach
                particles[k].addForce(spring_force(k,i*particles2D_row+(j+1)%particles2D_col,l_0,ke,kd));
                particles[k].addForce(spring_force(k,(i+1)*particles2D_row+j%particles2D_col,l_0,ke,kd));
                particles[k].addForce(spring_force(k,(i-1)*particles2D_row+j%particles2D_col,l_0,ke,kd));

                //Shear
                particles[k].addForce(spring_force(k,(i+1)*particles2D_row+(j+1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
                particles[k].addForce(spring_force(k,(i-1)*particles2D_row+(j+1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
            }

            if(j==particles2D_col-1){
                //Streach
                particles[k].addForce(spring_force(k,i*particles2D_row+(j-1)%particles2D_col,l_0,ke,kd));
                particles[k].addForce(spring_force(k,(i+1)*particles2D_row+j%particles2D_col,l_0,ke,kd));
                particles[k].addForce(spring_force(k,(i-1)*particles2D_row+j%particles2D_col,l_0,ke,kd));

                //Shear
                particles[k].addForce(spring_force(k,(i-1)*particles2D_row+(j-1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
                particles[k].addForce(spring_force(k,(i+1)*particles2D_row+(j-1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
            }

            if(i!=0 && j!=0 && i!=particles2D_row-1 && j!=particles2D_col-1) {
                //Streach
                particles[k].addForce(spring_force(k,i*particles2D_row+(j+1)%particles2D_col,l_0,ke,kd));
                particles[k].addForce(spring_force(k,i*particles2D_row+(j-1)%particles2D_col,l_0,ke,kd));
                particles[k].addForce(spring_force(k,(i+1)*particles2D_row+j%particles2D_col,l_0,ke,kd));
                particles[k].addForce(spring_force(k,(i-1)*particles2D_row+j%particles2D_col,l_0,ke,kd));

                //Shear
                particles[k].addForce(spring_force(k,(i+1)*particles2D_row+(j+1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
                particles[k].addForce(spring_force(k,(i-1)*particles2D_row+(j-1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
                particles[k].addForce(spring_force(k,(i+1)*particles2D_row+(j-1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));
                particles[k].addForce(spring_force(k,(i-1)*particles2D_row+(j+1)%particles2D_col,l_0*1.41f,ke_shear,kd_shear));

                //Bend
//                particles[k].addForce(spring_force(k,(i+1)*5+(j+1)%5,l_0*1.41f,ke,kd));
//                particles[k].addForce(spring_force(k,(i-1)*5+(j-1)%5,l_0*1.41f,ke,kd));
//                particles[k].addForce(spring_force(k,(i+1)*5+(j-1)%5,l_0*1.41f,ke,kd));
//                particles[k].addForce(spring_force(k,(i-1)*5+(j+1)%5,l_0*1.41f,ke,kd));


            }



        }

        particles[k].addForce(glm::vec3(0.0,-9.8,0.0));

    }
}


glm::vec3 Generator::spring_force(int id1, int id2, float l0, float kelas, float kdamp){

    Particle P1 = particles[id1];
    Particle P2 = particles[id2];

    glm::vec3 diff = P2.getCurrentPosition()-P1.getCurrentPosition();
    float distance = glm::length(diff);

    if(distance >0){
        return ((kelas*(distance - l0)+kdamp*(P2.getVelocity()-P1.getVelocity())*(diff/distance))*(diff/distance));
    }

    else
        return glm::vec3(0.0,0.0,0.0);
}





