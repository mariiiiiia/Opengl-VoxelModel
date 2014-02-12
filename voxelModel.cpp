#include <stdio.h>     // - Just for some ASCII messages
#include <string>
#include <array>
#include <vector>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "structs.h"
#include "voxelModel.h"



Point bound_box1;		// points that define the bounding box, where i search for the voxels that should be drawn
static float width_x, width_y, width_z;        // widths of bounding box

void VoxelModel::setVoxels( const std::vector<Point> vert, const std::vector<Triangle> tr, const std::vector<Vector> normal){   
	// VOXEL WIDTH
	float d=VOXEL_WIDTH;      

	std::vector< Point> bound_box;

	//voxels.clear();
	
	if (int(this->voxels.size())==0 ){
		for (int k=0; k<int(tr.size()); k++){
			Point p1 = vert.at(tr.at(k).p1);
			Point p2 = vert.at(tr.at(k).p2);
			Point p3 = vert.at(tr.at(k).p3);

			this->boundingBoxOfTriangle(p1,p2,p3, d);
			this->triangleVoxelization(vert, tr.at(k), normal.at(k), d);
		}			
		//delete multiple voxels at same coordinates
		for (int i=0; i<int(this->voxels.size()); i++){
			for (int j=i; j<int(voxels.size()); j++){
				if ( i!=j && voxels.at(i).equals( this->voxels.at(j)) ) {
					this->voxels.erase( this->voxels.begin()+j);
					//this->mc.erase( this->mc.begin()+j);
				}
			}
		}
		// sort voxels on the y axes - this helps the free fall process - otherwise there is no point to
		Point temp;
		for (int i=0; i<int(this->voxels.size()); i++){
			for (int j=0;j<int(this->voxels.size()); j++){
				if ( this->voxels.at(j).y>voxels.at(i).y ){
					temp.insert( this->voxels.at(j).x, this->voxels.at(j).y, this->voxels.at(j).z );
					this->voxels.at(j).insert( this->voxels.at(i).x, this->voxels.at(i).y, this->voxels.at(i).z);
					this->voxels.at(i).insert(temp.x, temp.y, temp.z);
				}
			}
		}
	}
}

#define GL_SQUARE(x, y, z, d)  do { \
	glBegin(GL_QUADS); \
			glVertex3f( x, y, z-d); \
			glVertex3f( x, y+d, z-d); \
			glVertex3f( x+d, y+d, z-d); \
			glVertex3f( x+d, y, z-d); \
			glEnd(); } while(0)

