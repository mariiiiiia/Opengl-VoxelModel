#include <stdio.h>     // - Just for some ASCII messages
#include <string>
#include <array>
#include <vector>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "visuals.h"  // for the Point and Triangle structures only
#include "voxelModel.h"


std::vector<Point> voxels;

Point cp1,cp2,cp3,cp4,cp5,cp6,cp7,cp8;  // points that define the bounding box, where i search for the voxels that should be drawn
float width_x, width_y, width_z;        // widths of bounding box

float d=0.2;		// voxel width

//int w=0, b=1;

void voxelModel(const std::vector<Point> &vert,const std::vector<Triangle> &tr, const std::vector<Point> &normal){
	float x,y,z;

	voxels.clear();
	//glColor4f( 1,0,0.5, 1.0);
	//for (int k=0; k<1; k++){
	for (int k=0; k<tr.size(); k++){
		Point p1 = vert.at(tr.at(k).p1);
		Point p2 = vert.at(tr.at(k).p2);
		Point p3 = vert.at(tr.at(k).p3);

		//Point p1,p2,p3;
		//p1.insert( 0,1,0);
		//p2.insert( 0, 3,0);
		//p3.insert( 0.6,1.5,1.8);

		//glBegin( GL_TRIANGLES);
		//glVertex3f( p1.x,p1.y,p1.z); glVertex3f( p2.x,p2.y,p2.z); glVertex3f(p3.x, p3.y,p3.z);
		//glEnd();

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
		glVertex3f( x+d, y+d, z-d);
		glVertex3f( x+d, y+d, z);
		glVertex3f( x, y+d, z);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f( x, y, z);
		glVertex3f( x+d, y, z);
		glVertex3f( x+d, y, z-d);
		glVertex3f( x, y, z-d);
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

void placeVoxel( Point p1, Point vp){
	if (p1.x>=0 && p1.y>=0 && p1.z>=0) drawVoxel( vp);
	//else if (p1.x>=0 && p1.y>=0 && p1.z<0) drawVoxel( vp.x, vp.y,-vp.z);
	//else if (p1.x>=0 && p1.y<0 && p1.z>=0) drawVoxel(vp.x,-vp.y,vp.z);
	/*else if (p1.x<0 && p1.y>=0 && p1.z>=0) drawVoxel(-vp.x,vp.y,vp.z);
	else if (p1.x>=0 && p1.y<0 && p1.z<0) drawVoxel(vp.x,-vp.y,-vp.z);
	else if (p1.x<0 && p1.y<0 && p1.z>=0) drawVoxel(-vp.x,-vp.y,vp.z);
	else if (p1.x<0 && p1.y>=0 && p1.z<0) drawVoxel(-vp.x,vp.y,-vp.z);
	else if (p1.x<0 && p1.y<0 && p1.z<0) drawVoxel(-vp.x,-vp.y,-vp.z);*/
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
	
	cp1.insert( xmin, ymin, zmax);
	cp2.insert( xmax, ymin, zmax);
	cp3.insert( xmax, ymax, zmax);
	cp4.insert( xmin, ymax, zmax);

	cp5.insert( xmin, ymin, zmin);
	cp6.insert( xmax, ymin, zmin);
	cp7.insert( xmax, ymax, zmin);
	cp8.insert( xmin, ymax, zmin);

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
	float dotpr1,dotpr2,dotpr3,dotpr4,dotpr5,dotpr6,dotpr7,dotpr8; // dot products 
	Point ap1,ap2,ap3,ap4,ap5,ap6,ap7,ap8;		// vectors from triangle point to the cube points that have to be checked if they're above or below the triangle
	Point cp;   // the point (xmin,ymin,zmax) of the voxel to be rendered
	
	for (float i=0; i<=(int(width_x/d)); i++){
		for (float j=0; j<=(int(width_y/d)); j++){
			for (float k=0; k<=(int(width_z/d)); k++){	

				p.insert( cp1.x+i*d, cp1.y+j*d, cp1.z-k*d);
				ap1.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);   //vector a->p1
				dotpr1 = ap1.dotproduct( normal);

				p.insert( p.x+d, p.y, p.z);
				ap2.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);  //vector a->p2
				dotpr2 = ap2.dotproduct( normal);
				
				p.insert( p.x, p.y+d, p.z);
				ap3.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);  //vector a->p3
				dotpr3 = ap3.dotproduct( normal);
				
				p.insert( p.x-d, p.y, p.z);
				ap4.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);  //a->p4
				dotpr4 = ap4.dotproduct( normal);

				p.insert( p.x, p.y-d, p.z-d);					
				ap5.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);  //a->p5
				dotpr5 = ap5.dotproduct( normal);

				p.insert( p.x, p.y+d, p.z);
				ap6.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);  //a->p6
				dotpr6 = ap6.dotproduct( normal);

				p.insert( p.x+d, p.y, p.z);
				ap7.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);  //a->p7
				dotpr7 = ap7.dotproduct( normal);

				p.insert( p.x, p.y-d, p.z);
				ap8.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);  //a->p8
				dotpr8 = ap8.dotproduct( normal);			


				if (  !(dotpr1>0 && dotpr2>0 && dotpr3>0 && dotpr4>0 && dotpr5>0 && dotpr6>0 && dotpr7>0 && dotpr8>0) &&
					  !(dotpr1<0 && dotpr2<0 && dotpr3<0 && dotpr4<0 && dotpr5<0 && dotpr6<0 && dotpr7<0 && dotpr8<0) ){

					cp.insert( cp1.x+i*d, cp1.y+j*d, cp1.z-k*d);
					if (checkIntersection( tp1, tp2, tp3, normal, cp))	drawVoxel(cp);
				}
			}
		}
	}
}

