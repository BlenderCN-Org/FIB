#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include<QElapsedTimer>

#include "particle.h"
#include "geometry.h"
#include "cal3dExt/model.h"
#include "cal3dExt/modelloader.h"


#include <cal3d/cal3d.h>



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
    std::vector<Model*> models;
    GLuint VAO, planeVAO;

    float radius=0.5f;
    int life = 50;

    Plane plane_down, plane_up, plane_right, plane_left, plane_bottom, plane_front;

    Particle::UpdateMethod method = Particle::UpdateMethod::EulerOrig;

    std::vector<ModelData*> modelsData;
    std::vector<Model*> m_models;
//    std::vector<Agent*> m_agents;

    int nb_particles;

private:


    //Buffers
    GLuint particleBuffer, planeBuffer;

    //Find index of a particle that is unused (already dead or not yet living)
    int firstUnusedParticle();
    //Respawns particle
    void respawnParticle(Particle &particle);

    QElapsedTimer timer;

};

#endif // GENERATOR_H
