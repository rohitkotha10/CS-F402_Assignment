#pragma once

#include "geometry.h"

#include <iostream>
#include <fstream>
#include <vector>

double uniform() {
    (double)rand() / RAND_MAX;
}

void genLines(std::ostream& ofs, int n) {
    ofs << n << std::endl;
    for (int i = 0; i < n; i++) {
        const double PI = 3.14159265358979732384626433832795;
        double r = 9, x = 10, y = 10;
        for (int i = 0; i < n; i++) {
            double theta = 2 * PI * uniform();

            double len = sqrt(uniform()) * r;

            std::cout << x + len * cos(theta) << ' ' << y + len * sin(theta) << std::endl;
        }
    }