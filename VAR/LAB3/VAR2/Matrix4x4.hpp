//
//  Matrix4x4.hpp
//  VAR2
//
//  Created by Nils Burlat on 02/11/2017.
//  Copyright © 2017 Nils Burlat. All rights reserved.
//

#ifndef Matrix4x4_hpp
#define Matrix4x4_hpp

#include <stdio.h>
#include <vector>

class Matrix4x4
{
public:
    Matrix4x4();
    
    void print();
    
    std::vector<std::vector<float>> m;
};

#endif /* Matrix4x4_hpp */
