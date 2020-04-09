#include "Configurations.hh"
using namespace std;

string stl_filePath   = "./stl_files/";    // directory for storing stl files
string gcode_filePath = "./gcode_files/";  // directory for storing gcode files

double thr = 0.000000001;        // Threshold
double dzEachLevel = 1;          // high for mm to assign to one level
double centerX           = 100;  // center X
double centerY           = 100;  // center Y
double centerZ           = 0;    // center Z
bool needMoveToCenter    = true;
bool needSupportMaterial = true;

double zStart = 0.3;
double zgap = 0.2;     // gap between each adjacent layers
double ingap = 0.2;    // gap for infill
double ef    = 0.015;  // factor for extrude, e += distance * ef
double emax  = 100;    // When E bigger than 10000, the precision will lose,
                    // terrible things happen