// GL_SQUARE(x, y, z, d);
void VoxelModel::drawVoxel( ){
	float d = this->voxels.at(0).width();
	for (int i=0; i<int(this->voxels.size()); i++){
		float x=this->voxels.at(i).x-d/2, y=this->voxels.at(i).y-d/2, z=this->voxels.at(i).z+d/2;

		//// material identities
		//float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		//float ambReflection[] = { 0.5, 0.5, 0.5, 1.0f };
		//float diffReflection[] = { 0.5, 0.5, 0.5, 1.0f };
		//glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
		//glMaterialfv(GL_FRONT, GL_AMBIENT, ambReflection);
		//glMaterialfv(GL_FRONT, GL_DIFFUSE, diffReflection);
		//
		//glColor3f(0.2,0.2,1);	
		//glBegin(GL_POINTS);
		//glVertex3f( x+d/2, y+d/2, z-d/2);
		//glEnd();
		
		//glColor4f(0.9,0.7,0.9, 0.7);

		glColor4f(0.8,0.8,0.8,0.2);
		

		glBegin(GL_QUADS);
			glNormal3f(0,0,-1);	glVertex3f( x, y, z-d);
			glNormal3f(0,0,-1);	glVertex3f( x, y+d, z-d);
			glNormal3f(0,0,-1);	glVertex3f( x+d, y+d, z-d);
			glNormal3f(0,0,-1);	glVertex3f( x+d, y, z-d);
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(1,0,0);	glVertex3f( x+d, y, z);
			glNormal3f(1,0,0); glVertex3f( x+d, y, z-d);
			glNormal3f(1,0,0); glVertex3f( x+d, y+d, z-d);
			glNormal3f(1,0,0); glVertex3f( x+d, y+d, z);
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(0,0,1); glVertex3f( x, y, z);
			glNormal3f(0,0,1); glVertex3f( x+d, y, z);
			glNormal3f(0,0,1); glVertex3f( x+d, y+d, z);
			glNormal3f(0,0,1); glVertex3f( x, y+d, z);
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(-1,0,0); glVertex3f( x, y, z);
			glNormal3f(-1,0,0); glVertex3f( x, y+d, z);
			glNormal3f(-1,0,0); glVertex3f( x, y+d, z-d);
			glNormal3f(-1,0,0); glVertex3f( x, y, z-d);
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(0,1,0); glVertex3f( x, y+d, z-d);
			glNormal3f(0,1,0); glVertex3f( x, y+d, z);
			glNormal3f(0,1,0); glVertex3f( x+d, y+d, z);
			glNormal3f(0,1,0); glVertex3f( x+d, y+d, z-d);
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(0,-1,0); glVertex3f( x, y, z);
			glNormal3f(0,-1,0); glVertex3f( x, y, z-d);
			glNormal3f(0,-1,0); glVertex3f( x+d, y, z-d);
			glNormal3f(0,-1,0); glVertex3f( x+d, y, z);
		glEnd();

		glColor3f(1,0,0);

		glBegin(GL_LINE_LOOP);
			glVertex3f( x, y, z-d);
			glVertex3f( x, y+d, z-d);
			glVertex3f( x+d, y+d, z-d);
			glVertex3f( x+d, y, z-d);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3f( x+d, y, z);
			glVertex3f( x+d, y, z-d);
			glVertex3f( x+d, y+d, z-d);
			glVertex3f( x+d, y+d, z);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3f( x, y, z);
			glVertex3f( x+d, y, z);
			glVertex3f( x+d, y+d, z);
			glVertex3f( x, y+d, z);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3f( x, y, z);
			glVertex3f( x, y+d, z);
			glVertex3f( x, y+d, z-d);
			glVertex3f( x, y, z-d);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3f( x, y+d, z-d);
			glVertex3f( x, y+d, z);
			glVertex3f( x+d, y+d, z);
			glVertex3f( x+d, y+d, z-d);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3f( x, y, z);
			glVertex3f( x, y, z-d);
			glVertex3f( x+d, y, z-d);
			glVertex3f( x+d, y, z);
		glEnd();
	}
}

void VoxelModel::boundingBoxOfTriangle( Point tp1, Point tp2, Point tp3, float d){
	float xmin,ymin,zmin, xmax,ymax,zmax;

	xmin=std::min( tp1.x, std::min(tp2.x, tp3.x));
	ymin=std::min( tp1.y, std::min(tp2.y, tp3.y));
	zmin=std::min( tp1.z, std::min(tp2.z, tp3.z));

	xmax=std::max( tp1.x, std::max(tp2.x, tp3.x));
	ymax=std::max( tp1.y, std::max(tp2.y, tp3.y));
	zmax=std::max( tp1.z, std::max(tp2.z, tp3.z));

    if (xmin>0) xmin = int( xmin/d ) *d - d;
	else if (xmin<0) xmin = -int( abs(xmin/d)) *d - d;

	if (ymin>0) ymin = int( ymin/d ) *d - d;
	else if (ymin<0) ymin = -int( abs(ymin/d) ) *d -d;

	if (zmin>0) zmin = int( zmin/d ) *d - d;
	else if (zmin<0) zmin = -int( abs(zmin/d) ) *d - d;


	if (xmax>0)	xmax = int( xmax/d ) *d + d;
	else if (xmax<0) xmax = -int( abs(xmax/d) ) *d + d;

	if (ymax>0) ymax = int( ymax/d ) *d + d;
	else if (ymax<0) ymax = -int( abs(ymax/d) ) *d + d;

	if (zmax>0) zmax = int( zmax/d ) *d + d;
	else if (zmax<0) zmax = -int( abs(zmax/d) ) *d + d;
	
	if (xmin==0 && xmax==0) xmin=-d;
	if (ymin==0 && ymax==0) ymin=-d;
	if (zmin==0 && zmax==0) zmin=-d;

	bound_box1.insert( xmin, ymin, zmax);

	width_x = abs(xmax-xmin);
	width_y = abs(ymax-ymin);
	width_z = abs(zmax-zmin);
}

