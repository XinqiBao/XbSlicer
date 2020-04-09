#include "Vec3d.hh"

#include <cmath>

#include "Configurations.hh"

Vec3d::Vec3d() {}
Vec3d::Vec3d(double x, double y, double z) : x(x), y(y), z(z) {}
void Vec3d::readBuffer(BufferReadBinary& buffer)
{
    x = buffer.getFloat();
    buffer.stepFloat();
    y = buffer.getFloat();
    buffer.stepFloat();
    z = buffer.getFloat();
    buffer.stepFloat();
}

bool Vec3d::equal(const Vec3d& v) const
{
    if (fabs(x - v.x) < thr && fabs(y - v.y) < thr && fabs(z - v.z) < thr)
        return true;
    return false;
}
bool Vec3d::equal(const Vec3d* v) const { return equal(*v); }
double Vec3d::sum() const { return x + y + z; }

std::istream& operator>>(std::istream& s, Vec3d& v)
{
    return s >> v.x >> v.y >> v.z;
}
std::istream& operator>>(std::istream& s, Vec3d* v)
{
    return s >> v->x >> v->y >> v->z;
}
std::ostream& operator<<(std::ostream& s, const Vec3d& v)
{
    return s << v.x << "," << v.y << "," << v.z;
}
std::ostream& operator<<(std::ostream& s, const Vec3d* v)
{
    return s << v->x << "," << v->y << "," << v->z;
}
