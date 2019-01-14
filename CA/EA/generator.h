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
    Generator();
    Generator(int nb_particles);

    //Vector of particles
    std::vector<Particle> particles;
    int nb_particles;
    float radius=0.5f;
    int life = 50;
    //Update all particles
    void Update(GLfloat dt);
    //Render particles
    void Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView);
    //Limit planes of the world
    Plane plane_down, plane_up, plane_right, plane_left, plane_bottom, plane_front;
    GLuint VAO, planeVAO;
    //Update method used : Euler
    Particle::UpdateMethod method = Particle::UpdateMethod::EulerOrig;


    bool pathfinding = false;
    //Add character for pathfinding
    void addPathCharacter(int x, int y);
    Particle pathParticle;
    //Path obtained from Astar
    std::vector<int> Path;
    int current_id=0;
    int size_x, size_y, startx, starty;
    //Obstacles for the pathfinding
    std::vector<Sphere> obstacles;


    QElapsedTimer timer;


private:

    //Buffers
    GLuint particleBuffer, planeBuffer;

    void checkSteering(Particle &P, int id);
    float MaxAhead = 1.0f;
    float MaxDeviance = 0.01f;

    void updatePath();
    float updateAngle(Particle &P, float angle);

    //Cal3d models
    void loadModels(std::vector<ModelData*>& models);
    std::vector<Model*> models;
    std::vector<ModelData*> modelsData;
    std::vector<Model*> m_models;

};

#endif // GENERATOR_H
