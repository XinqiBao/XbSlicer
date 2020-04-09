#include "Cross.hh"

Cross::Cross(Vec2d* p1, Vec2d* p2) : p1(*p1), p2(*p2) {}
Cross::Cross(const Vec2d& p1, const Vec2d& p2) : p1(p1), p2(p2) {}

bool Cross::equal(const Cross* cr) const
{
    return (p1.equal(cr->p1) && p2.equal(cr->p2)) ||
           (p1.equal(cr->p2) && p2.equal(cr->p1));
}
bool Cross::equal(const Cross& cr) const
{
    if ((p1.equal(cr.p1) && p2.equal(cr.p2)) ||
        (p1.equal(cr.p2) && p2.equal(cr.p1)))
        return true;
    return false;
}

std::ostream& operator<<(std::ostream& s, const Cross* cr)
{
    return s << cr->p1 << "  ---  " << cr->p2;
}
std::ostream& operator<<(std::ostream& s, const Cross& cr)
{
    return s << cr.p1 << "  ---  " << cr.p2;
}
