#include "geometry.h"
#include "sweep.h"
#include "timer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

double sweep_x;

int main(int argc, char** argv) {
    ifstream ifs;
    ofstream ofs;

    // final answer containing intersections
    vector<Point> ans;

    // event queue stored in a map
    event_queue events;

    // sweep status stored in stl set based on balanced tree
    sweep_status sweepline;

    // handle degenrates before passing to event queue
    vector<Line> prearr;

    if (argc == 2) {
        ifs.open(argv[1]);
        prearr = readInput(ifs);
        ifs.close();
    } else
        prearr = readInput(cin);

    Timer t1;
    t1.start("Line Sweep");
    events.processLines(prearr);

    int afterInter = 0;
    Line inter1, inter2;

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

        ofs.open("event.debug", ios_base::out);
        ofs << "Event Queue" << endl;
        ofs << events;
        ofs.close();

        ofs.open("sweep.debug", ios_base::out);
        ofs << "Sweep Status" << endl;
        ofs << sweepline;
        ofs.close();
    }

    cout << ans.size() << endl;
    for (auto i: ans) cout << i << endl;
    t1.display();
}
