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

	bool comparisonGreater(Point p){
		if (this->x>p.x && this->y>p.y && this->z>p.z) return true;
		else return false;
	}
	
	bool comparisonLower(Point p){
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


};

class Triangle {
	public:
		int p1;  // pointers to vertices
		int p2;
		int p3;
};

class Voxel{
public:
	float x;
	float y;
	float z;

	float width(){
		float voxel_width=1.0;
		return voxel_width;
	}

	void insert(float x, float y, float z){
		this->x=x;
		this->y=y;
		this->z=z;
	}

	bool equals( Point p){
		if (this->x==p.x && this->y==p.y && this->z==p.z) return true;
		else return false;
	}

	bool equals( Voxel v){
		if (this->x==v.x && this->y==v.y && this->z==v.z) return true;
		else return false;
	}

};

