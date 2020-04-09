#include "Slicer.hh"

#include <fstream>

#include "BufferWrite.hh"
#include "Slicing.hh"
using namespace std;

void Slicer::slicing()
{
    double zmax = stl.getZmax();
    double zmin = stl.getZmin();
    numLayer    = (zmax - zmin - zStart) / zgap + 1;
    layers.reserve(numLayer);
    checkLayers = new bool[numLayer];
    for (int i = 0; i < numLayer; i++) checkLayers[i] = false;
    for (double z = zmin + zStart; z < zmax; z += zgap)
    {
        layers.push_back(Layer(z));
    }
    cout << finishedCount << "/" << numLayer;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads[i] = thread(&Slicer::threadLayer, this);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads[i].join();
    }
    cout << "\nWriting...";
#if 0  // Write to gcode file directly
	ofstream f(gcode_target, ios_base::out | ios_base::app);
	for (const auto& layer : layers)
		layer.commandsOut(f);
#endif
#if 1  // Write to writing buffer
    BufferWrite buffer(gcode_target);
    for (const auto& layer : layers) layer.commandsOut(buffer);
    buffer.flush();
#endif
    cout << "\nDone!\n";
}

// mult-threads management function
void Slicer::threadLayer()
{
    for (int index = 0; index < numLayer; index++)
    {
        checkMutex.lock();
        if (checkLayers[index])
        {
            checkMutex.unlock();
            continue;
        }
        checkLayers[index] = true;
        checkMutex.unlock();

        Slicing cl(stl, layers[index]);

        countMutex.lock();
        finishedCount++;
        cout << "\r" << finishedCount << "/" << numLayer;
        countMutex.unlock();
    }
}

Slicer::Slicer(std::string& filename) : Slicer(filename.c_str()) {}

Slicer::Slicer(const char* filename)
{
#if 0  // Make directory for Windows
		_mkdir(gcode_filePath.c_str());	//make directory for storing gcode
#endif
#if 1
    std::string mkdir = "mkdir -p ";
    int res           = system(mkdir.append(gcode_filePath.c_str()).c_str());
#endif

    // TODO:	STL file extensions: .stl, .stlb, .bif.
    // This can work for .stl and .stlb, but not good.
    int extensionLen = 3;
    bool isBinary;
    if (filename[strlen(filename) - 1] == 'b')
    {
        isBinary = true;
        extensionLen++;
    }
    else
        isBinary = false;

    // change the file extension from ".stl" to ".gcode"
    stl_target = stl_filePath;
    stl_target.append(filename);
    gcode_target = gcode_filePath;
    gcode_target.append(filename, strlen(filename) - extensionLen);
    gcode_target.append("gcode");
    remove(gcode_target.c_str());  // remove old gcode file
    std::cout << "\tFile: " << filename << "\nReading...";
    std::cout << "\nNumber of triangles: " << stl.read(stl_target, isBinary);
    std::cout << "\nRead finish\n";
}
