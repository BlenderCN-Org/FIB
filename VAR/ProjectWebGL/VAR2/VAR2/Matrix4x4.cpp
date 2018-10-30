//
//  Matrix4x4.cpp
//  VAR2
//
//  Created by Nils Burlat on 02/11/2017.
//  Copyright © 2017 Nils Burlat. All rights reserved.
//

#include "Matrix4x4.hpp"
#include <string>

Matrix4x4::Matrix4x4()
{
    m = std::vector<std::vector<float>>();
    for(int i = 0; i < 4; i++)
    {
        m.push_back(std::vector<float>());
        for(int j = 0; j < 4; j++)
        {
            m[i].push_back(99);
        }
    }
}

void Matrix4x4::print()
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            printf("%f\t\t", m[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}
