#define VOXEL_WIDTH1 0.1
#define VOXEL_WIDTH2 0.2
#define VOXEL_WIDTH3 0.45
#define VOXEL_WIDTH4 1.0
#define VOXEL_WIDTH5 1.5

class VoxelModel{
public:
	std::vector< Voxel> voxels;
	std::vector< MarchingCube> mc;
	float voxel_width;

	float setUpVoxelWidth(); // voxel width can take 5 values 

	void setVoxels( const std::vector<Point> vert,const std::vector<Triangle> tr, const std::vector<Vector> normal);
	// draw the voxel model of a given object

	void drawVoxel( );
	// given the coordinates (x,y,z) of low-left corner of a cube, this function draws the voxel

	void boundingBoxOfTriangle(Point p1, Point p2, Point p3, float d);
	// given a triangle, this function constructs the bounding box and puts it in the voxel grid

	void triangleVoxelization( std::vector< Point> vert, Triangle tr, Vector normal, float d);
	// given a triangle and its normal vector, this function voxelizes the triangle. The function uses the 8 points of the bounding box of the triangle.

	bool voxelAlreadyExists(Voxel vox, int &voxpos);

};

bool checkLineTriangleIntersection( TriangleCoord tri, Vector normal, Point lp1, Point lp2, Point &interpoint);
	// this function takes as arguments 3 points of a triangle,its normal vector and a point. 
	//This point lp is one of the 8 points of a cube that we want to define if it is voxel or not. 

bool pointInsideCube( Point cube1, float d, Point p);		// d= cube_width 

bool pointInsideSurface( TriangleCoord tri, Vector normal, Point p);
