#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <queue>
#include <set>

using namespace std;

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
    double cury;
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

ostream& operator<<(ostream& os, const Line& a) {
    os << '{' << a.left << ',' << a.right << '}';
    return os;
}

istream& operator>>(istream& is, Line& a) {
    is >> a.left >> a.right;
    return is;
}

struct comp {
    bool operator()(const pair<Point, int>& a, const pair<Point, int>& b) const { return a.first.x > b.first.x; };
};

struct sweepcomp {
    bool operator()(const Line& a, const Line& b) const { return a.cury > b.cury; };
};

void logInput(
    ostream& ofs, const vector<Line>& lines, priority_queue<pair<Point, int>, vector<pair<Point, int>>, comp> events) {
    ofs << lines.size() << endl;
    for (int i = 0; i < lines.size(); i++) { ofs << lines[i] << ' ' << i << endl; }
    while (!events.empty()) {
        ofs << events.top().first << ' ' << events.top().second << endl;
        events.pop();
    }
}

void logStatus(ostream& ofs, const vector<Line>& lines, set<Line, sweepcomp> sweepStatus) {
    ofs << lines.size() << endl;
    for (int i = 0; i < lines.size(); i++) { ofs << lines[i].left << ' ' << i << endl; }
    for (auto& i: sweepStatus) { ofs << i << ' ' << i.cury << endl; }
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

    cout << m1 << ' ' << m2 << ' ' << c1 << ' ' << c2 << endl;

    x = (c2 - c1) / (m1 - m2);
    y = (c2 * m1 - c1 * m2) / (m1 - m2);

    return Point(x, y, PTYPE::intersection);
}

vector<Line> readLines(istream& instream) {
    int n;
    instream >> n;
    vector<Line> ans(n);

    for (int i = 0; i < n; i++) {
        Point a, b;
        instream >> a >> b;
        if (a.x < b.x || (a.x == b.x && a.y < b.y)) {
            ans[i].left = a;
            ans[i].right = b;
        } else {
            ans[i].left = b;
            ans[i].right = a;
        };
        ans[i].left.type = PTYPE::left;
        ans[i].right.type = PTYPE::right;
    }

    return ans;
}

int main() {
    ifstream ifs("linedata.txt");
    ofstream testLog("log.txt");
    ofstream ofs("out.txt");
    vector<Line> lines = readLines(ifs);
    int n = lines.size();

    priority_queue<pair<Point, int>, vector<pair<Point, int>>, comp> events;
    for (int i = 0; i < n; i++) {
        events.push(make_pair(lines[i].left, i));
        events.push(make_pair(lines[i].right, i));
    }

    logInput(testLog, lines, events);

    set<Line, sweepcomp> sweepStatus;

    while (!events.empty()) {
        Line& cur = lines[events.top().second];
        if (events.top().first.type == PTYPE::left) {
            cur.cury = cur.left.y;
            auto it = sweepStatus.insert(cur);

        } else if (events.top().first.type == PTYPE::right) {
            auto it = sweepStatus.find(cur);

            sweepStatus.erase(it);
        } else if (events.top().first.type == PTYPE::intersection) {
            // the intersection is always for the top line, so it is exchanged with line below
            // this below line will be the successor

            auto succ = sweepStatus.find(cur);
            auto it = succ;
            succ++;
        }
        events.pop();
    }
    if (checkIntersection(lines[0], lines[1])) {
        Point trial = intersect(lines[0], lines[1]);
        cout << lines[0] << ' ' << lines[1] << endl;
        cout << trial << endl;
    }

    logStatus(ofs, lines, sweepStatus);
}
