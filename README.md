# CS-F402 Assignment

## Build Instructions
- Tested on `Windows 11` with `mingw64` compiler for windows
- `CGAL` only used for overlay code. intersection doesn't need `CGAL`
- To get `CGAL` install using `MSYS2` and `pacman`
- Compile overlay code by linking with `-lmpfr -lgmp`

## Line Segment Intersection
- [GeoGebra](https://www.geogebra.org/m/VWN3g9rE) used to draw some test cases
- Compile file `intersection.cpp` using `g++`
- Input format starts with number of line segments n, followed by the 4 points x1 y1 x2 y2 for each of the following n lines 
- Specify input file as a command line argument `./intersection.exe input.txt`
- Leave the argument as blank if you want to read input from command line itself

## Overlay of two Maps
- `CGAL 5.5.2` used and the example was taken from [here](https://doc.cgal.org/latest/Arrangement_on_surface_2/Arrangement_on_surface_2_2overlay_8cpp-example.html)