#pragma once
#include "geometry.h"

#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <set>
#include <vector>

using namespace std;

extern double sweep_x;

class event_queue {
public:
    void processLines(const vector<Line>& arr);

    void push(pair<Point, Line> cur) { data.insert(cur); };
    void erase(pair<Point, Line> cur);
    pair<Point, Line> top() { return *(data.begin()); };
    void pop() { data.erase(data.begin()); };

    int size() const { return data.size(); };
    bool empty() const { return data.empty(); };

    multimap<Point, Line> data;
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

void event_queue::processLines(const vector<Line>& arr) {
    if (arr.size() == 0) return;
    for (int i = 0; i < arr.size(); i++) {
        this->push(make_pair(arr[i].left, arr[i]));
        this->push(make_pair(arr[i].right, arr[i]));
    }
}

void event_queue::erase(pair<Point, Line> cur) {
    typedef multimap<Point, Line>::iterator iterator;
    pair<iterator, iterator> iterpair = data.equal_range(cur.first);

    iterator it = iterpair.first;
    for (; it != iterpair.second;) {
        if (it->second == cur.second) {
            it = data.erase(it);
        } else
            it++;
    }
}

void sweep_status::erase(Line cur) {
    auto it = data.find(cur);
    data.erase(it);
}

bool sweep_status::existPred(Line cur) const {
    auto it = data.find(cur);
    if (it == data.begin())
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
    it++;
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

vector<Line> readInput(istream& instream) {
    int n;
    instream >> n;
    vector<Line> ans(n);
    for (int i = 0; i < n; i++) { instream >> ans[i]; }
    return ans;
}

void processEvent(Line fir, Line sec, event_queue& events) {
    // fir is below line i.e least y before intersection
    if (checkIntersection(fir, sec)) {
        Point ans = intersect(fir, sec);
        if (compare(ans.x, sweep_x) == 1) events.push(make_pair(ans, fir));
    }
}

void removeEvent(Line fir, Line sec, event_queue& events) {
    // fir is below line i.e least y before intersection
    if (checkIntersection(fir, sec)) {
        Point ans = intersect(fir, sec);
        if (compare(ans.x, sweep_x) == 1) { events.erase(make_pair(ans, fir)); }
    }
}

void processLeftEvents(Line cur, const sweep_status& sweepline, event_queue& events) {
    if (sweepline.existPred(cur) && sweepline.existSucc(cur)) {
        // cur in the middle
        removeEvent(sweepline.getPred(cur), sweepline.getSucc(cur), events);
        processEvent(sweepline.getPred(cur), cur, events);
        processEvent(cur, sweepline.getSucc(cur), events);
    } else if (sweepline.existSucc(cur)) {
        // is in the bottom
        processEvent(cur, sweepline.getSucc(cur), events);
    } else if (sweepline.existPred(cur)) {
        // is in the top
        processEvent(sweepline.getPred(cur), cur, events);
    }
}

void processRightEvents(Line cur, const sweep_status& sweepline, event_queue& events) {
    if (sweepline.existPred(cur) && sweepline.existSucc(cur)) {
        // cur in the middle
        processEvent(sweepline.getPred(cur), sweepline.getSucc(cur), events);
    }
}

void processInterEvents(Line cur, const sweep_status& sweepline, event_queue& events) {
    Line below, top;
    if (sweepline.existSucc(cur)) {
        below = cur;
        top = sweepline.getSucc(cur);
    } else if (sweepline.existPred(cur)) {
        below = sweepline.getPred(cur);
        top = cur;
    }

    if (sweepline.existSucc(top)) {
        removeEvent(top, sweepline.getSucc(top), events);
        processEvent(below, sweepline.getSucc(top), events);
    }

    if (sweepline.existPred(below)) {
        removeEvent(sweepline.getPred(below), below, events);
        processEvent(sweepline.getPred(below), top, events);
    }
}