#include "Vec2d.hh"

#include <cmath>

#include "Configurations.hh"

Vec2d::Vec2d() {}
Vec2d::Vec2d(double x, double y) : x(x), y(y) {}
Vec2d::Vec2d(const Vec3d& v1, const Vec3d& v2, double z)
{
    x = (v1.x - v2.x) * (z - v1.z) / (v1.z - v2.z) + v1.x;
    y = (v1.y - v2.y) * (z - v1.z) / (v1.z - v2.z) + v1.y;
}

bool Vec2d::equal(const Vec2d& v) const
{
    if (fabs(x - v.x) < thr && fabs(y - v.y) < thr) return true;
    return false;
}
bool Vec2d::equal(const Vec2d* v) const { return equal(*v); }
double Vec2d::sum() const { return x + y; }

std::istream& operator>>(std::istream& s, Vec2d& v) { return s >> v.x >> v.y; }
std::istream& operator>>(std::istream& s, Vec2d* v)
{
    return s >> v->x >> v->y;
}
std::ostream& operator<<(std::ostream& s, const Vec2d& v)
{
    return s << v.x << "," << v.y;
}
std::ostream& operator<<(std::ostream& s, const Vec2d* v)
{
    return s << v->x << "," << v->y;
}
