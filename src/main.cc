#include <fstream>
#include <iostream>
#include <string>

#include "Slicer.hh"
using namespace std;

int main(int argc, char* argv[])
{
    const char* filename = argc < 2 ? "cube.stl" : argv[1];
    Slicer slicer(filename);  // eliipticalvase.stl
    slicer.slicing();

    return 0;
}
