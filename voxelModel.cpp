#include <stdio.h>     // - Just for some ASCII messages
#include <string>
#include <array>
#include <vector>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "visuals.h"  // for the Point and Triangle structures only
#include "voxelModel.h"


std::vector<Point> voxels;

Point bound_box1, bound_box2, bound_box3, bound_box4, bound_box5, bound_box6, bound_box7, bound_box8;  // points that define the bounding box, where i search for the voxels that should be drawn
float width_x, width_y, width_z;        // widths of bounding box

float d=0.2;		// voxel width

int w=0, b=1;

void voxelModel(const std::vector<Point> &vert,const std::vector<Triangle> &tr, const std::vector<Point> &normal){
	float x,y,z;

	voxels.clear();
	glColor4f( 1,0,0.5, 1.0);
	for (int k=0; k<1; k++){
		Point p1,p2,p3;
		p1.insert( 0,1,0);
		p2.insert( 0, 3,0);
		p3.insert( 0,1.5,1.8);

		glBegin( GL_TRIANGLES);
		glVertex3f( p1.x,p1.y,p1.z); glVertex3f( p2.x,p2.y,p2.z); glVertex3f(p3.x, p3.y,p3.z);
		glEnd();

	//for (int k=0; k<tr.size(); k++){
	//	Point p1 = vert.at(tr.at(k).p1);
	//	Point p2 = vert.at(tr.at(k).p2);
	//	Point p3 = vert.at(tr.at(k).p3);

		boundingBoxOfTriangle(p1,p2,p3);
		triangleVoxelization(p1,p2,p3, normal.at(k));
	}			
}

void drawVoxel( Point p){
	voxels.push_back( p);

	float x=p.x, y=p.y, z=p.z;

	glColor3f(0.2,0.2,1);
	
	glBegin(GL_POINTS);
	glVertex3f( x+d/2, y+d/2, z-d/2);
	glEnd();


	//if (b==1) {glColor4f(1,1,1, 0.5); b=0;w=1;}
	//else if (w==1) {glColor4f(0,1.0,0, 0.5); b=1;w=0;}
	glColor4f(1,1,1, 0.5);
	

	glBegin(GL_QUADS);
		glVertex3f( x, y, z-d);
		glVertex3f( x, y+d, z-d);
		glVertex3f( x+d, y+d, z-d);
		glVertex3f( x+d, y, z-d);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f( x+d, y, z);
		glVertex3f( x+d, y, z-d);
		glVertex3f( x+d, y+d, z-d);
		glVertex3f( x+d, y+d, z);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f( x, y, z);
		glVertex3f( x+d, y, z);
		glVertex3f( x+d, y+d, z);
		glVertex3f( x, y+d, z);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f( x, y, z);
		glVertex3f( x, y+d, z);
		glVertex3f( x, y+d, z-d);
		glVertex3f( x, y, z-d);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f( x, y+d, z-d);
		glVertex3f( x, y+d, z);
		glVertex3f( x+d, y+d, z);
		glVertex3f( x+d, y+d, z-d);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f( x, y, z);
		glVertex3f( x, y, z-d);
		glVertex3f( x+d, y, z-d);
		glVertex3f( x+d, y, z);
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
		glVertex3f( x+d, y+d, z-d);
		glVertex3f( x+d, y+d, z);
		glVertex3f( x, y+d, z);
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex3f( x, y, z);
		glVertex3f( x+d, y, z);
		glVertex3f( x+d, y, z-d);
		glVertex3f( x, y, z-d);
	glEnd();
}