void VoxelModel::triangleVoxelization( std::vector< Point> vert, Triangle tr, Vector normal, float d){
	Point tp1, tp2, tp3;
	tp1 = vert.at(tr.p1);
	tp2 = vert.at(tr.p2);
	tp3 = vert.at(tr.p3);
	TriangleCoord tri;
	tri.set( tp1, tp2, tp3);

	// every triangle has its bounding box which consists of potential voxels
	// this function defines which voxels are on the triangle plane => so they should be rendered
	// -----------------
	// calculate dot products between each vector tp1 (this could be tp2 or tp3) to the potential-voxel points (this is 8 vectors) and normal vector
	// if these dot products have the same sign, then this cube is not a voxel because it is above or below the triangle 
	// otherwise, it's upon the triangle so it should be rendered

	float dotpr1,dotpr2,dotpr3,dotpr4,dotpr5,dotpr6,dotpr7,dotpr8; // dot products between a->p(i) vectors (i=0,1,2,...,8) and normal
	Vector ap1,ap2,ap3,ap4,ap5,ap6,ap7,ap8;			// vectors from triangle point to the cube points that have to be checked if they're above or below the triangle
	Point cube1,cube2,cube3,cube4,cube5,cube6,cube7,cube8;   // the point (xmin,ymin,zmax) of the voxel to be rendered
	Point cp;
	

	for (float i=0; i<=(int(width_x/d)); i++){
		for (float j=0; j<=(int(width_y/d)); j++){
			for (float k=0; k<=(int(width_z/d)); k++){	

				cube1.insert( bound_box1.x+i*d, bound_box1.y+j*d, bound_box1.z-k*d);
				ap1.insert( tp1.x-cube1.x, tp1.y-cube1.y, tp1.z-cube1.z);  //vector a->p1
				dotpr1 = normal.dotproduct( ap1);

				cube2.insert( cube1.x+d, cube1.y, cube1.z);
				ap2.insert( tp1.x-cube2.x, tp1.y-cube2.y, tp1.z-cube2.z);  //vector a->p2
				dotpr2 = normal.dotproduct( ap2);
				
				cube3.insert( cube2.x, cube2.y+d, cube2.z);
				ap3.insert( tp1.x-cube3.x, tp1.y-cube3.y, tp1.z-cube3.z);  //vector a->p3
				dotpr3 = normal.dotproduct( ap3);
				
				cube4.insert( cube3.x-d, cube3.y, cube3.z);
				ap4.insert( tp1.x-cube4.x, tp1.y-cube4.y, tp1.z-cube4.z);  //a->p4
				dotpr4 = normal.dotproduct( ap4);

				cube5.insert( cube4.x, cube4.y-d, cube4.z-d);					
				ap5.insert( tp1.x-cube5.x, tp1.y-cube5.y, tp1.z-cube5.z);  //a->p5
				dotpr5 = normal.dotproduct( ap5);

				cube6.insert( cube5.x, cube5.y+d, cube5.z);
				ap6.insert( tp1.x-cube6.x, tp1.y-cube6.y, tp1.z-cube6.z);  //a->p6
				dotpr6 = normal.dotproduct( ap6);

				cube7.insert( cube6.x+d, cube6.y, cube6.z);
				ap7.insert( tp1.x-cube7.x, tp1.y-cube7.y, tp1.z-cube7.z);  //a->p7
				dotpr7 = normal.dotproduct( ap7);

				cube8.insert( cube7.x, cube7.y-d, cube7.z);
				ap8.insert( tp1.x-cube8.x, tp1.y-cube8.y, tp1.z-cube8.z);  //a->p8
				dotpr8 = normal.dotproduct( ap8);			

				if (  !(dotpr1>0.0 && dotpr2>0.0 && dotpr3>0.0 && dotpr4>0.0 && dotpr5>0.0 && dotpr6>0.0 && dotpr7>0.0 && dotpr8>0.0) &&
					  !(dotpr1<0.0 && dotpr2<0.0 && dotpr3<0.0 && dotpr4<0.0 && dotpr5<0.0 && dotpr6<0.0 && dotpr7<0.0 && dotpr8<0.0) ){
					// NOW WE KNOW THE CUBE IS AT THE TRIANGLE'S PLANE
					// WE NEED TO CHECK IF THE CUBE IS INSIDE THE TRIANGLE 
					
					cp.insert( cube1.x+d/2, cube1.y+d/2, cube1.z-d/2);

					//if ( checkLineTriangleIntersection( tri, normal, cube1, cube2) ||   //edge1
					//     checkLineTriangleIntersection( tri, normal, cube2, cube3) ||	//edge2
					//	 checkLineTriangleIntersection( tri, normal, cube3, cube4) ||	//edge3
					//     checkLineTriangleIntersection( tri, normal, cube4, cube1) ||	//edge4
					//	 checkLineTriangleIntersection( tri, normal, cube5, cube6) ||	//edge5
					//	 checkLineTriangleIntersection( tri, normal, cube6, cube7) ||	//edge6
					//	 checkLineTriangleIntersection( tri, normal, cube7, cube8) ||	//edge7 
					//	 checkLineTriangleIntersection( tri, normal, cube8, cube5) ||	//edge8
					//	 checkLineTriangleIntersection( tri, normal, cube1, cube5) ||	//edge9
					//	 checkLineTriangleIntersection( tri, normal, cube4, cube6) ||	//edge10
					//	 checkLineTriangleIntersection( tri, normal, cube3, cube7) ||	//edge11
					//	 checkLineTriangleIntersection( tri, normal, cube2, cube8) || 	//edge12
					//	 checkLineTriangleIntersection( tri, normal, cube1, cube7) ||
					//	 checkLineTriangleIntersection( tri, normal, cube2, cube6) ||
					//	 checkLineTriangleIntersection( tri, normal, cube4, cube8) ||
					//	 checkLineTriangleIntersection( tri, normal, cube5, cube3)  ) {

					if ( checkIntersection( tp1, tp2, tp3, normal, cube1) ||
					     checkIntersection( tp1, tp2, tp3, normal, cube2) ||
						 checkIntersection( tp1, tp2, tp3, normal, cube3) ||
					     checkIntersection( tp1, tp2, tp3, normal, cube4) ||
						 checkIntersection( tp1, tp2, tp3, normal, cube5) ||
						 checkIntersection( tp1, tp2, tp3, normal, cube6) ||
						 checkIntersection( tp1, tp2, tp3, normal, cube7) ||
						 checkIntersection( tp1, tp2, tp3, normal, cube8) ||
						 checkIntersection( tp1, tp2, tp3, normal, cp)  ) {
							 //===================== INSERT VOXEL TO LIST =====================================//
							 Voxel vox;
							 vox.insert(cube1.x+d/2, cube1.y+d/2, cube1.z-d/2);
							 this->voxels.push_back(vox);
							 //==================== INSERT MARCHING CUBE TO LIST ================================//
							 MarchingCube march_cube;
							 march_cube.set( vox);
							 this->mc.push_back(march_cube);

							 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 1, dotpr1<0);		// corner state is true if it is outside the surface
							 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 2, dotpr2<0);		
							 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 3, dotpr3<0);
							 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 4, dotpr4<0);
							 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 5, dotpr5<0);
							 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 6, dotpr6<0);
							 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 7, dotpr7<0);
							 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 8, dotpr8<0);

							 //Triangle tr;
							 //tr.set( tr.p1, tr.p2, tr.p3);
							 this->mc.at( mc.size()-1).tr.push_back( tr); 
					}
				}
			}
		}
	}
}

