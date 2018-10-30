//
//  Vector.hpp
//  VAR2
//
//  Created by Nils Burlat on 02/11/2017.
//  Copyright © 2017 Nils Burlat. All rights reserved.
//

#ifndef Vector_hpp
#define Vector_hpp

#include <stdio.h>
#include "Point.hpp"

class Vector
{
public:
    
    Vector();
    Vector(const Vector& v);
    Vector(float dx, float dy, float dz);
    
    inline float dx() { return m_dx; };
    inline float dy() { return m_dy; };
    inline float dz() { return m_dz; };
    
    inline void dx(float dx) { m_dx = dx; };
    inline void dy(float dy) { m_dy = dy; };
    inline void dz(float dz) { m_dz = dz; };
    
    void normalize();
    
    float scalar(Vector& v);
    float scalar(Point& v);
    
    void print();
    
private:
    float m_dx;
    float m_dy;
    float m_dz;
};

#endif /* Vector_hpp */
