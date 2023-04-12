#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <set>

using namespace std;

struct Point {
    Point() : x {0}, y {0} {};
    Point(double x, double y) : x {x}, y {y} {};
    double x, y;
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

struct Line {
    Line() {};
    Line(Point a, Point b) : start {a}, end {b} {};
    Point atParameter(double x) { return (1 - x) * start + x * end; };
    Point start;
    Point end;
};
