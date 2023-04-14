#include "geometry.h"
#include "sweep.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <set>

using namespace std;

double sweep_x;

int main() {
    ifstream ifs;
    ofstream ofs;

    // event queue to get points
    event_queue events;

    // sweep status in an ordered dictionary or stl set based on balanced tree
    sweep_status sweepline;

    ifs.open("testLines/1/input.txt");
    events.readLines(ifs);
    ifs.close();

    ofs.open("out.txt", ios_base::out);
    logInput(ofs, events);
    ofs.close();

    while (!events.empty()) {
        shared_ptr<Line> cur = events.top().second;
        sweep_x = events.top().first.x;
        if (events.top().first.type == PTYPE::left) {
            sweepline.push(cur);
            processLeftEvents(cur, sweepline, events);
        } else if (events.top().first.type == PTYPE::right) {
            processRightEvents(cur, sweepline, events);
            sweepline.erase(cur);
        } else if (events.top().first.type == PTYPE::intersection) {
            // the intersection is always for the top line, so it is exchanged with line below
            // this below line will be the successor
            processInterEvents(cur, sweepline, events);
            shared_ptr<Line> top = cur;
            shared_ptr<Line> bottom = sweepline.getSucc(top);
        }

        ofs.open("status.txt", ios_base::out);
        logStatus(ofs, sweepline);
        ofs.close();
        events.pop();
    }
}
