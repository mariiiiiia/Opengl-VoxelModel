extern std::vector<Point> vertices;
extern std::vector<Triangle> triangles;
extern std::vector<Vector> vertNormal;

void showObj(const std::vector<Point> &vert,const std::vector<Triangle> &tr, bool solid,bool wireframe,const std::vector<Vector> vertNormal, int texture);

Point sphericCoord(Point v);
