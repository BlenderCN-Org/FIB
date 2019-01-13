#pragma once
#include "geometry.h"

//****************************************************
// Plane
//****************************************************

Plane::Plane(const glm::vec3& point, const glm::vec3& normalVect){
    normal = glm::normalize(normalVect);
    dconst = -glm::dot(point, normal);
};


Plane::Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2){
    glm::vec3 v1 = point1 - point0;
    glm::vec3 v2 = point2 - point0;
    normal = glm::normalize(glm::cross(v1, v2));
    dconst = -glm::dot(point0, normal);
};

void Plane::setPosition(const glm::vec3& newPos){
    dconst = -glm::dot(newPos, normal);
};

bool Plane::isInside(const glm::vec3& point){
    float dist;
    dist = glm::dot(point, normal) + dconst;
    if (dist > 1.e-7)
        return false;
    else
        return true;
};

float Plane::distPoint2Plane(const glm::vec3& point){
    float dist;
    return dist = glm::dot(point, normal) + dconst;
};

glm::vec3 Plane::closestPointInPlane(const glm::vec3& point){
    glm::vec3 closestP;
    float r = (-dconst - glm::dot(point, normal));
    return closestP = point + r*normal;
};

bool Plane::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){
    if (distPoint2Plane(point1)*distPoint2Plane(point2) > 0)	return false;
    float r = (-dconst - glm::dot(point1, normal)) / glm::dot((point2 - point1), normal);
    pTall = (1 - r)*point1 + r*point2;
    return true;
};


void Plane::checkCollision(Particle &p){
    float disant, disact;
    disact = distPoint2Plane(p.getPreviousPosition());
    disant = disact;
    disact = distPoint2Plane(p.getCurrentPosition());

    if(disant*disact < 0.0f){
        float epsilon = p.getBouncing(); //Elastic collision - bouncing coefficient

        glm::vec3 newPos = p.getCurrentPosition() - (1+epsilon)*(glm::dot(normal,p.getCurrentPosition())+dconst)*normal;
        p.setPosition(newPos.x,newPos.y,newPos.z);

        glm::vec3 newVel = p.getVelocity() - (1+epsilon)*(glm::dot(normal,p.getVelocity()))*normal;
        p.setVelocity(newVel.x,newVel.y,newVel.z);
    }
}


void Plane::initBuffers(){

    GLfloat quad[12] = {
        1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, -1.0f
    };

    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeBuffer);
    glBindVertexArray(planeVAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, planeBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (GLvoid*)0);

    glBindVertexArray(0);

}


void Plane::Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    program->bind();

    program->setUniformValue("projection", proj);

    glm::vec3 position = -dconst*normal;

    modelView.translate(position.x,position.y,position.z);
    modelView.scale(10,10,10);
    modelView.rotate(-normal.x*90,0.0f,0.0f,1.0f);
    modelView.rotate(normal.z*90,1.0f,0.0f,0.0f);

    program->setUniformValue("modelview", modelView);
    program->setUniformValue("normalMatrix", modelView.normalMatrix());
    program->setUniformValue("color",1.0,0.5,0.0,1.0);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glBindVertexArray(0);

    program->release();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

}



//****************************************************
// Triangle
//****************************************************


Triangle::Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2){
    vertex1 = point0;
    vertex2 = point1;
    vertex3 = point2;
}


void Triangle::setPosition(const glm::vec3& newPos){
    vertex1 += newPos;
    vertex2 += newPos;
    vertex3 += newPos;
}

bool Triangle::isInside(const glm::vec3& point){
    float area1,area2,area3,area4;
    Plane plane = Plane(vertex1,vertex2,vertex3);
    glm::vec3 X = plane.closestPointInPlane(point);

    area1 = computeArea(X,vertex2,vertex3);
    area2 = computeArea(vertex1,X,vertex3);
    area3 = computeArea(vertex1,vertex2,X);
    area4 = computeArea(vertex1,vertex2,vertex3);

    return ((area1+area2+area3-area4) <= 0);
}

float Triangle::computeArea(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3){
    return (0.5f * glm::length(glm::cross((point2-point1),(point3-point1))));
}

