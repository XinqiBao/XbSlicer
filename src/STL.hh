#pragma once

/*
        @author: Xinqi Bao
        For reading in stl file, both ASCII and binary.
        In stl file, the model represented as many triangles.
        Store every triangles and assign to several groups with a constant high,
        then send to Slicing.o to slice to defferent layers.
*/

#include <unistd.h>

#include <string>
#include <vector>

#include "Triangle.hh"

class Stl
{
private:
    int numLevels;
    double xmin, xmax, ymin, ymax, zmin, zmax;
    int readASCII(std::string& fileTarget);
    int readBinary(std::string& fileTarget);
    void assignTriangles();
    void setMinAndMax();
    void moveToCenter();

public:
    std::vector<Triangle> triangles;  // contain all the triangles
    std::vector<std::vector<Triangle>>
        levels;  // each element is a group of assigned triangles by a range of
                 // high
    double getZmax() const;
    double getZmin() const;
    std::vector<Triangle>& getLevel(double z);
    int read(std::string& stl_target, bool isBinary);
    Stl();
    Stl(std::string& stl_target, bool isBinary);

    void printSTL();
};
