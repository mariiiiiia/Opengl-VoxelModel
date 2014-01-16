
extern std::vector<Point> voxels;

void voxelModel(const std::vector<Point> &vert,const std::vector<Triangle> &tr, const std::vector<Point> &normal);
// draw the voxel model of a given object

void drawVoxel( Point p);
// given the coordinates (x,y,z) of low-left corner of a cube, this function draws the voxel

void boundingBoxOfTriangle(Point p1, Point p2, Point p3);
// given a triangle, this function constructs the bounding box and puts it in the voxel grid

void triangleVoxelization( Point tp1, Point tp2, Point tp3, Point normal);
// given a triangle and its normal vector, this function voxelizes the triangle. The function uses the 8 points of the bounding box of the triangle. 

bool checkIntersection( Point tp1, Point tp2, Point tp3, Point normal, Point lp);
// this function takes as arguments 3 points of a triangle,its normal vector and a point. 
//This point lp is one of the 8 points of a cube that we want to define if it is voxel or not. 