bool checkLineTriangleIntersection( TriangleCoord tri, Vector normal, Point lp1, Point lp2){
	Point tp1, tp2, tp3;
	tp1.insert(tri.p1.x, tri.p1.y, tri.p1.z);
	tp2.insert(tri.p2.x, tri.p2.y, tri.p2.z);
	tp3.insert(tri.p3.x, tri.p3.y, tri.p3.z);

	// plane equation: Ax+By+Cz=D
	// compute triangle vectors
	Vector ab,bc,ca,ac, line_dir;
	ab.insert(tp2.x-tp1.x, tp2.y-tp1.y, tp2.z-tp1.z);  // vector tp1->tp2
	bc.insert(tp3.x-tp2.x, tp3.y-tp2.y, tp3.z-tp2.z);  // vector tp2->tp3
	ca.insert(tp1.x-tp3.x, tp1.y-tp3.y, tp1.z-tp3.z);  // vector tp3->tp1
	ac.insert(tp3.x-tp1.x, tp3.y-tp1.y, tp3.z-tp1.z);  // vector tp1->tp3
	// calculate NOT normalized normal
	normal = ab.crossProduct(ac);
	// calculate line direction 
	line_dir.insert( lp2.x-lp1.x, lp2.y-lp1.y, lp2.z-lp1.z);
	//float dist =  sqrt( pow(line_dir.x,2)+ pow(line_dir.y,2)+ pow(line_dir.z,2));
	//line_dir.insert( line_dir.x/dist, line_dir.y/ dist, line_dir.z/ dist);
	// check if line and triangle plane are parallel
	float NDotRayDir = normal.dotproduct( line_dir);
	if (NDotRayDir==0) return false;	// line and triangle plane are parallel!
	// calculate D
	float D = normal.dotproduct(tp1);
	// line parametric form p+n*t,where p=(p.x,p.y,p.z) and n=triangle normal
	// compute t
	float t= -( D + normal.dotproduct(lp1))/ NDotRayDir;
	if (t<0) return false;
	// compute the intersection point 
	Point interp;
	interp.insert( lp1.x+t*line_dir.x, lp1.y+t*line_dir.y, lp1.z+t*line_dir.z);
	// check if the intersection point interp is inside or outside the triangle
	// that means we check if the point is at the right side of the triangle vectors 
	// that's legal if triangles are counter-clockwise
	//------------------------------------------------------
	Vector ap,bp,cp;
	ap.insert( interp.x-tp1.x, interp.y-tp1.y, interp.z-tp1.z);     //vector tp1->interp
	Vector n1;
	n1 = ab.crossProduct(ap);
	float dotpr0 = normal.dotproduct(n1);
	if (dotpr0<0) return false;  // interp is on the right side 

	bp.insert( interp.x-tp2.x, interp.y-tp2.y, interp.z-tp2.z);
	Vector n2;
	n2 = bc.crossProduct(bp);
	float dotpr1 = normal.dotproduct(n2);
	if (dotpr1<0) return false;  // interp is on the right side
		
	cp.insert( interp.x-tp3.x, interp.y-tp3.y, interp.z-tp3.z);
	Vector n3;
	n3 = ca.crossProduct(cp);
	float dotpr2 = normal.dotproduct(n3);
	if (dotpr2<0) return false;  // interp is on the right side
	
	glColor3f( 1, 0.3,1);
	glBegin( GL_POINTS);
	glVertex3f( interp.x, interp.y, interp.z);
	glEnd();
	return true;
}

