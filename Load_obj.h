#include <stdio.h>     // - Just for some ASCII messages
#include <string>
#include <array>


void loadObj(std::string filename, std::vector<Point> &vertices, std::vector<Triangle> &triangles, std::vector<Triangle> &hornTriangles);
bool ccw_or_cw(Point p1, Point p2, Point p3);