//
//  Vector.cpp
//  VAR2
//
//  Created by Nils Burlat on 02/11/2017.
//  Copyright © 2017 Nils Burlat. All rights reserved.
//

#include "Vector.hpp"

#include <cmath>

Vector::Vector(float dx, float dy, float dz) : m_dx(dx), m_dy(dy), m_dz(dz)
{}

Vector::Vector(const Vector& v) : m_dx(v.m_dx), m_dy(v.m_dy), m_dz(v.m_dz)
{}

void Vector::normalize()
{
    float norm = this->norm();
    m_dx /= norm;
    m_dy /= norm;
    m_dz /= norm;
}

float Vector::scalar(Vector& v)
{
    return m_dx * v.dx() + m_dy * v.dy() + m_dz * v.dz();
}

float Vector::scalar(Point& p)
{
    return m_dx * p.x() + m_dy * p.y() + m_dz * p.z();
}

float Vector::norm()
{
    return std::sqrt(m_dx*m_dx + m_dy * m_dy + m_dz * m_dz);
}

void Vector::print()
{
    printf("(%f, %f, %f)\n", m_dx, m_dy, m_dz);
}
