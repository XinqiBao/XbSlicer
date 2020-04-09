
#include "Layer.hh"

#include <cmath>

#include "Configurations.hh"
using namespace std;

double Command::e = 0;
void Command::G0_high(std::ostream& s, double z)
{
    s << "G0 F6600  Z" << z << '\n';
}

void Command::resetE(std::ostream& s)
{
    e = 0;
    s << "G92 E0 ; reset the expected extruder position\n";
}

std::ostream& operator<<(std::ostream& s, const Command& c)
{
    switch (c.cmd)
    {  // TODO: figure out the more efficient way, without switch!!
    case 0:
        return s << "G0 F6600 X" << c.x << " Y" << c.y << '\n';
    case 1:
        Command::e += c.de;
        return s << "G1 F783 X" << c.x << " Y" << c.y << " E" << Command::e
                 << '\n';
    }
}

void Command::G0_high(BufferWrite& buf, double z)
{
    buf << "G0 F6600  Z" << z << '\n';
}

void Command::resetE(BufferWrite& buf)
{
    e = 0;
    buf << "G92 E0 ; reset the expected extruder position\n";
}

BufferWrite& operator<<(BufferWrite& buf, const Command& c)
{
    switch (c.cmd)
    {  // TODO: figure out the more efficient way, without switch!!
    case 0:
        buf.writeG0(c.x, c.y);
        return buf;
    case 1:
        Command::e += c.de;
        buf.writeG1(c.x, c.y, Command::e);
        return buf;
    }
}

Command::Command(uint16_t cmd, double x, double y) : cmd(cmd), x(x), y(y) {}

Layer::Layer(double z) : z(z) {}

double Layer::getZ() const { return z; }

double& Layer::getMinX() { return minX; }

double& Layer::getMaxX() { return maxX; }

vector<Loop>& Layer::getLoops() { return loops; }

vector<Command>& Layer::getCommands() { return commands; }

vector<vector<Cross>>& Layer::getParts() { return parts; }

void Layer::commandsOut(ostream& s) const
{
    Command::G0_high(s, z);
    Command::resetE(s);
    for (const auto& c : commands) s << c;
    s.flush();
}

void Layer::commandsOut(BufferWrite& buf) const
{
    Command::G0_high(buf, z);
    Command::resetE(buf);
    for (const auto& c : commands) buf << c;
    // buf.flush();
}

void Layer::generateDe()
{
    tx = commands.front().x;
    ty = commands.front().y;
    for (auto& tc : commands)
    {
        if (tc.cmd == 1)
            tc.de = sqrt(pow(tc.x - tx, 2) + pow(tc.y - ty, 2)) * ef;
        tx = tc.x;
        ty = tc.y;
    }
}
