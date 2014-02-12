
Vector CalcTriangleNormal( std::vector< Point> vertices, Triangle triangle);

Vector CalcTriangleNormal( TriangleCoord triangle);

void avgNormals( std::vector< Vector> &vertNormal, std::vector< Vector> normal, std::vector<Triangle> triangle, int vertices_quantity);