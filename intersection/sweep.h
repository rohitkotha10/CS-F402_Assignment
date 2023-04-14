#pragma once
#include "geometry.h"

#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <set>

using namespace std;

extern double sweep_x;

class event_queue {
public:
    void readLines(istream& ifs);

    void push(pair<Point, Line> cur) { data.insert(cur); };
    void erase(pair<Point, Line> cur);
    pair<Point, Line> top() { return *(data.begin()); };
    void pop() { data.erase(data.begin()); };

    int size() const { return data.size(); };
    bool empty() const { return data.empty(); };

    map<Point, Line> data;
};

class sweep_status {
public:
    void push(Line cur) { data.insert(cur); };
    void erase(Line cur);

    int size() const { return data.size(); };
    bool empty() const { return data.empty(); };

    bool existPred(Line cur) const;
    Line getPred(Line cur) const;
    bool existSucc(Line cur) const;
    Line getSucc(Line cur) const;

    multiset<Line> data;
};

void event_queue::readLines(istream& instream) {
    int n;
    instream >> n;

    for (int i = 0; i < n; i++) {
        Line temp;
        instream >> temp;

        this->push(make_pair(temp.left, temp));
        this->push(make_pair(temp.right, temp));
    }
}

void event_queue::erase(pair<Point, Line> cur) {
    auto it = data.find(cur.first);
    data.erase(it);
}

void sweep_status::erase(Line cur) {
    auto it = data.find(cur);
    data.erase(it);
}

bool sweep_status::existPred(Line cur) const {
    auto it = data.find(cur);
    if (it == data.begin() || it == data.end())
        return false;
    else
        return true;
}

Line sweep_status::getPred(Line cur) const {
    auto it = data.find(cur);
    it--;
    return *it;
}

bool sweep_status::existSucc(Line cur) const {
    auto it = data.find(cur);
    if (it == data.end())
        return false;
    else
        return true;
}

Line sweep_status::getSucc(Line cur) const {
    auto it = data.find(cur);
    it++;
    return *it;
}

ostream& operator<<(ostream& ofs, const event_queue& events) {
    ofs << events.size() << endl;
    for (auto i: events.data) { ofs << i.first << ' ' << i.second << endl; }
    return ofs;
}

ostream& operator<<(ostream& ofs, const sweep_status& sweepline) {
    ofs << sweepline.size() << endl;
    for (auto i: sweepline.data) { ofs << i << ' ' << i.evaly(sweep_x) << endl; }
    return ofs;
}

void processEvent(Line fir, Line sec, event_queue& events) {
    // fir is always top of sec int sweep line status
    // so that the intersection association of point is with top line
    double minx = events.top().first.x;
    if (checkIntersection(fir, sec)) {
        Point ans = intersect(fir, sec);
        if (ans.x > minx) events.push(make_pair(ans, fir));
    }
}

void processLeftEvents(Line cur, const sweep_status& sweepline, event_queue& events) {
    if (sweepline.existPred(cur) && sweepline.existSucc(cur)) {
        // cur in the middle
        processEvent(sweepline.getPred(cur), cur, events);
        processEvent(cur, sweepline.getSucc(cur), events);
    } else if (sweepline.existPred(cur)) {
        // is in the bottom
        processEvent(sweepline.getPred(cur), cur, events);
    } else if (sweepline.existSucc(cur)) {
        // is in the top
        processEvent(cur, sweepline.getSucc(cur), events);
    }
}

void processRightEvents(Line cur, const sweep_status& sweepline, event_queue& events) {
    if (sweepline.existPred(cur) && sweepline.existSucc(cur)) {
        // cur in the middle
        processEvent(sweepline.getPred(cur), sweepline.getSucc(cur), events);
    }
}

Line getMatchingInter(Line cur, const sweep_status& sweepline) {
    Line potential;
    if (sweepline.existSucc(cur)) {
        potential = sweepline.getSucc(cur);
        if (potential == cur) return potential;
    }
    if (sweepline.existPred(cur)) {
        potential = sweepline.getPred(cur);
        if (potential == cur) return potential;
    }

    cout << "NONE" << endl;

    return Line();
}

void processInterEvents(Line cur, const sweep_status& sweepline, event_queue& events) {
    // Line top = cur;
    // Line below = sweepline.getSucc(top);
    // Line topplus = sweepline.getPred(top);
    // Line belowminus = sweepline.getSucc(below);

    // if (below != nullptr && topplus != nullptr) { processEvent(topplus, below, events); }
    // if (top != nullptr && belowminus != nullptr) { processEvent(top, belowminus, events); }
}