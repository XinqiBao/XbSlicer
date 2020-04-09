#include "GCode.hh"

#include <algorithm>
#include <cmath>

#include "Configurations.hh"
using namespace std;

Gcode::Gcode(Layer& layer)
    : loops(layer.getLoops()),
      commands(layer.getCommands()),
      parts(layer.getParts()),
      minX(layer.getMinX()),
      maxX(layer.getMaxX())
{
    if (loops.empty()) return;
    int reserveSize = 0;
    for (const auto& lp : loops)
        reserveSize += lp.size() + int((lp.getmaxX() - lp.getminX()) / dp) * 2;
    commands.reserve(reserveSize);
    G0_high(layer.getZ());
    // outline();
    // fill();
    doLoops();
    layer.generateDe();
}

void Gcode::G0(double x, double y, double z)
{
    commands.push_back(Command(0, x, y));
    // f << "G0 F6600 X" << x << " Y" << y << " Z" << z << '\n';
    // f.flush();
}
void Gcode::G0(double x, double y)
{
    commands.push_back(Command(0, x, y));
    // f << "G0 F6600 X" << x << " Y" << y << '\n';
    // f.flush();
}
void Gcode::G0_high(double z)
{
    // f << "G0 F6600  Z" << z << '\n';
    // f.flush();
}
void Gcode::G1(double x, double y)
{
    commands.push_back(Command(1, x, y));
    // f << "G1 F783 X" << x << " Y" << y << " E" << e << '\n';
    // f.flush();
}
void Gcode::resetE()
{  // Reset E to 0
    // f << "G92 E0 ; reset the expected extruder position\n";
    // f.flush();
}

static double x = 0, y = 0;

void Gcode::doLoops()
{
#if 0  // by erasing the finished loop
	while(!loops.empty()){
		double minDistance = pow(loops.front().midX - x, 2) + pow(loops.front().midY - y, 2);
		int k = 0;
		double distance;
		for(int i = 1; i < loops.size(); i++){
			distance = pow(loops[i].midX - x, 2) + pow(loops[i].midY - y, 2);
			if(distance < minDistance){
				minDistance = distance;
				k = i;
			}
		}
		fillLoop(loops[k]);
		x = commands.back().x;
		y = commands.back().y;
		loops.erase(loops.begin() + k);
	}
#endif
#if 1  // by checking the visited value
    int numLoop = loops.size();
    while (numLoop)
    {
        int i = 0, k = 0;
        double minDistance, distance;
        while (loops[i].visited) i++;
        minDistance = pow(loops[i].midX - x, 2) + pow(loops[i].midY - y, 2);
        k           = i;
        for (i++; i < loops.size(); i++)
        {
            if (loops[i].visited) continue;
            distance = pow(loops[i].midX - x, 2) + pow(loops[i].midY - y, 2);
            if (distance < minDistance)
            {
                minDistance = distance;
                k           = i;
            }
        }
        fillLoop(loops[k]);
        loops[k].visited = true;  // mark as filled loop
        x                = commands.back().x;
        y                = commands.back().y;
        numLoop--;
    }
#endif
}

void Gcode::fillLoop(Loop& loop)
{
    vector<Loop> ploops;
    ploops.push_back(loop);
    int numIntersectionsTotal = loop.size();
    for (auto& plp : loop.subLoops)
    {
        ploops.push_back(plp);
        numIntersectionsTotal += plp.size();
    }

    // outline
    double x1, y1, x2, y2;

    int numLoop = ploops.size();
    while (numLoop)
    {
        int i = 0, k = 0;
        double minDistance, distance;
        while (ploops[i].visited) i++;
        minDistance = pow(ploops[i].midX - x, 2) + pow(ploops[i].midY - y, 2);
        k           = i;
        for (i++; i < ploops.size(); i++)
        {
            if (ploops[i].visited) continue;
            distance = pow(ploops[i].midX - x, 2) + pow(ploops[i].midY - y, 2);
            if (distance < minDistance)
            {
                minDistance = distance;
                k           = i;
            }
        }
        // start to print this loop's outline
        x1 = ploops[k].back().x;
        y1 = ploops[k].back().y;
        G0(x1, y1);
        for (int i = 0; i < ploops[k].size(); i++)
        {
            x2 = ploops[k].at(i).x;
            y2 = ploops[k].at(i).y;
            G1(x2, y2);
            x1 = x2, y1 = y2;
        }
        ploops[k].visited = true;
        x                 = commands.back().x;
        y                 = commands.back().y;
        numLoop--;
    }
    /*
    for (auto& lp : ploops) {
            x1 = lp.back().x;
            y1 = lp.back().y;
            G0(x1, y1);
            for (int i = 0; i < lp.size(); i++) {
                    x2 = lp.at(i).x;
                    y2 = lp.at(i).y;
                    G1(x2, y2);
                    x1 = x2, y1 = y2;
            }
    }
    */

    // fill
    vector<vector<Cross>> pparts;
    double invDp = 1.0 / dp;
    numParts     = (loop.getmaxX() - loop.getminX()) * dp + 1;
    pparts.reserve(numParts);
    int meanIntersectionsPerPart = numIntersectionsTotal / numParts * 1.5;
    for (int i = 0; i < numParts; i++)
    {
        pparts.push_back(vector<Cross>());
        pparts.back().reserve(meanIntersectionsPerPart);
    }
    for (const auto& lp : ploops)
    {  // assign all intersections to partial
        Vec2d v1, v2(lp.back());
        int index1, index2 = (v2.x - loop.getminX()) * invDp;
        int imin, imax;

        for (auto& l : lp.head)
        {
            v1     = v2;
            index1 = index2;
            v2     = l;
            index2 = (v2.x - loop.getminX()) * invDp;
            if (index1 < index2)
            {
                imin = index1;
                imax = index2;
            }
            else
            {
                imin = index2;
                imax = index1;
            }
            for (int i = imin; i <= imax; i++)
                pparts[i].push_back(Cross(v1, v2));
        }
    }

    vector<double> ys;
    bool isUp = true;
    for (double x = loop.getminX() + ingap; x < loop.getmaxX(); x += ingap)
    {
        ys.clear();
        double y1, y2;
        for (auto& cr : pparts.at((x - loop.getminX()) * invDp))
        {
            double x1 = cr.p1.x, y1 = cr.p1.y;
            double x2 = cr.p2.x, y2 = cr.p2.y;
            if ((x1 <= x && x < x2) || (x2 <= x && x < x1))
                ys.push_back((x - x1) * (y2 - y1) / (x2 - x1) + y1);
        }
        std::sort(ys.begin(), ys.end());
        if (ys.size() == 0) continue;

        if (isUp)
        {
            y1 = ys.at(0);
            G0(x, y1);
            bool isE = true;
            for (int i = 1; i < ys.size(); i++)
            {
                y2 = ys.at(i);
                if (isE)
                {
                    G1(x, y2);
                    isE = false;
                }
                else
                {
                    G0(x, y2);
                    y1  = y2;
                    isE = true;
                }
            }
            isUp = false;
        }
        else
        {
            y1 = ys.at(ys.size() - 1);
            G0(x, y1);
            bool isE = true;
            for (int i = ys.size() - 2; i >= 0; i--)
            {
                y2 = ys.at(i);
                if (isE)
                {
                    G1(x, y2);
                    isE = false;
                }
                else
                {
                    G0(x, y2);
                    y1  = y2;
                    isE = true;
                }
            }
            isUp = true;
        }
    }
}

