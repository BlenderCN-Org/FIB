#include "particle.h"


Particle::Particle()
{
}



Particle::Particle(glm::vec3 pos, glm::vec3 vel, float bouncing, bool fixed, int lifetime, glm::vec3 force) :
m_currentPosition(pos), m_previousPosition(pos), m_force(force), m_velocity(vel), m_bouncing(bouncing), m_lifetime(lifetime), m_fixed(fixed)
{
}


Particle::~Particle()
{
}

//setters
void Particle::setPosition(const float& x, const float& y, const float& z)
{
    glm::vec3 pos(x,y,z);
    m_currentPosition =  pos;
}
void Particle::setPosition(glm::vec3 pos)
{
    m_currentPosition = pos;
}

void Particle::setPreviousPosition(const float& x, const float& y, const float& z)
{
    glm::vec3 pos(x, y, z);
    m_previousPosition = pos;
}

void Particle::setPreviousPosition(glm::vec3 pos)
{
    m_previousPosition = pos;
}

void Particle::setForce(const float& x, const float& y, const float& z)
{
    glm::vec3 force(x, y, z);
    m_force = force;
}

void Particle::setForce(glm::vec3 force)
{
    m_force = force;
}

void Particle::addForce(const float& x, const float& y, const float& z)
{
    glm::vec3 force(x,y,z);
    m_force += force;
}

void Particle::addForce(glm::vec3 force)
{
    m_force += force;
}

void Particle::setVelocity(const float& x, const float& y, const float& z)
{
    glm::vec3 vel(x,y,z);
    m_velocity = vel;
}

void Particle::setVelocity(glm::vec3 vel)
{
    m_velocity = vel;
}

void Particle::setBouncing(float bouncing)
{
    m_bouncing = bouncing;
}

void Particle::setLifetime(float lifetime)
{
    m_lifetime = lifetime;
}

void Particle::setFixed(bool fixed)
{
    m_fixed = fixed;
}

//getters
glm::vec3 Particle::getCurrentPosition()
{
    return m_currentPosition;
}

glm::vec3 Particle::getPreviousPosition()
{
    return m_previousPosition;
}

glm::vec3 Particle::getForce()
{
    return m_force;
}

glm::vec3 Particle::getVelocity()
{
    return m_velocity;
}

float Particle::getBouncing()
{
    return m_bouncing;
}

float Particle::getLifetime()
{
    return m_lifetime;
}

bool Particle::isFixed()
{
    return m_fixed;
}

void Particle::updateParticle(const float& dt, UpdateMethod method)
{
    if (!m_fixed & (m_lifetime > 0))
    {
        switch (method)
        {
        case UpdateMethod::EulerOrig:
        {
            m_previousPosition = m_currentPosition;
            m_currentPosition += m_velocity*dt;
            m_velocity += m_force*dt;
        }
            break;
        case UpdateMethod::EulerSemi:
        {
            m_previousPosition = m_currentPosition;
            m_velocity += m_force*dt;
            m_currentPosition += m_velocity*dt;
        }
            break;
        case UpdateMethod::Verlet:
        {
            float kd = 0.9f;

            m_velocity = (m_currentPosition - m_previousPosition)/dt;
            m_previousPosition = m_currentPosition;
            m_currentPosition += kd*m_velocity*dt + m_force*dt*dt;

//            glm::vec3 temp = m_currentPosition;
//            m_velocity += m_force*dt;
//            m_currentPosition += 0.3f*(m_currentPosition - m_previousPosition) + m_velocity*dt;
//            m_previousPosition = temp;
        }
            break;
        }
    }
    return;
}


