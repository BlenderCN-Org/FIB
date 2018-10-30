#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm.hpp>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "particle.h"

class Geometry{
    virtual void setPosition(const glm::vec3& newPos) = 0;
    virtual bool isInside(const glm::vec3& point) = 0;
};

class Plane : public Geometry {

public:
    glm::vec3 normal;
    float dconst;
    Plane(){}
    virtual ~Plane() {}
    Plane(const glm::vec3& point, const glm::vec3& normalVect);
    Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);

    GLuint planeBuffer, planeVAO;

    void setPosition(const glm::vec3& newPos);
    bool isInside(const glm::vec3& point);
    float distPoint2Plane(const glm::vec3& point);
    glm::vec3 closestPointInPlane(const glm::vec3& point);
    bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
    void checkCollision(Particle &p);
    void initBuffers();
    void Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView);
};

class Triangle : public Plane {
public:
    glm::vec3 vertex1, vertex2, vertex3;
    Triangle(){}
    virtual ~Triangle() {}
    Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);

    GLuint triangleBuffer, triangleVAO;

    void setPosition(const glm::vec3& newPos);
    bool isInside(const glm::vec3& point);
    float computeArea(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3);
    bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
    void checkCollision(Particle &p);
    void initBuffers();
    void Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView);

};

class Sphere : public Geometry {
public:
    glm::vec3 center;
    float radi;
    Sphere(){}
    Sphere(const glm::vec3& point, const float& radious);
    ~Sphere() {}

    GLuint sphereBuffer, sphereVAO;

    void setPosition(const glm::vec3& newPos);
    bool isInside(const glm::vec3& point);
    bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
    void checkCollision(Particle &p);
    void initBuffers();
    void Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView);
};

#endif