void Gcode::outline()
{
    double x1, y1, x2, y2;
    for (auto& lp : loops)
    {
        x1 = lp.back().x;
        y1 = lp.back().y;
        G0(x1, y1);
        for (int i = 0; i < lp.size(); i++)
        {
            x2 = lp.at(i).x;
            y2 = lp.at(i).y;
            G1(x2, y2);
            x1 = x2, y1 = y2;
        }
    }
}

void Gcode::fill()
{
    double invDp              = 1.0 / dp;
    minX                      = loops.front().getminX();
    maxX                      = loops.front().getmaxX();
    int numIntersectionsTotal = loops.front().size();
    for (int i = 1; i < loops.size(); i++)
    {
        numIntersectionsTotal += loops.at(i).size();
        if (minX > loops.at(i).getminX()) minX = loops.at(i).getminX();
        if (maxX < loops.at(i).getmaxX()) maxX = loops.at(i).getmaxX();
    }
    numParts = (maxX - minX) * dp + 1;
    parts.reserve(numParts);
    int meanIntersectionsPerPart = numIntersectionsTotal / numParts * 1.5;
    for (int i = 0; i < numParts; i++)
    {
        parts.push_back(std::vector<Cross>());
        parts.back().reserve(meanIntersectionsPerPart);
    }
    for (const auto& lp : loops)
    {  // assign all intersections to partial
        Vec2d v1(lp.back());
        Vec2d v2(lp.front());
        int index1 = (v1.x - minX) * invDp;
        int index2 = (v2.x - minX) * invDp;
        int imin, imax;
        if (index1 < index2)
        {
            imin = index1;
            imax = index2;
        }
        else
        {
            imin = index2;
            imax = index1;
        }
        for (int i = imin; i <= imax; i++) parts[i].push_back(Cross(v1, v2));

        for (int i = 1; i < lp.size(); i++)
        {  // from the front to the item before the back
            v1     = v2;
            index1 = index2;
            v2     = lp.at(i);
            index2 = (v2.x - minX) * invDp;
            if (index1 < index2)
            {
                imin = index1;
                imax = index2;
            }
            else
            {
                imin = index2;
                imax = index1;
            }
            for (int i = imin; i <= imax; i++)
                parts[i].push_back(Cross(v1, v2));
        }
    }
    std::vector<double> ys;
    bool isUp = true;
    for (double x = minX + ingap; x < maxX; x += ingap)
    {
        ys.clear();
        double y1, y2;
        for (auto& cr : parts.at((x - minX) * invDp))
        {
            double x1 = cr.p1.x, y1 = cr.p1.y;
            double x2 = cr.p2.x, y2 = cr.p2.y;
            if ((x1 <= x && x < x2) || (x2 <= x && x < x1))
                ys.push_back((x - x1) * (y2 - y1) / (x2 - x1) + y1);
        }
        std::sort(ys.begin(), ys.end());
        if (ys.size() == 0) continue;

        if (isUp)
        {
            y1 = ys.at(0);
            G0(x, y1);
            bool isE = true;
            for (int i = 1; i < ys.size(); i++)
            {
                y2 = ys.at(i);
                if (isE)
                {
                    G1(x, y2);
                    isE = false;
                }
                else
                {
                    G0(x, y2);
                    y1  = y2;
                    isE = true;
                }
            }
            isUp = false;
        }
        else
        {
            y1 = ys.at(ys.size() - 1);
            G0(x, y1);
            bool isE = true;
            for (int i = ys.size() - 2; i >= 0; i--)
            {
                y2 = ys.at(i);
                if (isE)
                {
                    G1(x, y2);
                    isE = false;
                }
                else
                {
                    G0(x, y2);
                    y1  = y2;
                    isE = true;
                }
            }
            isUp = true;
        }
    }
}
