
void setVoxels(std::vector<Voxel> &voxels, const std::vector<Point> &vert,const std::vector<Triangle> &tr, const std::vector<Vector> &normal, float d);
// draw the voxel model of a given object

void drawVoxel( std::vector<Voxel> voxel_grid, float d);
// given the coordinates (x,y,z) of low-left corner of a cube, this function draws the voxel

void boundingBoxOfTriangle(Point p1, Point p2, Point p3, float d);
// given a triangle, this function constructs the bounding box and puts it in the voxel grid

void triangleVoxelization( Point tp1, Point tp2, Point tp3, Vector normal, float d, std::vector<Voxel > &voxels);
// given a triangle and its normal vector, this function voxelizes the triangle. The function uses the 8 points of the bounding box of the triangle. 

bool checkIntersection( Point tp1, Point tp2, Point tp3, Vector normal, Point lp);
// this function takes as arguments 3 points of a triangle,its normal vector and a point. 
//This point lp is one of the 8 points of a cube that we want to define if it is voxel or not. 