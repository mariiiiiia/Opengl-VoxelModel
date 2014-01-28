#define VOXEL_WIDTH 0.8
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
	
	bool operator<=(Vector v){
		if (this->x<=v.x && this->y<=v.y && this->z<=v.z) return true;
		else return false;
	}

};

class Triangle {
	public:
		int p1;  // pointers to vertices
		int p2;
		int p3;
};

class Voxel : public Point
{
public:
//	float x;
//	float y;
//	float z;

	static inline float width(){
		return VOXEL_WIDTH;
	}

	void insert(float x, float y, float z){
		this->x=x;
		this->y=y;
		this->z=z;
	}

	// voxel == point
	//bool operator==(const Point &p) {
	//	return equals(p);
	//}

	//bool equals(const Point &p){
	//	if (this->x==p.x && this->y==p.y && this->z==p.z) return true;
	//	else return false;
	//}

	bool equals( Voxel v){
		if (this->x==v.x && this->y==v.y && this->z==v.z) return true;
		else return false;
	}

};

