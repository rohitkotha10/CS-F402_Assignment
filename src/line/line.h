#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <set>

using namespace std;

enum class PTYPE { left, right, intersection, nota };

struct Point {
    Point() {};
    Point(double x, double y) : x {x}, y {y}, type {PTYPE::nota} {};
    Point(double x, double y, PTYPE cat) : x {0}, y {0}, type {cat} {};
    double x, y;
    PTYPE type;
};

struct Line {
    Line() {};
    Line(Point a, Point b) : left {a}, right {b} {};
    Point left, right;
};

Point operator+(const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y);
}

Point operator-(const Point& a, const Point& b) {
    return Point(a.x - b.x, a.y - b.y);
}

Point operator*(double p, const Point& a) {
    return Point(p * a.x, p * a.y);
}

Point operator*(const Point& a, double p) {
    return Point(p * a.x, p * a.y);
}

ostream& operator<<(ostream& os, const Point& a) {
    os << '(' << a.x << ',' << a.y << ')';
    return os;
}

istream& operator>>(istream& is, Point& a) {
    is >> a.x >> a.y;
    return is;
}