bool checkIntersection( Point tp1, Point tp2, Point tp3, Vector normal, Point lp){
	// plane equation: Ax+By+Cz=D
	// compute triangle vectors
	Vector ab,bc,ca, ac;
	ab.insert(tp2.x-tp1.x, tp2.y-tp1.y, tp2.z-tp1.z);  //vector tp1->tp2
	bc.insert(tp3.x-tp2.x, tp3.y-tp2.y, tp3.z-tp2.z);  // vector tp2->tp3
	ca.insert(tp1.x-tp3.x, tp1.y-tp3.y, tp1.z-tp3.z);  // vector tp3->tp1
	ac.insert(tp3.x-tp1.x, tp3.y-tp1.y, tp3.z-tp1.z);  //vector tp1->tp2
	// calculate NOT normalized normal
	normal = ab.crossProduct(ac);
	// calculate D
	float D = normal.dotproduct(tp1);
	// line parametric form p+n*t,where p=(p.x,p.y,p.z) and n=triangle normal
	// compute t
	float t= ( D - normal.dotproduct(lp))/ (normal.dotproduct(normal));
	// compute the intersection point 
	Point interp;
	interp.insert( lp.x+t*normal.x, lp.y+t*normal.y, lp.z+t*normal.z);
	// check if the intersection point interp is inside or outside the triangle
	// that means we check if the point is at the right side of the triangle vectors 
	// that's legal if triangles are counter-clockwise
	//-------------------------
	Vector ap,bp,cp;
	ap.insert( lp.x-tp1.x, lp.y-tp1.y, lp.z-tp1.z);     //vector tp1->lp
	Vector n1;
	n1 = ab.crossProduct(ap);
	float dotpr0 = normal.dotproduct(n1);
	if (dotpr0<0) return false;  // lp is on the right side 

	bp.insert( lp.x-tp2.x, lp.y-tp2.y, lp.z-tp2.z);
	Vector n2;
	n2 = bc.crossProduct(bp);
	float dotpr1 = normal.dotproduct(n2);
	if (dotpr1<0) return false;  // lp is on the right side

	cp.insert( lp.x-tp3.x, lp.y-tp3.y, lp.z-tp3.z);
	Vector n3;
	n3 = ca.crossProduct(cp);
	float dotpr2 = normal.dotproduct(n3);
	if (dotpr2<0) return false;  // lp is on the right side

	return true;

}