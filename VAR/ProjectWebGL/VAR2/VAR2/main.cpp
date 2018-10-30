//
//  main.cpp
//  VAR2
//
//  Created by Nils Burlat on 26/10/2017.
//  Copyright © 2017 Nils Burlat. All rights reserved.
//

#include "DisplaySurface.hpp"
#include "Matrix4x4.hpp"

#include <iostream>
#include <vector>

Matrix4x4 viewingMatrix(DisplaySurface display, Point eye)
{
    Matrix4x4 matrix;
    
    Vector u = Vector(display.u());
    u.normalize();
    Vector v = Vector(display.v());
    v.normalize();
    Vector n = Vector(display.normal());
    
    matrix.m[0][0] = u.dx();
    matrix.m[0][1] = u.dy();
    matrix.m[0][2] = u.dz();
    matrix.m[0][3] = - u.scalar(eye);
    matrix.m[1][0] = v.dx();
    matrix.m[1][1] = v.dy();
    matrix.m[1][2] = v.dz();
    matrix.m[1][3] = - v.scalar(eye);
    matrix.m[2][0] = n.dx();
    matrix.m[2][1] = n.dy();
    matrix.m[2][2] = n.dz();
    matrix.m[2][3] = - n.scalar(eye);
    matrix.m[3][0] = 0;
    matrix.m[3][1] = 0;
    matrix.m[3][2] = 0;
    matrix.m[3][3] = 1;
    
    return matrix;
}

Matrix4x4 projectionMatrix(DisplaySurface display, Point eye, float znear, float zfar)
{
    Matrix4x4 matrix;
    
    float l,r,b,t,n,f;
    n=znear;
    f=zfar;
    
    Vector u = Vector(display.u());
    u.normalize();
    Vector v = Vector(display.v());
    v.normalize();
    Vector normal = Vector(display.normal());
    
    l=-n*(150+u.scalar(eye))/(150+normal.scalar(eye));
    r=n*(150-u.scalar(eye))/(150+normal.scalar(eye));
    t=-n*(150+v.scalar(eye))/(150+normal.scalar(eye));
    b=n*(150-v.scalar(eye))/(150+normal.scalar(eye));
    
    
    printf("%f, %f, %f, %f, %f, %f\n\n", l, r, t, b, n, f);
    
    matrix.m[0][0] = 2*n/(r-l);
    matrix.m[0][1] = 0;
    matrix.m[0][2] = (r+l)/(r-l);
    matrix.m[0][3] = 0;
    matrix.m[1][0] = 0;
    matrix.m[1][1] = 2*n/(b-t);
    matrix.m[1][2] = (t+b)/(b-t);
    matrix.m[1][3] = 0;
    matrix.m[2][0] = 0;
    matrix.m[2][1] = 0;
    matrix.m[2][2] = -(f+n)/(f-n);
    matrix.m[2][3] = - 2*f*n/(f-n);
    matrix.m[3][0] = 0;
    matrix.m[3][1] = 0;
    matrix.m[3][2] = -1;
    matrix.m[3][3] = 0;
    
    
    return matrix;
}

int main(int argc, const char * argv[]) {
    
    DisplaySurface front(Point(-150, -150, -150), Vector(300, 0, 0), Vector(0, 300, 0));
    DisplaySurface left(Point(-150, -150, 150), Vector(0, 0, -300), Vector(0, 300, 0));
    DisplaySurface right(Point(150, -150, -150), Vector(0, 0, 300), Vector(0, 300, 0));
    DisplaySurface floor(Point(-150, -150, 150), Vector(300, 0, 0), Vector(0, 0, -300));

    
    Matrix4x4 mFront = viewingMatrix(front, Point(50, 20, 100));
    printf("viewingMatrix(front, Point(50, 20, 100) = \n");
    mFront.print();
    
    Matrix4x4 mLeft = viewingMatrix(left, Point(50, 20, 100));
    printf("viewingMatrix(left, Point(50, 20, 100) = \n");
    mLeft.print();
    
    Matrix4x4 mRight = viewingMatrix(right, Point(50, 20, 100));
    printf("viewingMatrix(right, Point(50, 20, 100) = \n");
    mRight.print();
    
    Matrix4x4 mFloor = viewingMatrix(floor, Point(50, 20, 100));
    printf("viewingMatrix(floor, Point(50, 20, 100) = \n");
    mFloor.print();
    
    printf("projectionMatrix(front, Point(50, 20, 100), znear=0.1, zfar=100 = \n");
    Matrix4x4 pFront = projectionMatrix(front, Point(50, 20, 100),0.1,100);
    pFront.print();
    
    printf("projectionMatrix(left, Point(50, 20, 100), znear=0.1, zfar=100 = \n");
    Matrix4x4 pLeft = projectionMatrix(left, Point(50, 20, 100),0.1,100);
    pLeft.print();
    
    printf("projectionMatrix(right, Point(50, 20, 100), znear=0.1, zfar=100 = \n");
    Matrix4x4 pRight = projectionMatrix(right, Point(50, 20, 100),0.1,100);
    pRight.print();
    
    printf("projectionMatrix(floor, Point(50, 20, 100), znear=0.1, zfar=100 = \n");
    Matrix4x4 pFloor = projectionMatrix(floor, Point(50, 20, 100),0.1,100);
    pFloor.print();
    
    
    return 0;
}
