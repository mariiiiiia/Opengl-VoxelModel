#include <stdio.h>     // - Just for some ASCII messages
#include <array>
#include <string>


extern std::vector<Point> vertices;
extern std::vector<Triangle> triangles;
extern std::vector<Point> vertNormal;

void showObj(std::vector<Point> &vert, std::vector<Triangle> &tr, bool solid,bool wireframe, std::vector<Point> normal, int texture);

Point sphericCoord(Point v);
