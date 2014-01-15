
extern std::vector<Point> voxels;

void voxelModel(const std::vector<Point> &vert,const std::vector<Triangle> &tr, const std::vector<Point> &normal);

void drawVoxel( Point p);
// given the coordinates (x,y,z) of low-left corner of a cube, this function draws the voxel

void placeVoxel(Point p1, Point vp);
// point p1 is the given point and vp the lower-left-nearest point of voxel to be drawn

void boundingBoxOfTriangle(Point p1, Point p2, Point p3);

void triangleVoxelization( Point tp1, Point tp2, Point tp3, Point normal);