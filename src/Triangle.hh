#pragma once

/*
        @author: Xinqi Bao
        Triangle with 3 3-D vectors.
*/

#include <iostream>

#include "BufferReadBinary.hh"
#include "Vec3d.hh"

class Skip
{
public:
    friend std::istream& operator>>(std::istream& s, Skip& skip);
};

class Triangle
{
public:
    Vec3d normal, v1, v2, v3;

    Triangle();
    Triangle(std::istream& s);
    Triangle(BufferReadBinary& buffer);
    Triangle(Vec3d& normal, Vec3d& v1, Vec3d& v2, Vec3d& v3);

    friend std::ostream& operator<<(std::ostream& s, Triangle& tri);
};