bool Triangle::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){
    return false;
}


void Triangle::checkCollision(Particle &p){
    Plane plane = Plane(vertex1,vertex2,vertex3);
    if (this->isInside(p.getCurrentPosition())){
        plane.checkCollision(p);
        plane.normal = -normal;
        plane.checkCollision(p);
    }
}


void Triangle::initBuffers(){

    std::vector<float> vertices;
    vertices.clear();

    vertices.push_back(vertex1.x);
    vertices.push_back(vertex1.y);
    vertices.push_back(vertex1.z);
    vertices.push_back(vertex2.x);
    vertices.push_back(vertex2.y);
    vertices.push_back(vertex2.z);
    vertices.push_back(vertex3.x);
    vertices.push_back(vertex3.y);
    vertices.push_back(vertex3.z);

    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleBuffer);
    glBindVertexArray(triangleVAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, triangleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)nullptr);

    glBindVertexArray(0);

}


void Triangle::Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    program->bind();

    program->setUniformValue("projection", proj);
    program->setUniformValue("modelview", modelView);
    program->setUniformValue("normalMatrix", modelView.normalMatrix());

    program->setUniformValue("color",0.0,0.0,0.7,1.0);

    glBindVertexArray(triangleVAO);
    glDrawArrays(GL_TRIANGLES,0,3);
    glBindVertexArray(0);

    program->release();

    //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}


//****************************************************
// Sphere
//****************************************************

Sphere::Sphere(const glm::vec3& point, const float& radious){
    center = point;
    radi = radious;
}

void Sphere::setPosition(const glm::vec3& newPos){
    center=newPos;
}

bool Sphere::isInside(const glm::vec3& point){
    float dist;
    dist = glm::length(point-center);
    if (dist > radi)
        return false;
    else
        return true;
}

bool Sphere::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){
    if (isInside(point1) && isInside(point2))	return false;

    glm::vec3 v = point2-point1;
    float alpha = glm::dot(v,v);
    float beta = 2*glm::dot(v,point1-center);
    float gamma = glm::dot(center,center) + glm::dot(point1,point1) - 2*dot(point1,center) - radi*radi;
    float delta = beta*beta - 4*alpha*gamma;

    float lambda1 = (-beta + sqrt(delta)) / (2*alpha);
    float lambda2 = (-beta - sqrt(delta)) / (2*alpha);

    if(lambda1>0 && lambda1 <1){
        pTall = point1 + lambda1 * v;
    }
    else {
        pTall = point1 + lambda2 * v;
    }

    return true;
}


void Sphere::checkCollision(Particle &p){
    if (this->isInside(p.getCurrentPosition())){
        glm::vec3 point(0.0f);
        this->intersecSegment(p.getPreviousPosition(),p.getCurrentPosition(),point);
        Plane plane = Plane(point,glm::normalize(point-center));
        plane.checkCollision(p);

    }
}


void Sphere::initBuffers(){

    GLfloat quad[20] = {
        //x, y, z (OpenGL coordinates system), lx, ly (local coordinates)
        2*radi, -2*radi, 0.0f, radi, -radi,
        2*radi, 2*radi, 0.0f, radi, radi,
        -2*radi, 2*radi, 0.0f, -radi, radi,
        -2*radi, -2*radi, 0.0f, -radi, -radi,
    };

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereBuffer);
    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*20, quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, (GLvoid*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2 , 2, GL_FLOAT, GL_FALSE, 20, (GLvoid*)12);

    glBindVertexArray(0);

}


void Sphere::Display(QOpenGLShaderProgram *program, QMatrix4x4 proj, QMatrix4x4 modelView){

    program->bind();

    program->setUniformValue("projection", proj);
    program->setUniformValue("modelview", modelView);
    program->setUniformValue("normalMatrix", modelView.normalMatrix());

    program->setUniformValue("color",1.0,0.0,0.0,1.0);
    program->setUniformValue("offset", center.x,center.y,center.z);
    program->setUniformValue("radius",radi);
    glBindVertexArray(sphereVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    program->release();
}
