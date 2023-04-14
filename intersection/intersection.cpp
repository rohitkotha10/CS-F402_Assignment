#include <iostream>
#include <fstream>
#include <utility>
#include <memory>
#include <vector>
#include <queue>
#include <set>

#define event_queue  priority_queue<pair<Point, shared_ptr<Line>>, vector<pair<Point, shared_ptr<Line>>>, comp>
#define sweep_status set<shared_ptr<Line>, sweepcomp>

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
    bool operator()(const pair<Point, shared_ptr<Line>>& a, const pair<Point, shared_ptr<Line>>& b) const {
        return a.first.x > b.first.x;
    };
};

struct sweepcomp {
    bool operator()(const shared_ptr<Line> a, const shared_ptr<Line> b) const { return a->cury > b->cury; };
};

event_queue readLines(istream& instream) {
    int n;
    instream >> n;

    event_queue events;

    for (int i = 0; i < n; i++) {
        Point a, b;
        instream >> a >> b;
        shared_ptr<Line> temp = make_shared<Line>();
        if (a.x < b.x || (a.x == b.x && a.y < b.y)) {
            temp->left = a;
            temp->right = b;
        } else {
            temp->left = b;
            temp->right = a;
        };
        temp->left.type = PTYPE::left;
        temp->right.type = PTYPE::right;

        events.push(make_pair(temp->left, temp));
        events.push(make_pair(temp->right, temp));
    }

    return events;
}

void logInput(ostream& ofs, const event_queue& events) {
    event_queue cop = events;
    ofs << "Input Log" << endl;
    ofs << cop.size() << endl;
    while (!cop.empty()) {
        ofs << cop.top().first << ' ' << *cop.top().second << endl;
        cop.pop();
    }
}

void logStatus(ostream& ofs, const sweep_status& sweepline) {
    ofs << "Sweep Status Log" << endl;
    ofs << sweepline.size() << endl;
    for (auto i: sweepline) { ofs << *i << ' ' << i->cury << endl; }
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

    return Point(x, y, PTYPE::intersection);
}

void processEvent(shared_ptr<Line> fir, shared_ptr<Line> sec, event_queue& events) {
    // fir is always top of sec int sweep line status
    // so that the intersection association of point is with top line
    double minx = events.top().first.x;
    if (checkIntersection(*fir, *sec)) {
        Point ans = intersect(*fir, *sec);
        if (ans.x > minx) events.push(make_pair(ans, fir));
    }
}

shared_ptr<Line> getPred(shared_ptr<Line> cur, const sweep_status& sweepline) {
    auto it = sweepline.find(cur);
    if (it == sweepline.begin())
        return nullptr;
    else
        return *(--it);
}

shared_ptr<Line> getSucc(shared_ptr<Line> cur, const sweep_status& sweepline) {
    auto it = sweepline.find(cur);
    it++;
    if (it != sweepline.end())
        return *it;
    else
        return nullptr;
}

void processLeftEvents(shared_ptr<Line> cur, const sweep_status& sweepline, event_queue& events) {
    shared_ptr<Line> prev = getPred(cur, sweepline);
    shared_ptr<Line> succ = getSucc(cur, sweepline);
    if (prev != nullptr && succ != nullptr) {
        // cur in the middle
        processEvent(prev, cur, events);
        processEvent(cur, succ, events);
    } else if (prev != nullptr) {
        // is in the bottom
        processEvent(prev, cur, events);
    } else if (succ != nullptr) {
        // is in the top
        processEvent(cur, succ, events);
    }
}

void processRightEvents(shared_ptr<Line> cur, const sweep_status& sweepline, event_queue& events) {
    shared_ptr<Line> prev = getPred(cur, sweepline);
    shared_ptr<Line> succ = getSucc(cur, sweepline);

    if (prev != nullptr && succ != nullptr) {
        // cur in the middle
        processEvent(prev, succ, events);
    }
}

void processInterEvents(shared_ptr<Line> cur, const sweep_status& sweepline, event_queue& events) {
    shared_ptr<Line> top = cur;
    shared_ptr<Line> below = getSucc(top, sweepline);
    shared_ptr<Line> topplus = getPred(top, sweepline);
    shared_ptr<Line> belowminus = getSucc(below, sweepline);

    if (below != nullptr && topplus != nullptr) { processEvent(topplus, below, events); }
    if (top != nullptr && belowminus != nullptr) { processEvent(top, belowminus, events); }
}

int main() {
    ifstream ifs;
    ofstream ofs;

    // event queue to get points stl priority-queue based on heap
    event_queue events;
    ifs.open("testLines/1/input.txt");
    events = readLines(ifs);
    ifs.close();

    ofs.open("out.txt", ios_base::out);
    logInput(ofs, events);
    ofs.close();

    // sweep status in an ordered dictionary or stl set based on balanced tree
    sweep_status sweepline;

    vector<Point> ans;

    while (!events.empty()) {
        shared_ptr<Line> cur = events.top().second;
        if (events.top().first.type == PTYPE::left) {
            cur->cury = cur->left.y;
            sweepline.insert(cur);
            processLeftEvents(cur, sweepline, events);
        } else if (events.top().first.type == PTYPE::right) {
            processRightEvents(cur, sweepline, events);
            auto it = sweepline.find(cur);
            if (events.size() == 6) {
                cout << *sweepline.begin() << endl;
                cout << cur << endl;
                auto it = sweepline.find(cur);
                cout << *it << endl;
                cout << "OK" << endl;
            }
            sweepline.erase(it);
        } else if (events.top().first.type == PTYPE::intersection) {
            // the intersection is always for the top line, so it is exchanged with line below
            // this below line will be the successor
            ans.push_back(events.top().first);
            processInterEvents(cur, sweepline, events);
            shared_ptr<Line> top = cur;
            shared_ptr<Line> bottom = getSucc(top, sweepline);
            swap(bottom->cury, top->cury);
        }
        events.pop();
    }

    cout << ans.size() << endl;
    for (auto i: ans) cout << i << ' ';
    cout << endl;

    ofs.open("out.txt", ios_base::app);
    logStatus(ofs, sweepline);
    ofs.close();
}
