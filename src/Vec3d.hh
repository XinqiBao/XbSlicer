#pragma once

/*
        @author: Xinqi Bao
        3-D vector, with 3 double vaule points.
*/

#include <iostream>

#include "BufferReadBinary.hh"

class Vec3d
{
public:
    double x, y, z;
    void readBuffer(BufferReadBinary& buffer);
    bool equal(const Vec3d& v) const;
    bool equal(const Vec3d* v) const;
    double sum() const;

    Vec3d();
    Vec3d(double x, double y, double z);

    friend std::istream& operator>>(std::istream& s, Vec3d& v);
    friend std::istream& operator>>(std::istream& s, Vec3d* v);
    friend std::ostream& operator<<(std::ostream& s, const Vec3d& v);
    friend std::ostream& operator<<(std::ostream& s, const Vec3d* v);
};
