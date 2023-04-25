#pragma once

#include <iostream>
#include <fstream>
#include <utility>
#include <cmath>

#define PREC 0.000001

extern double sweep_x;

int compare(const double& a, const double& b) {
    if (fabs(a - b) < PREC)
        return 0;
    else if (a < b)
        return -1;
    else
        return 1;
}

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
        if (compare(a.x, b.x) == 0) {
            isVertical = true;
            k = a.x;
        } else {
            isVertical = false;
            m = (a.y - b.y) / (a.x - b.x);
            c = a.y - m * a.x;
        }
    };

    double evaly(double val) const { return m * val + c; };

    Point left, right;
    bool isVertical;
    double k;     // x = k if vertical
    double m, c;  // y = mx + c;
};

std::ostream& operator<<(std::ostream& os, const Point& a) {
    os << '(' << a.x << ',' << a.y << ')';
    return os;
}

std::istream& operator>>(std::istream& is, Point& a) {
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
    return (compare(a.x, b.x) == -1 || (compare(a.x, b.x) == 0 && compare(a.y, b.y) == -1));
}

bool operator==(const Point& a, const Point& b) {
    return compare(a.x, b.x) == 0 && compare(a.x, b.x) == 0;
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
    if (compare(cr, 0) == 1)
        return 1;
    else if (compare(cr, 0) == 0)
        return 0;
    else
        return -1;
}

std::ostream& operator<<(std::ostream& os, const Line& a) {
    os << '{' << a.left << ',' << a.right << '}';
    return os;
}

std::istream& operator>>(std::istream& is, Line& cur) {
    Point a, b;
    is >> a >> b;
    if (compare(a.x, b.x) == -1 || (compare(a.x, b.x) == 0 && compare(a.x, b.x) == -1)) {
        cur = Line(a, b);
    } else {
        cur = Line(b, a);
    }
    cur.left.type = PTYPE::left;
    cur.right.type = PTYPE::right;

    return is;
}

bool operator<(const Line& a, const Line& b) {
    return compare(a.evaly(sweep_x), b.evaly(sweep_x)) == -1 ||
           (compare(a.evaly(sweep_x), b.evaly(sweep_x)) == 0 && orientation(a.left, b.left, a.right) < 0);
}

bool operator==(const Line& a, const Line& b) {
    return a.left == b.left && a.right == b.right;
}

bool operator>(const Line& a, const Line& b) {
    return b < a;
}

// one line is part of another with different endpoints
bool checkSameLine(const Line& a, const Line& b) {
    if (a.isVertical && b.isVertical) { return compare(a.k, b.k) == 0; }
    return (compare(a.m, b.m) == 0 && compare(a.c, b.c) == 0);
}

// must be two different lines and non vertical
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