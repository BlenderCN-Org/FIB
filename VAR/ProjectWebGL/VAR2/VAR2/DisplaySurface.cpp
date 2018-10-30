//
//  DisplaySurface.cpp
//  VAR2
//
//  Created by Nils Burlat on 02/11/2017.
//  Copyright © 2017 Nils Burlat. All rights reserved.
//

#include "DisplaySurface.hpp"

DisplaySurface::DisplaySurface(const Point& orig, const Vector& uvector, const Vector& vvector) : m_origin(orig), m_u(uvector), m_v(vvector)
{}

Vector DisplaySurface::normal()
{
    Vector v(m_u.dy() * m_v.dz() - m_u.dz() * m_v.dy(), m_u.dz() * m_v.dx() - m_u.dx() * m_v.dz(), m_u.dx() * m_v.dy() - m_u.dy() * m_v.dx() );
    v.normalize();
    return v;
}