bool checkIntersection( Point tp1, Point tp2, Point tp3, Point normal, Point lp){
	lp.insert(lp.x+d/2, lp.y+d/2, lp.z-d/2);
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
	else if (t==0) return true;
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
	//----------------------------
	//Point p1,p2;
	//p1.insert( interp.x-tp1.x, interp.y-tp1.y, interp.z-tp1.z);
	//Point s,r;
	//s.x = p1.crossProduct(tp2).x/ tp1.crossProduct(tp2).x;
	//s.y = p1.crossProduct(tp2).y/ tp1.crossProduct(tp2).y;
	//s.z = p1.crossProduct(tp2).z/ tp1.crossProduct(tp2).z;
	//p2.insert( interp.x-tp2.x, interp.y-tp1.y, interp.z-tp1.z);
 //   r.x = tp1.crossProduct(p2).x/ tp1.crossProduct(tp2).x;
	//r.y = tp1.crossProduct(p2).y/ tp1.crossProduct(tp2).y;
	//r.z = tp1.crossProduct(p2).z/ tp1.crossProduct(tp2).z;

	//Point zero, ace;
	//ace.insert(1,1,1);
	//zero.insert(0,0,0);
	//Point sr;
	//sr.insert( s.x+r.x, s.y+r.y, s.z+r.z);
	//if ( (s.comparisonGreater(zero) || s.equals(zero)) && (r.comparisonGreater(zero) || r.equals(zero)) && (sr.comparisonLower(zero) || sr.equals(ace))) return true;
	//else return false;
	//------------------------------
	
//	Point ab,ac,bc,ca,ba,cb;
//	ab.insert( tp2.x-tp1.x, tp2.y-tp1.y, tp2.z-tp1.z);  //vector tp1->tp2
//	ac.insert( tp3.x-tp1.x, tp3.y-tp1.y, tp3.z-tp1.z);
//	bc.insert( tp3.x-tp2.x, tp3.y-tp2.y, tp3.z-tp2.z);
//	ca.insert(-ac.x,-ac.y,-ac.z);
//	ba.insert( -ab.x,-ab.y,-ab.z);
//	cb.insert(-bc.x,-bc.y,-bc.z);
//
//	//Point normal;
//	normal.insert(ba.crossProduct(ca).x, ca.crossProduct(ca).y, ba.crossProduct(ca).z);
//
//	Point ap;
//	ap.insert( interp.x-tp1.x, interp.y-tp1.y, interp.z-tp1.z);
//	Point crosspr1;
//	crosspr1 = ba.crossProduct(ap);
//	float vnum= normal.dotproduct(crosspr1);
//	if (vnum<0) return true;
//
//	Point bp;
////	ab.insert(tp2.x-tp1.x, tp2.y-tp1.y, tp2.z-tp1.z);  //vector tp1->tp2
//	bp.insert( interp.x-tp2.x, interp.y-tp2.y, interp.z-tp2.z);
//	crosspr1 = cb.crossProduct(bp);
//	vnum= normal.dotproduct(crosspr1);
//	if (vnum<0) return true;
//
//	Point cp;
//	cp.insert( interp.x-tp3.x, interp.y-tp3.y, interp.z-tp3.z);
//	crosspr1 = ac.crossProduct(cp);
//	vnum= normal.dotproduct(crosspr1);
//	if (vnum<0) return true;
//	else return false;



}

