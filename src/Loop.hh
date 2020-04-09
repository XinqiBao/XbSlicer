#pragma once

/*
        @author: Xinqi Bao
*/

#include <vector>

#include "Vec2d.hh"

class Loop
{
private:
    bool completed;
    double minX, maxX, minY, maxY;
    double xsum = 0, ysum = 0;

public:
    std::vector<Vec2d> head;
    std::vector<Loop> subLoops;
    double midX = 0, midY = 0;
    bool visited = false;

    Loop(Vec2d& p, uint32_t size);
    bool isCompleted() const;
    void add(double x, double y);
    void add(Vec2d* p);
    void add(Vec2d& p);
    int size() const;
    const Vec2d& at(int i) const;
    const Vec2d& front() const;
    const Vec2d& back() const;
    Vec2d& at(int i);
    Vec2d& front();
    Vec2d& back();
    double getminX() const;
    double getmaxX() const;
    double getminY() const;
    double getmaxY() const;
    double getXsum() const;
    double getYsum() const;
    void loopSum();
    void optimize();
    bool checkSubLoop(Loop& lp);
    void print() const;
};
