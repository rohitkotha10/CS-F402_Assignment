#include "line.h"

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <queue>
#include <set>

using namespace std;

int compare(double a, double b) {
    if (abs(a - b) <= 0.001)
        return 0;
    else if (a < b)
        return -1;
    else
        return 1;
}

struct comp {
    bool operator()(const pair<Point, int>& a, const pair<Point, int>& b) const {
        return compare(a.first.x, b.first.x) > 0;
    };
};

struct sweepcomp {
    bool operator()(const pair<int, double>& a, const pair<int, double>& b) const {
        return compare(a.second, b.second) > 0;
    };
};

void logInput(
    ostream& ofs, const vector<Line>& lines, priority_queue<pair<Point, int>, vector<pair<Point, int>>, comp> events) {
    ofs << lines.size() << endl;
    for (int i = 0; i < lines.size(); i++) { ofs << lines[i].left << ' ' << lines[i].right << ' ' << i << endl; }
    while (!events.empty()) {
        ofs << events.top().first.x << ' ' << events.top().first.y << ' ' << events.top().second << endl;
        events.pop();
    }
}

void logStatus(ostream& ofs, const vector<Line>& lines, multiset<pair<int, double>, sweepcomp> sweepStatus) {
    ofs << lines.size() << endl;
    for (int i = 0; i < lines.size(); i++) { ofs << lines[i].left << ' ' << lines[i].right << ' ' << i << endl; }
    for (auto& i: sweepStatus) { ofs << i.first << ' ' << i.second << endl; }
}

Point intersect(const Line& a, const Line& b) {
    return Point(0, 0, PTYPE::intersection);
}

vector<Line> readLines(istream& instream) {
    int n;
    instream >> n;
    vector<Line> ans(n);

    for (int i = 0; i < n; i++) {
        Point a, b;
        instream >> a >> b;
        if (compare(a.x, b.x) == 0 && compare(a.y, b.y) == 0)  // ignore point
            continue;
        if (compare(a.x, b.x) < 0 || (compare(a.x, b.x) == 0 && compare(a.y, b.y) < 0)) {
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

    multiset<pair<int, double>, sweepcomp> sweepStatus;

    while (!events.empty()) {
        if (events.top().first.type == PTYPE::left) {
            auto it = sweepStatus.insert(make_pair(events.top().second, events.top().first.y));

            it++;  // cout << *(it++)->second << endl;
        }
        events.pop();
    }
    logStatus(ofs, lines, sweepStatus);
    auto it = sweepStatus.begin();
    it++;
    it++;
    if (it != sweepStatus.end()) { cout << it->first << "curr" << it->second << endl; }
    if (it != sweepStatus.begin()) {
        auto prev = it--;
        cout << it->first << "pred" << it->second << endl;
        it++;
    }
    it++;
    if (it != sweepStatus.end()) { cout << it->first << "succ" << it->second << endl; }
    ofs << "OK" << endl;
}
