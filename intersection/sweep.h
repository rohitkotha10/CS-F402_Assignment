#pragma once
#include "geometry.h"

#include <iostream>
#include <fstream>
#include <utility>
#include <memory>
#include <set>

using namespace std;

extern double sweep_x;

class event_queue {
public:
    void readLines(istream& ifs);

    void push(pair<Point, shared_ptr<Line>> cur) { data.insert(cur); };
    void erase(pair<Point, shared_ptr<Line>> cur);
    pair<Point, shared_ptr<Line>> top() { return *(data.begin()); };
    void pop() { data.erase(data.begin()); };

    int size() const { return data.size(); };
    bool empty() const { return data.empty(); };

    struct event_comp {
        bool operator()(const pair<Point, shared_ptr<Line>>& a, const pair<Point, shared_ptr<Line>>& b) const {
            return a.first.x < b.first.x || (a.first.x == b.first.x && a.first.y < b.first.y);
        };
    };

    set<pair<Point, shared_ptr<Line>>, event_comp> data;
};

class sweep_status {
public:
    void push(shared_ptr<Line> cur) { data.insert(cur); };
    void erase(shared_ptr<Line> cur);

    int size() const { return data.size(); };
    bool empty() const { return data.empty(); };

    shared_ptr<Line> getPred(shared_ptr<Line> cur) const;
    shared_ptr<Line> getSucc(shared_ptr<Line> cur) const;

    struct sweep_comp {
        bool operator()(const shared_ptr<Line> a, const shared_ptr<Line> b) const {
            return a->evaly(sweep_x) > b->evaly(sweep_x) ||
                   (a->evaly(sweep_x) == b->evaly(sweep_x) && a->left.x < b->left.x);
        };
    };

    set<shared_ptr<Line>, sweep_comp> data;
};

void event_queue::readLines(istream& instream) {
    int n;
    instream >> n;

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

        push(make_pair(temp->left, temp));
        push(make_pair(temp->right, temp));
    }
}

void event_queue::erase(pair<Point, shared_ptr<Line>> cur) {
    auto it = data.find(cur);
    data.erase(it);
}

void sweep_status::erase(shared_ptr<Line> cur) {
    auto it = data.find(cur);
    data.erase(it);
}

shared_ptr<Line> sweep_status::getPred(shared_ptr<Line> cur) const {
    auto it = data.find(cur);
    if (it == data.begin())
        return nullptr;
    else
        return *(--it);
}

shared_ptr<Line> sweep_status::getSucc(shared_ptr<Line> cur) const {
    auto it = data.find(cur);
    it++;
    if (it != data.end())
        return *it;
    else
        return nullptr;
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
    for (auto i: sweepline.data) { ofs << *i << ' ' << i->evaly(sweep_x) << endl; }
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

void processLeftEvents(shared_ptr<Line> cur, const sweep_status& sweepline, event_queue& events) {
    shared_ptr<Line> prev = sweepline.getPred(cur);
    shared_ptr<Line> succ = sweepline.getSucc(cur);
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
    shared_ptr<Line> prev = sweepline.getPred(cur);
    shared_ptr<Line> succ = sweepline.getSucc(cur);

    if (prev != nullptr && succ != nullptr) {
        // cur in the middle
        processEvent(prev, succ, events);
    }
}

void processInterEvents(shared_ptr<Line> cur, const sweep_status& sweepline, event_queue& events) {
    shared_ptr<Line> top = cur;
    shared_ptr<Line> below = sweepline.getSucc(top);
    shared_ptr<Line> topplus = sweepline.getPred(top);
    shared_ptr<Line> belowminus = sweepline.getSucc(below);

    if (below != nullptr && topplus != nullptr) { processEvent(topplus, below, events); }
    if (top != nullptr && belowminus != nullptr) { processEvent(top, belowminus, events); }
}