void boundingBoxOfTriangle( Point tp1, Point tp2, Point tp3){
	float xmin,ymin,zmin, xmax,ymax,zmax;

	xmin=std::min( tp1.x, std::min(tp2.x, tp3.x));
	ymin=std::min( tp1.y, std::min(tp2.y, tp3.y));
	zmin=std::min( tp1.z, std::min(tp2.z, tp3.z));

	xmax=std::max( tp1.x, std::max(tp2.x, tp3.x));
	ymax=std::max( tp1.y, std::max(tp2.y, tp3.y));
	zmax=std::max( tp1.z, std::max(tp2.z, tp3.z));

    if (xmin>0) xmin = int( abs(xmin/d) ) *d - d;
	else if (xmin<0) xmin = -int( abs(xmin/d)) *d - d;

	if (ymin>0) ymin = int( abs(ymin/d) ) *d - d;
	else if (ymin<0) ymin = -int( abs(ymin/d) ) *d -d;

	if (zmin>0) zmin = int( abs(zmin/d) ) *d - d;
	else if (zmin<0) zmin = -int( abs(zmin/d) ) *d - d;


	if (xmax>0)	xmax = int( abs(xmax/d) ) *d + d;
	else if (xmax<0) xmax = -int( abs(xmax/d) ) *d + d;

	if (ymax>0) ymax = int( abs(ymax/d) ) *d + d;
	else if (ymax<0) ymax = -int( abs(ymax/d) ) *d + d;

	if (zmax>0) zmax = int( abs(zmax/d) ) *d + d;
	else if (zmax<0) zmax = -int( abs(zmax/d) ) *d + d;
	
	bound_box1.insert( xmin, ymin, zmax);
	bound_box2.insert( xmax, ymin, zmax);
	bound_box3.insert( xmax, ymax, zmax);
	bound_box4.insert( xmin, ymax, zmax);

	bound_box5.insert( xmin, ymin, zmin);
	bound_box6.insert( xmax, ymin, zmin);
	bound_box7.insert( xmax, ymax, zmin);
	bound_box8.insert( xmin, ymax, zmin);

	width_x = abs(xmax-xmin);
	width_y = abs(ymax-ymin);
	width_z = abs(zmax-zmin);

	//glColor3f( 0,1,0);

	//glBegin(GL_LINE_STRIP);
	//glVertex3f( cp1.x, cp1.y,cp1.z);
	//glVertex3f( cp2.x, cp2.y,cp2.z);
	//glVertex3f( cp3.x, cp3.y,cp3.z);
	//glVertex3f( cp4.x, cp4.y,cp4.z);
	//glVertex3f( cp1.x, cp1.y,cp1.z);
	//glEnd();

	//glBegin(GL_LINE_STRIP);
	//glVertex3f( cp5.x, cp5.y,cp5.z);
	//glVertex3f( cp6.x, cp6.y,cp6.z);
	//glVertex3f( cp7.x, cp7.y,cp7.z);
	//glVertex3f( cp8.x, cp8.y,cp8.z);
	//glVertex3f( cp5.x, cp5.y,cp5.z);
	//glEnd();

	//printf("widths %f %f %f\n", width_x, width_y, width_z);
	//printf("voxel point cp2 %f %f %f zmin=%f \n", cp2.x, cp2.y, cp2.z, zmin);
	//printf("voxel point cp6 %f %f %f zmax=%f \n", cp6.x, cp6.y, cp6.z, zmax);
}

void triangleVoxelization( Point tp1, Point tp2, Point tp3, Point normal){
	// every triangle has its bounding box which consists of potential voxels
	// this function defines which voxels are on the triangle plane => so they should be rendered
	// -----------------
	// calculate dot products between each vector tp1 (this could be tp2 or tp3) to the potential-voxel points (this is 8 vectors) and normal vector
	// if these dot products have the same sign, then this cube is not a voxel because it is above or below the triangle 
	// otherwise, it's upon the triangle so it should be rendered

	Point p;   // this is the point (xmin,ymin,zmax) of every potential-voxel that is being checked
	float dotpr1,dotpr2,dotpr3,dotpr4,dotpr5,dotpr6,dotpr7,dotpr8; // dot products between a->p(i) vectors (i=0,1,2,...,8) and normal
	Point ap1,ap2,ap3,ap4,ap5,ap6,ap7,ap8;		// vectors from triangle point to the cube points that have to be checked if they're above or below the triangle
	Point cube1,cube2,cube3,cube4,cube5,cube6,cube7,cube8;   // the point (xmin,ymin,zmax) of the voxel to be rendered
	
	for (float i=0; i<=(int(width_x/d)); i++){
		for (float j=0; j<=(int(width_y/d)); j++){
			for (float k=0; k<=(int(width_z/d)); k++){	

				cube1.insert( bound_box1.x+i*d, bound_box1.y+j*d, bound_box1.z-k*d);
				ap1.insert( tp1.x-cube1.x, tp1.y-cube1.y, tp1.z-cube1.z);   //vector a->p1
				dotpr1 = ap1.dotproduct( normal);

				cube2.insert( cube1.x+d, cube1.y, cube1.z);
				ap2.insert( tp1.x-cube2.x, tp1.y-cube2.y, tp1.z-cube2.z);  //vector a->p2
				dotpr2 = ap2.dotproduct( normal);
				
				cube3.insert( cube2.x, cube2.y+d, cube2.z);
				ap3.insert( tp1.x-cube3.x, tp1.y-cube3.y, tp1.z-cube3.z);  //vector a->p3
				dotpr3 = ap3.dotproduct( normal);
				
				cube4.insert( cube3.x-d, cube3.y, cube3.z);
				ap4.insert( tp1.x-cube4.x, tp1.y-cube4.y, tp1.z-cube4.z);  //a->p4
				dotpr4 = ap4.dotproduct( normal);

				cube5.insert( cube4.x, cube4.y-d, cube4.z-d);					
				ap5.insert( tp1.x-cube5.x, tp1.y-cube5.y, tp1.z-cube5.z);  //a->p5
				dotpr5 = ap5.dotproduct( normal);

				cube6.insert( cube5.x, cube5.y+d, cube5.z);
				ap6.insert( tp1.x-cube6.x, tp1.y-cube6.y, tp1.z-cube6.z);  //a->p6
				dotpr6 = ap6.dotproduct( normal);

				cube7.insert( cube6.x+d, cube6.y, cube6.z);
				ap7.insert( tp1.x-cube7.x, tp1.y-cube7.y, tp1.z-cube7.z);  //a->p7
				dotpr7 = ap7.dotproduct( normal);

				cube8.insert( cube7.x, cube7.y-d, cube7.z);
				ap8.insert( tp1.x-cube8.x, tp1.y-cube8.y, tp1.z-cube8.z);  //a->p8
				dotpr8 = ap8.dotproduct( normal);			

				if (  !(dotpr1>0 && dotpr2>0 && dotpr3>0 && dotpr4>0 && dotpr5>0 && dotpr6>0 && dotpr7>0 && dotpr8>0) &&
					  !(dotpr1<0 && dotpr2<0 && dotpr3<0 && dotpr4<0 && dotpr5<0 && dotpr6<0 && dotpr7<0 && dotpr8<0) ){

					if ( checkIntersection( tp1, tp2, tp3, normal, cube1) ||
					     checkIntersection( tp1, tp2, tp3, normal, cube2) ||
						 checkIntersection( tp1, tp2, tp3, normal, cube3) ||
					     checkIntersection( tp1, tp2, tp3, normal, cube4) ||
						 checkIntersection( tp1, tp2, tp3, normal, cube5) ||
						 checkIntersection( tp1, tp2, tp3, normal, cube6) ||
						 checkIntersection( tp1, tp2, tp3, normal, cube7) ||
						 checkIntersection( tp1, tp2, tp3, normal, cube8) )		 drawVoxel(cube1);
				}
			}
		}
	}
}

