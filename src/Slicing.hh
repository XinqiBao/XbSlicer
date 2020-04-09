#pragma once

/*
        @author: Xinqi Bao
        With a particular z, get lines from triangles at this position.
        Then make loops from the pool of intersections, store the result into
   layer object.
*/

#include <vector>

#include "Cross.hh"
#include "GCode.hh"
#include "Loop.hh"
#include "STL.hh"
#include "Triangle.hh"

class Slicing
{
private:
    double z;
    std::vector<Loop>& loops;
    std::vector<Triangle>& triangles;
    std::vector<Cross> intersections;
    bool isCross(const Vec3d& v_1, const Vec3d& v_2) const;
    bool isSectionExist(Cross& cr) const;
    void intersection();
    void mkLoop();

public:
    Slicing(Stl* stl, Layer* layer);
    Slicing(Stl& stl, Layer& layer);

    void printVector() const;
    void printLoop() const;
};
