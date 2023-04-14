#include "geometry.h"
#include "sweep.h"

#include <iostream>
#include <fstream>

using namespace std;

double sweep_x;

int main() {
    ifstream ifs;
    ofstream ofs;

    // event queue to get points
    event_queue events;

    // sweep status in an ordered dictionary or stl set based on balanced tree
    sweep_status sweepline;

    ifs.open("testLines/2/input.txt");
    events.readLines(ifs);
    ifs.close();

    ofs.open("out.txt", ios_base::out);
    ofs << "Event Queue" << endl;
    ofs << events;
    ofs.close();

    while (!events.empty()) {
        Line cur = events.top().second;
        sweep_x = events.top().first.x;
        if (events.top().first.type == PTYPE::left) {
            sweepline.push(cur);
            processLeftEvents(cur, sweepline, events);
        } else if (events.top().first.type == PTYPE::right) {
            processRightEvents(cur, sweepline, events);
            sweepline.erase(cur);
        } else if (events.top().first.type == PTYPE::intersection) {
            cout << events.top().first << endl;
            processInterEvents(cur, sweepline, events);
            Line other = getMatchingInter(cur, sweepline);
            sweepline.erase(cur);
            sweepline.erase(other);
            sweepline.push(cur);
            sweepline.push(other);
        }

        ofs.open("status.txt", ios_base::out);
        ofs << "Sweep Status" << endl;
        ofs << sweepline;
        ofs.close();

        events.pop();
    }
}
