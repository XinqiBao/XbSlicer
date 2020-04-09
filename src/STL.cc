#include "STL.hh"

#include <fstream>

#include "BufferReadBinary.hh"
#include "Configurations.hh"
using namespace std;

Stl::Stl() {}  // Have to call the function read() later
Stl::Stl(std::string& stl_target, bool isBinary)
{
    // TODO: if the stl file does not exist
    read(stl_target, isBinary);
}

double Stl::getZmax() const { return zmax; }
double Stl::getZmin() const { return zmin; }

int Stl::read(string& stl_target, bool isBinary)
{
    // TODO: read for ASCII and binary
    int numTriangles;
    if (isBinary)
        numTriangles = readBinary(stl_target);
    else
        numTriangles = readASCII(stl_target);
    setMinAndMax();
    if (needMoveToCenter) moveToCenter();
    assignTriangles();
    return numTriangles;
}

// see stl format for ASCII file
int Stl::readASCII(string& fileTarget)
{
    ifstream f(fileTarget);
    streampos pos = f.tellg();
    f.seekg(0, ios_base::end);
    int len = f.tellg();
    f.seekg(pos);
    triangles.reserve(len / 150);
    /*char s[256];
    while (f >> s) {
            if (!strcmp(s,"facet")) {
                    triangles.push_back(Triangle(f));
            }
    }*/		//old version for read stl
    f.ignore(256, '\n');
    while (f.peek() != 'e') triangles.push_back(Triangle(f));
    f.close();
    return triangles.size();
}

// see stl format for binary file
// will much faster than read in by ASCII file
// and using BufferReadBinary.o to reduce the I/O time
int Stl::readBinary(string& fileTarget)
{
    BufferReadBinary buffer(fileTarget);
    // a number in the stl file which means total number for triangles
    uint32_t numTrianglesInFile = buffer.getNumTrianglesInFile();
    triangles.reserve(numTrianglesInFile);
    while (numTrianglesInFile)
    {
        int numTrianglesInBlock = buffer.getNumTrianglesInBlock();
        numTrianglesInFile -= numTrianglesInBlock;
        while (numTrianglesInBlock)
        {
            triangles.push_back(Triangle(buffer));
            numTrianglesInBlock--;
        }
        buffer.readBuffer();
    }
    return triangles.size();
}

// By using more memory to speed up the program
// After assign the triangles, no need to check triangles which is out of the
// high range
void Stl::assignTriangles()
{
    double invDz = 1.0 / dzEachLevel;
    numLevels    = (zmax - zmin) * invDz + 1;
    levels.reserve(numLevels);
    int meanTrianglesPerLevel = triangles.size() / numLevels * 1.5;
    for (int i = 0; i < numLevels; i++)
    {
        levels.push_back(vector<Triangle>());
        levels.back().reserve(meanTrianglesPerLevel);
    }
    for (const auto& t : triangles)
    {
        double zmint, zmaxt;
        if (t.v1.z < t.v2.z)
        {
            zmint = t.v1.z;
            zmaxt = t.v2.z;
        }
        else
        {
            zmaxt = t.v1.z;
            zmint = t.v2.z;
        }
        zmint = zmint < t.v3.z ? zmint : t.v3.z;
        zmaxt = zmaxt > t.v3.z ? zmaxt : t.v3.z;
        for (int slice = int((zmint - zmin) * invDz);
             slice <= int((zmaxt - zmin) * invDz); slice++)
            levels[slice].push_back(t);
    }
    triangles.clear();
    // triangles.~vector();
}

// set the minimum and maximum of x, y and z, can be used by latter optimization
void Stl::setMinAndMax()
{
    if (triangles.empty()) return;
    xmin = xmax = triangles.front().v1.x;
    ymin = ymax = triangles.front().v1.y;
    zmin = zmax = triangles.front().v1.z;

    for (const auto& t : triangles)
    {
        double xmint = t.v1.x, xmaxt = t.v1.x;
        double ymint = t.v1.y, ymaxt = t.v1.y;
        double zmint = t.v1.z, zmaxt = t.v1.z;

        if (t.v2.x < xmint)
            xmint = t.v2.x;
        else
            xmaxt = t.v2.x;
        if (t.v3.x < xmint)
            xmint = t.v3.x;
        else if (t.v3.x > xmaxt)
            xmaxt = t.v3.x;

        if (t.v2.y < ymint)
            ymint = t.v2.y;
        else
            ymaxt = t.v2.y;
        if (t.v3.y < ymint)
            ymint = t.v3.y;
        else if (t.v3.y > ymaxt)
            ymaxt = t.v3.y;

        if (t.v2.z < zmint)
            zmint = t.v2.z;
        else
            zmaxt = t.v2.z;
        if (t.v3.z < zmint)
            zmint = t.v3.z;
        else if (t.v3.z > zmaxt)
            zmaxt = t.v3.z;

        if (xmint < xmin) xmin = xmint;
        if (xmaxt > xmax) xmax = xmaxt;
        if (ymint < ymin) ymin = ymint;
        if (ymaxt > ymax) ymax = ymaxt;
        if (zmint < zmin) zmin = zmint;
        if (zmaxt > zmax) zmax = zmaxt;
    }
}

// Sometime the model does not locate at a great position
// By the setted center point of printer table, move the model to a nice place
void Stl::moveToCenter()
{
    double mvX = centerX - (xmax + xmin) / 2;
    double mvY = centerY - (ymax + ymin) / 2;
    double mvZ = centerZ - zmin;

    for (auto& t : triangles)
    {
        t.v1.x += mvX;
        t.v1.y += mvY;
        t.v1.z += mvZ;

        t.v2.x += mvX;
        t.v2.y += mvY;
        t.v2.z += mvZ;

        t.v3.x += mvX;
        t.v3.y += mvY;
        t.v3.z += mvZ;
    }

    xmin += mvX;
    xmax += mvX;
    ymin += mvY;
    ymax += mvY;
    zmin += mvZ;
    zmax += mvZ;
}

vector<Triangle>& Stl::getLevel(double z)
{
    return levels.at(int(z - zmin) / dzEachLevel);
}

void Stl::printSTL()
{  // need to comment "triangles.clear();" in assignTriangles function
    cout << "number of triangles: " << triangles.size() << '\n';
    for (int i = 0; i < triangles.size(); i++)
        cout << "# " << i << " :\n" << triangles.at(i) << '\n';
}
