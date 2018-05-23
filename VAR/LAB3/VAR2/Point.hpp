//
//  Point.hpp
//  VAR2
//
//  Created by Nils Burlat on 02/11/2017.
//  Copyright © 2017 Nils Burlat. All rights reserved.
//

#ifndef Point_hpp
#define Point_hpp

#include <stdio.h>

class Point
{
public:
    
    Point();
    Point(float x, float y, float z);
    
    inline float x() { return m_x; };
    inline float y() { return m_y; };
    inline float z() { return m_z; };
    
    inline void x(float x) { m_x = x; };
    inline void y(float y) { m_y = y; };
    inline void z(float z) { m_z = z; };
    
private:
    float m_x;
    float m_y;
    float m_z;
};

#endif /* Point_hpp */
