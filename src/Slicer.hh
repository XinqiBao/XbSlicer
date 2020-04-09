#pragma once

/*
        @author: Xinqi Bao
        Main class to manage the whole program.
        Include reading file, slicing each layer and writting to file,
        using mult-threads.
*/

#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "Configurations.hh"
#include "Layer.hh"
#include "STL.hh"

class Slicer
{
private:
    Stl stl;
    int numLayer;
    int finishedCount = 0;  // count for each finished layers
    bool* checkLayers;
    std::string stl_target;     // original stl file name
    std::string gcode_target;   // the name for outputting gcode file
    std::vector<Layer> layers;  // contain each layers with loops and gcode
    std::thread threads[NUM_THREADS];
    std::mutex checkMutex;
    std::mutex countMutex;
    void threadLayer();

public:
    void slicing();
    Slicer(std::string& filename);
    Slicer(const char* filename);
};
