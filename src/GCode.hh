#pragma once

/*
        @author: Xinqi Bao
        After having loops for one layer, can use this class to generate the
   gcode
*/

#include <vector>

#include "Cross.hh"
#include "Layer.hh"
#include "Loop.hh"

static double dp = 1;

class Gcode
{
private:
    int numParts;
    double &minX, &maxX;
    std::vector<Loop>& loops;
    std::vector<Command>&
        commands;  // contain each line of gcode, stored to layer
    std::vector<std::vector<Cross>>& parts;
    void G0(double x, double y, double z);
    void G0(double x, double y);
    void G0_high(double z);
    void G1(double x, double y);
    void resetE();
    void outline();           // method before implement subLoops
    void fill();              // method before implement subLoops
    void doLoops();           //***method using implement subLoops***
    void fillLoop(Loop& lp);  //***method using implement subLoops***
public:
    Gcode(Layer& layer);
};
