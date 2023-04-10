#include <iostream>
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;

Point_2 assignPoint(std::string p);

int main() {
    {
        std::cout
            << (CGAL::collinear(assignPoint("0, 0.3"), assignPoint("1, 0.6"), assignPoint("2, 0.9"))
                    ? "collinear okay\n"
                    : "not collinear\n");
    }
    {
        Point_2 p = assignPoint("0, 1.0 / 3.0"), q = assignPoint("1, 2.0 / 3.0"), r = assignPoint("2, 1");
        auto yt = p.y();
        std::cout << "q = " << q.x() << " " << q.y() << std::endl;
        std::cout << (CGAL::collinear(p, q, r) ? "collinear\n" : "not collinear\n");
    }
    {
        Point_2 p(0, 0), q(1, 1), r(2, 2);
        std::cout << (CGAL::collinear(p, q, r) ? "collinear\n" : "not collinear\n");
    }
    return 0;
}

Point_2 assignPoint(std::string p) {
    Point_2 ans;
    std::istringstream input(p);
    input >> p;
    return ans;
}
