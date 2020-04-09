#include "Loop.hh"

#include <cmath>
#include <iostream>

#include "Configurations.hh"
using namespace std;

Loop::Loop(Vec2d& p, uint32_t size = 10) : completed(false)
{
    head.reserve(size);
    head.push_back(p);
    minX = p.x, maxX = p.x;
    minY = p.y, maxY = p.y;
}

bool Loop::isCompleted() const { return completed; }
void Loop::add(double x, double y) { add(new Vec2d(x, y)); }
void Loop::add(Vec2d* p) { add(*p); }
void Loop::add(Vec2d& p)
{
    if (head.front().equal(p))
    {  // Loop complete
        completed = true;
        midX      = (minX + maxX) / 2;
        midY      = (minY + maxY) / 2;
        return;
    }
    if (minX > p.x) minX = p.x;
    if (maxX < p.x) maxX = p.x;
    if (minY > p.y) minY = p.y;
    if (maxY < p.y) maxY = p.y;
    head.push_back(p);
}
int Loop::size() const { return head.size(); }
const Vec2d& Loop::at(int i) const { return head.at(i % head.size()); }
const Vec2d& Loop::front() const { return head.front(); }
const Vec2d& Loop::back() const { return head.back(); }
Vec2d& Loop::at(int i) { return head.at(i % head.size()); }
Vec2d& Loop::front() { return head.front(); }
Vec2d& Loop::back() { return head.back(); }
double Loop::getminX() const { return minX; }
double Loop::getmaxX() const { return maxX; }
double Loop::getminY() const { return minY; }
double Loop::getmaxY() const { return maxY; }
double Loop::getXsum() const { return xsum; }
double Loop::getYsum() const { return ysum; }
void Loop::loopSum()
{
    for (const auto& v : head)
    {
        xsum += v.x;
        ysum += v.y;
    }
}
void Loop::optimize()
{
    if (head.size() < 4) return;
    Vec2d p1(head.back());  // position at -1, 0, 1
    Vec2d p2(head.front());
    Vec2d p3(head.at(1));
    if (fabs((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y)) <
        thr)
        head.erase(head.begin());

    for (int i = 0; i < head.size() - 2; i++)
    {  // position form 0, 1, 2   to   end-2, end-1, end
        p1 = head.at(i);
        p2 = head.at(i + 1);
        p3 = head.at(i + 2);
        if (fabs((p2.x - p1.x) * (p3.y - p1.y) -
                 (p3.x - p1.x) * (p2.y - p1.y)) < thr)
        {  // in same line
            head.erase(head.begin() + (i + 1));
            i--;
        }
    }

    p1 = head.at(head.size() - 2);  // position at -2, -1, 0
    p2 = head.back();
    p3 = head.front();
    if (fabs((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y)) <
        thr)
        head.pop_back();

    for (auto& lp : subLoops) lp.optimize();
}
bool Loop::checkSubLoop(Loop& lp)
{
    if (lp.getminX() > minX && lp.getmaxX() < maxX && lp.getminY() > minY &&
        lp.getmaxY() < maxY)
    {
        subLoops.push_back(lp);
        for (auto& l : lp.subLoops)
        {
            subLoops.push_back(l);
        }
        lp.subLoops.clear();
        return true;
    }
    return false;
}
void Loop::print() const
{
    if (head.size() == 0)
    {
        cout << "null\n";
        return;
    }
    cout << head.at(0);
    for (int i = 1; i < head.size(); i++)
    {
        cout << " --> " << head.at(i);
    }
}
