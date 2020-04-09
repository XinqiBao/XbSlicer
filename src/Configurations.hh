#pragma once

#include <string>

#define NUM_THREADS 4

extern std::string stl_filePath;    // directory for storing stl files
extern std::string gcode_filePath;  // directory for storing gcode files

extern double thr;          // Threshold
extern double dzEachLevel;  // high for mm to assign to one level
extern double centerX;      // center X
extern double centerY;      // center Y
extern double centerZ;      // center Z
extern bool needMoveToCenter;
extern bool needSupportMaterial;

extern double zStart;
extern double zgap;   // gap between each adjacent layers
extern double ingap;  // gap for infill
extern double ef;     // factor for extrude, e += distance * ef
extern double emax;   // When E bigger than 10000, the precision will lose,
                     // terrible things happen
