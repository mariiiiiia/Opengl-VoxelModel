
void marchingCubes( VoxelModel &voxmod, std::vector< Point> vert, std::vector<Triangle> &newTriangles);

void ccwTriangle( Point &v1, Point &v2, Point &v3);

Point lineTriangleIntersection( TriangleCoord tr, Point c1, Point c2, bool &intersp_found);

Point checkLineTriangleIntersection( TriangleCoord tri, Point lp1, Point lp2, bool &intersp_found);

int constructCornerIndex( MarchingCube mc);

void listMCIntersectionPoints( int edges, std::vector< Point> vertices, std::vector< Triangle> triangles, std::vector< Point> cube, std::vector< Point> &vert_list);

bool findPointInVertices( Point v, std::vector< Point> vertices, int &pointerToVertice);

Point rayTriangleIntersection( TriangleCoord T, Point c1, Point c2, bool &inters_found);