bool checkIntersection( Point tp1, Point tp2, Point tp3, Point normal, Point lp){
	//lp.insert(lp.x+d/2, lp.y+d/2, lp.z-d/2);
	// plane equation: Ax+By+Cz=D
	// compute triangle vectors
	Point ab,bc,ca, ac;
	ab.insert(tp2.x-tp1.x, tp2.y-tp1.y, tp2.z-tp1.z);  //vector tp1->tp2
	bc.insert(tp3.x-tp2.x, tp3.y-tp2.y, tp3.z-tp2.z);  // vector tp2->tp3
	ca.insert(tp1.x-tp3.x, tp1.y-tp3.y, tp1.z-tp3.z);  // vector tp3->tp1
	ac.insert(tp3.x-tp3.x, tp3.y-tp1.y, tp3.z-tp1.z);  //vector tp1->tp2
	// calculate NOT normalized normal
	normal = ab.crossProduct(ac);
	// calculate D
	float D = normal.dotproduct(tp1);
	// line parametric form p+n*t,where p=(p.x,p.y,p.z) and n=triangle normal
	// compute t
	float t= ( D - normal.dotproduct(lp))/ (normal.dotproduct(normal));
	// compute the intersection point 
	if (t<=0) {
		//normal.insert(-normal.x,-normal.y,-normal.z);
		//D = normal.dotproduct(tp1);
		//t = (D- normal.dotproduct(lp))/ normal.dotproduct(normal);
		//normal.insert(-normal.x,-normal.y,-normal.z);
		//return false;
		t=-t;
	}
	Point interp;
	interp.insert( lp.x+t*normal.x, lp.y+t*normal.y, lp.z+t*normal.z);
	// check if the intersection point interp is inside or outside the triangle
	// that means we check if the point is at the right side of the triangle vectors 
	// that's legal if triangles are counter-clockwise
	//-------------------------
	Point ap,bp,cp;
	ap.insert( lp.x-tp1.x, lp.y-tp1.y, lp.z-tp1.z);     //vector tp1->lp
	Point n1;
	n1 = ab.crossProduct(ap);
	float dotpr0 = normal.dotproduct(n1);
	if (dotpr0<0) return false;  // lp is on the right side 

	bp.insert( lp.x-tp2.x, lp.y-tp2.y, lp.z-tp2.z);
	Point n2;
	n2 = bc.crossProduct(bp);
	float dotpr1 = normal.dotproduct(n2);
	if (dotpr1<0) return false;  // lp is on the right side
		
	cp.insert( lp.x-tp3.x, lp.y-tp3.y, lp.z-tp3.z);
	Point n3;
	n3 = ca.crossProduct(cp);
	float dotpr2 = normal.dotproduct(n3);
	if (dotpr2<0) return false;  // lp is on the right side
	
	return true;

}

