#include "line.h"

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <queue>
#include <set>

using namespace std;

struct comp {
    bool operator()(const pair<Point, int>& a, const pair<Point, int>& b) const {
        return a.first.x > b.first.x;
    };
};

vector<Line> readLines(istream& instream) {
    int n;
    instream >> n;
    vector<Line> ans(n);

    for (int i = 0; i < n; i++) { instream >> ans[i].start >> ans[i].end; }

    return ans;
}

int main() {
    ifstream ifs("linedata.txt");
    ofstream ofs("out.txt");
    vector<Line> lines = readLines(ifs);
    int n = lines.size();

    priority_queue<pair<Point, int>, vector<pair<Point, int>>, comp> events;
    for (int i = 0; i < n; i++) {
        events.push(make_pair(lines[i].start, i));
        events.push(make_pair(lines[i].end, i));
    }

    ofs << n << endl;
    for (int i = 0; i < n; i++) {
        ofs << lines[i].start << ' ' << lines[i].end << ' ' << i << endl;
    }

    while (!events.empty()) {
        ofs << events.top().first.x << ' ' << events.top().first.y << ' '
            << events.top().second << endl;
        events.pop();
    }
    ofs << "OK" << endl;
}
