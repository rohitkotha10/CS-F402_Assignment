#pragma once
#include <iostream>
#include <fstream>
#include <utility>
#include <cmath>

using namespace std;

#define PREC 0.000001

extern double sweep_x;

enum class PTYPE { left, right, intersection };

struct Point {
    Point() {};
    Point(double x, double y) : x {x}, y {y} {};
    Point(double x, double y, PTYPE cat) : x {x}, y {y}, type {cat} {};

    double x, y;
    PTYPE type;
};

struct Line {
    Line() {};
    Line(Point a, Point b) : left {a}, right {b} {
        m = (a.y - b.y) / (a.x - b.x);
        c = a.y - m * a.x;
    };

    double evaly(double val) const {
        return m * val + c;
    };

    Point left, right;
    double m, c;  // y = mx + c;
};

ostream& operator<<(ostream& os, const Point& a) {
    os << '(' << a.x << ',' << a.y << ')';
    return os;
}

istream& operator>>(istream& is, Point& a) {
    is >> a.x >> a.y;
    return is;
}

Point operator+(const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y);
}

Point operator-(const Point& a, const Point& b) {
    return Point(a.x - b.x, a.y - b.y);
}

bool operator<(const Point& a, const Point& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

bool operator>(const Point& a, const Point& b) {
    return b < a;
}

double cross(const Point& a, const Point& b) {
    return a.x * b.y - b.x * a.y;
}

// 1 anticlockwise, 0 collinear, -1 clockwise
int orientation(const Point& a, const Point& b, const Point& c) {
    double cr = cross(b - a, c - b);
    if (cr > 0)
        return 1;
    else if (cr == 0)
        return 0;
    else
        return -1;
}

ostream& operator<<(ostream& os, const Line& a) {
    os << '{' << a.left << ',' << a.right << '}';
    return os;
}

istream& operator>>(istream& is, Line& cur) {
    Point a, b;
    is >> a >> b;
    if (a.x < b.x || (a.x == b.x && a.y < b.y)) {
        cur = Line(a, b);
    } else {
        cur = Line(b, a);
    };
    cur.left.type = PTYPE::left;
    cur.right.type = PTYPE::right;

    return is;
}

bool operator<(const Line& a, const Line& b) {
    return fabs(a.evaly(sweep_x) - b.evaly(sweep_x)) > PREC && a.evaly(sweep_x) < b.evaly(sweep_x) ||
           (fabs(a.evaly(sweep_x) - b.evaly(sweep_x)) <= PREC && orientation(a.left, b.left, a.right) < 0);
}

bool operator==(const Line& a, const Line& b) {
    return a.left == b.left && a.right == b.right;
}

bool operator>(const Line& a, const Line& b) {
    return b < a;
}

bool checkIntersection(const Line& a, const Line& b) {
    if (orientation(a.left, b.left, a.right) == orientation(a.left, b.right, a.right)) return false;
    if (orientation(b.left, a.left, b.right) == orientation(b.left, a.right, b.right)) return false;
    return true;
}

Point intersect(const Line& a, const Line& b) {
    double x, y;

    x = (b.c - a.c) / (a.m - b.m);
    y = (b.c * a.m - a.c * b.m) / (a.m - b.m);

    Point temp = Point(x, y, PTYPE::intersection);
    return temp;
}