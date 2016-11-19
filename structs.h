#define FLOOR -10
#define LEFT_WALL -40
#define RIGHT_WALL 40
#define FAR_WALL -60
#define NEAR_WALL 0

class Point{
public:
	float x;
	float y;
	float z;

	void insert(float x, float y, float z){
		this->x=x;
		this->y=y;
		this->z=z;
	}

	void insert( Point p){
		this->x=p.x;
		this->y=p.y;
		this->z=p.z;
	}

	bool equals( Point p){
		if (this->x==p.x && this->y==p.y && this->z==p.z) return true;
		else return false;
	}

	bool operator>(Point p){
		if (this->x>p.x && this->y>p.y && this->z>p.z) return true;
		else return false;
	}
	
	bool operator<(Point p){
		if (this->x<p.x && this->y<p.y && this->z<p.z) return true;
		else return false;
	}

	bool operator>=(Point p){
		if (this->x>=p.x && this->y>=p.y && this->z>=p.z) return true;
		else return false;
	}
	
	bool operator<=(Point p){
		if (this->x<=p.x && this->y<=p.y && this->z<=p.z) return true;
		else return false;
	}

	Point operator- (Point p){
		Point new_p;
		new_p.x= this->x-p.x;
		new_p.y= this->y-p.y;
		new_p.z= this->z-p.z;

		return new_p;
	}
};

class Vector {
public:
	float x;
	float y;
	float z;

	bool equals( Vector v){
		if (this->x==v.x && this->y==v.y && this->z==v.z) return true;
		else return false;
	}

	void insert(float x, float y, float z){
		this->x=x;
		this->y=y;
		this->z=z;
	}

	void set( Point a, Point b){
		// a is starting point and b target point
		this->x = b.x-a.x;
		this->y = b.y-a.y;
		this->z = b.z-a.z;
	}

	Vector operator- (Vector v){
		Vector new_v;
		new_v.x= this->x-v.x;
		new_v.y= this->y-v.y;
		new_v.z= this->z-v.z;

		return new_v;
	}

	Vector operator+ (Vector v){
		Vector new_v;
		new_v.x= this->x+v.x;
		new_v.y= this->y+v.y;
		new_v.z= this->z+v.z;

		return new_v;
	}

	float dotproduct(Vector v){
		float pr;
		pr = this->x*v.x + this->y*v.y + this->z*v.z;
		
		return pr;
	}

	float dotproduct(Point p){
		float pr;
		pr = this->x*p.x + this->y*p.y + this->z*p.z;
		
		return pr;
	}

	Vector crossProduct(Vector v){
		Vector cp;
		cp.x = this->y*v.z - this->z*v.y;
		cp.y = -(this->x*v.z - this->z*v.x);
		cp.z = this->x*v.y - this->y*v.x;

		return cp;
	}

	bool operator>=(Vector v){
		if (this->x>=v.x && this->y>=v.y && this->z>=v.z) return true;
		else return false;
	}

	bool operator>(Vector v){
		if (this->x>v.x && this->y>v.y && this->z>v.z) return true;
		else return false;
	}
	
	bool operator<=(Vector v){
		if (this->x<=v.x && this->y<=v.y && this->z<=v.z) return true;
		else return false;
	}

	bool operator<(Vector v){
		if (this->x<v.x && this->y<v.y && this->z<v.z) return true;
		else return false;
	}

};

class Triangle {
public:
	int p1;  // pointers to vertices
	int p2;
	int p3;

	void set( int tp1, int tp2, int tp3){
		this->p1 = tp1;
		this->p2 = tp2;
		this->p3 = tp3;
	}
};

class TriangleCoord{
public:
	Point p1;
	Point p2;
	Point p3;

	void set( Point tp1, Point tp2, Point tp3){
		this->p1 = tp1;
		this->p2 = tp2;
		this->p3 = tp3;
	}
};

class Voxel : public Point
{
public:
	Vector velocity;

	void insert(float x, float y, float z){
		this->x=x;
		this->y=y;
		this->z=z;
	}

	bool equals( Voxel v){
		if (this->x==v.x && this->y==v.y && this->z==v.z) return true;
		else return false;
	}
};

class MarchingCube : public Voxel{
public:
	std::vector< bool> corner_states;		   // 0 for corners out of the isosurface 1 for those inside the isosurface
	std::vector< Triangle> tr;				   // triangles inside the marching cube - so that we can compute their intersection points
	std::vector< Point> intersection_points;   // intersection point between the marching cube and the isosurface
	
	void set(Voxel vox){
		this->x = vox.x;
		this->y = vox.y;
		this->z = vox.z;
	}
	
	void set(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void turnCornerOnOrOff( int p, bool st){
		if (this->corner_states.size()<8)	this->corner_states.push_back( st);
		else this->corner_states.at( p)= st;
	}

	void insertIntersectionPoint( Point p){
		this->intersection_points.push_back( p);
	}

	bool equals( MarchingCube mc){
		if (this->x==mc.x && this->y==mc.y && this->z==mc.z){
			return true;
		}
		else return false;
	}
};