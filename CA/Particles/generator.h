#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "particle.h"
#include "geometry.h"


class Generator
{

public:
//    enum class UpdateMethod : std::int8_t { EulerOrig, EulerSemi, Verlet };
    Generator();
    Generator(int nb_particles);

    //Update all particles
    void Update(GLfloat dt);
    //Render particles
    void Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView);
    //Init buffers for display
    void initBuffers();
    std::vector<Particle> particles;
    GLuint VAO, planeVAO;

    float radius=0.5f;
    int life = 50;

    Plane plane_down, plane_up, plane_right, plane_left, plane_bottom, plane_front;
    Triangle triangle;
    Sphere sphere;

    Particle::UpdateMethod method = Particle::UpdateMethod::EulerOrig;

    float ke = 500.0;
    float l_0 = 1;
    float kd = 10;

    float ke_shear=1.0;
    float kd_shear=0.001;

    bool spring1D_selected=false; //ke=500 kd=10
    bool spring2D_selected=true;

private:

    int nb_particles;
    int particles1D=9;
    int particles2D_row=10;
    int particles2D_col=10;

    //Buffers
    GLuint particleBuffer, planeBuffer;

    //Find index of a particle that is unused (already dead or not yet living)
    int firstUnusedParticle();
    //Respawns particle
    void respawnParticle(Particle &particle);

    void spring1D(); //Compute forces for the 1D-spring system
    void spring2D(); //Compute forces for the 2D-spring system
    glm::vec3 spring_force(int id1, int id2, float l0, float kelas, float kdamp); //Compute the spring force between 2 particles

};

#endif // GENERATOR_H
