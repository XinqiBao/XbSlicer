#pragma once

/*
        @author: Xinqi Bao
        2-D vector, with 2 double vaule points.
*/

#include <iostream>

#include "Vec3d.hh"

class Vec2d
{
public:
    double x, y;
    bool equal(const Vec2d& v) const;
    bool equal(const Vec2d* v) const;
    double sum() const;

    Vec2d();
    Vec2d(double x, double y);
    Vec2d(const Vec3d& v1, const Vec3d& v2, double z);

    friend std::istream& operator>>(std::istream& s, Vec2d& v);
    friend std::istream& operator>>(std::istream& s, Vec2d* v);
    friend std::ostream& operator<<(std::ostream& s, const Vec2d& v);
    friend std::ostream& operator<<(std::ostream& s, const Vec2d* v);
};
