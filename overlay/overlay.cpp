#include "arr_exact_construction_segments.h"
#include "arr_print.h"

#include <CGAL/basic.h>
#include <CGAL/Arr_overlay_2.h>

#include "timer.h"

#include <iostream>
#include <string>

using namespace std;

void createVerticalArrangement(Arrangement& arr1, int n) {
    for (int i = 1; i <= n; i++) { insert_non_intersecting_curve(arr1, Segment(Point(i, 1), Point(i, n))); }
}

void createHorizontalArrangement(Arrangement& arr1, int n) {
    for (int i = 1; i <= n; i++) { insert_non_intersecting_curve(arr1, Segment(Point(1, i), Point(n, i))); }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "value of n needed as argument" << endl;
        return -1;
    }

    int n = atoi(argv[1]);

    Arrangement arr1;
    // Construct the first arrangement, vertical
    createVerticalArrangement(arr1, n);

    // Construct the second arrangement, horizontal
    Arrangement arr2;
    createHorizontalArrangement(arr2, n);

    // Compute the overlay of the two arrangements.
    // n * (n - 1) / 2 edges
    // space increases n^2, time as nlogn
    Arrangement overlay_arr;

    Timer t1;
    t1.start("Overlay");
    CGAL::overlay(arr1, arr2, overlay_arr);
    print_arrangement_size(overlay_arr);
    t1.display();
    return 0;
}