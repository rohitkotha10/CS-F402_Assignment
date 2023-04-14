#pragma once
#include <iostream>
#include <fstream>
#include <utility>

using namespace std;

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
    Line(Point a, Point b) : left {a}, right {b} {};
    Point left, right;
    double evaly(double val) const;
};

struct InterPoint: Point {
    InterPoint() : Point() {};
    InterPoint(double x, double y) : Point(x, y, PTYPE::intersection) {};
};

double Line::evaly(double val) const {
    double ans;
    double m1, c1;
    m1 = (right.y - left.y) / (right.x - left.x);
    c1 = right.y - m1 * (right.x);
    return m1 * val + c1;
}

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

ostream& operator<<(ostream& os, const Line& a) {
    os << '{' << a.left << ',' << a.right << '}';
    return os;
}

istream& operator>>(istream& is, Line& cur) {
    Point a, b;
    is >> a >> b;
    if (a.x < b.x || (a.x == b.x && a.y < b.y)) {
        cur.left = a;
        cur.right = b;
    } else {
        cur.left = b;
        cur.right = a;
    };
    cur.left.type = PTYPE::left;
    cur.right.type = PTYPE::right;

    return is;
}

bool operator<(const Line& a, const Line& b) {
    return a.evaly(sweep_x) < b.evaly(sweep_x);
}

bool operator==(const Line& a, const Line& b) {
    return a.evaly(sweep_x) == b.evaly(sweep_x);
}

bool operator>(const Line& a, const Line& b) {
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

bool checkIntersection(const Line& a, const Line& b) {
    if (orientation(a.left, b.left, a.right) == orientation(a.left, b.right, a.right)) return false;
    if (orientation(b.left, a.left, b.right) == orientation(b.left, a.right, b.right)) return false;
    return true;
}

Point intersect(const Line& a, const Line& b) {
    double x, y;
    double m1, m2, c1, c2;
    m1 = (a.right.y - a.left.y) / (a.right.x - a.left.x);
    m2 = (b.right.y - b.left.y) / (b.right.x - b.left.x);
    c1 = a.right.y - m1 * (a.right.x);
    c2 = b.right.y - m2 * (b.right.x);

    x = (c2 - c1) / (m1 - m2);
    y = (c2 * m1 - c1 * m2) / (m1 - m2);

    Point temp = Point(x, y, PTYPE::intersection);
    return temp;
}