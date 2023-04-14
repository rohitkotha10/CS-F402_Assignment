# CS-F402 Assignment

## Build Instructions
- Tested on `Windows 11` with `mingw64` compiler for windows
- `CGAL` only used for overlay code. intersection doesn't need `CGAL`
- To get `CGAL` install using `MSYS2` and `pacman`
- Compile overlay code by linking with `-lmpfr -lgmp`

## Line Segment Intersection
- [GeoGebra](https://www.geogebra.org/m/VWN3g9rE) used to generate test cases

## Overlay of two Maps
- `CGAL 5.5.2` used and the example was taken from [here](https://doc.cgal.org/latest/Arrangement_on_surface_2/Arrangement_on_surface_2_2overlay_8cpp-example.html)