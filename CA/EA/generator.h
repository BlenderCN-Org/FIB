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

private:

    int nb_particles;

    //Buffers
    GLuint particleBuffer, planeBuffer;

    //Find index of a particle that is unused (already dead or not yet living)
    int firstUnusedParticle();
    //Respawns particle
    void respawnParticle(Particle &particle);

};

#endif // GENERATOR_H
