#include "geometry.h"
#include "sweep.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

double sweep_x;

int main(int argc, char** argv) {
    ifstream ifs;
    ofstream ofs;

    // event queue stored in a map
    event_queue events;

    // sweep status stored in stl set based on balanced tree
    sweep_status sweepline;

    if (argc == 2) {
        ifs.open(argv[1]);
        events.readLines(ifs);
        ifs.close();
    } else
        events.readLines(cin);

    int afterInter = 0;
    Line inter1, inter2;

    vector<Point> ans;

    while (!events.empty()) {
        Line cur = events.top().second;
        sweep_x = events.top().first.x;

        if (afterInter == 1) {
            sweepline.push(inter1);
            sweepline.push(inter2);
            afterInter = 0;
        }
        if (events.top().first.type == PTYPE::left) {
            sweepline.push(cur);
            processLeftEvents(cur, sweepline, events);
        } else if (events.top().first.type == PTYPE::right) {
            processRightEvents(cur, sweepline, events);
            sweepline.erase(cur);
        } else if (events.top().first.type == PTYPE::intersection) {
            ans.push_back(events.top().first);
            processInterEvents(cur, sweepline, events);
            Line other = sweepline.getSucc(cur);
            afterInter = 1;
            inter1 = cur;
            inter2 = other;
            sweepline.erase(cur);
            sweepline.erase(other);
        }
        events.pop();

        ofs.open("event.txt", ios_base::out);
        ofs << "Event Queue" << endl;
        ofs << events;
        ofs.close();

        ofs.open("sweep.txt", ios_base::out);
        ofs << "Sweep Status" << endl;
        ofs << sweepline;
        ofs.close();
    }

    cout << ans.size() << endl;
    for (auto i: ans) cout << i << endl;
}
