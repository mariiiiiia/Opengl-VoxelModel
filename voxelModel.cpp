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
		//p3.insert( 0.3,1.5,1.8);

		//glBegin( GL_TRIANGLES);
		//glVertex3f( p1.x,p1.y,p1.z); glVertex3f( p2.x,p2.y,p2.z); glVertex3f(p3.x, p3.y,p3.z);
		//glEnd();

		boundingBoxOfTriangle(p1,p2,p3);

		//Point norm;
		//norm.insert(0,0,1);
		triangleVoxelization(p1,p2,p3, normal.at(k));

		//printf( "voxel array size: %d\n", voxels.size());

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
		glVertex3f( x+d, y, z-d);
		glVertex3f( x+d, y+d, z-d);
		glVertex3f( x, y+d, z-d);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f( x+d, y, z-d);
		glVertex3f( x+d, y, z);
		glVertex3f( x+d, y+d, z);
		glVertex3f( x+d, y+d, z-d);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f( x, y, z);
		glVertex3f( x, y+d, z);
		glVertex3f( x+d, y+d, z);
		glVertex3f( x+d, y, z);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f( x, y, z-d);
		glVertex3f( x, y+d, z-d);
		glVertex3f( x, y+d, z);
		glVertex3f( x, y, z);
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

	cp1.x= xmin; cp1.y= ymin; cp1.z= zmax;
	cp2.x= xmax; cp2.y= ymin; cp2.z= zmax;
	cp3.x= xmax; cp3.y= ymax; cp3.z= zmax;
	cp4.x= xmin; cp4.y= ymax; cp4.z= zmax;

	cp5.x= xmin; cp5.y= ymin; cp5.z= zmin;
	cp6.x= xmax; cp6.y= ymin; cp6.z= zmin;
	cp7.x= xmax; cp7.y= ymax; cp7.z= zmin;
	cp8.x= xmin; cp8.y= ymax; cp8.z= zmin;

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
	Point cp;   // the point (xmin,ymin,zmax) of the voxel 
	
	for (float i=0; i<=(int(width_x/d)); i++){
		for (float j=0; j<=(int(width_y/d)); j++){
			for (float k=0; k<=(int(width_z/d)); k++){	

				p.insert( cp1.x+i*d, cp1.y+j*d, cp1.z-k*d);
				ap1.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);
				dotpr1 = ap1.dotproduct( normal);

				p.insert( p.x+d, p.y, p.z);
				ap2.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);
				dotpr2 = ap2.dotproduct( normal);
				
				p.insert( p.x, p.y+d, p.z);
				ap3.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);
				dotpr3 = ap3.dotproduct( normal);
				
				p.insert( p.x-d, p.y, p.z);
				ap4.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);
				dotpr4 = ap4.dotproduct( normal);

				p.insert( p.x, p.y-d, p.z-d);
				ap5.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);
				dotpr5 = ap5.dotproduct( normal);

				p.insert( p.x+d, p.y, p.z);
				ap6.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);
				dotpr6 = ap6.dotproduct( normal);

				p.insert( p.x, p.y+d, p.z);
				ap7.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);
				dotpr7 = ap7.dotproduct( normal);

				p.insert( p.x-d, p.y, p.z);
				ap8.insert( tp1.x-p.x, tp1.y-p.y, tp1.z-p.z);
				dotpr8 = ap8.dotproduct( normal);			

				if (  !(dotpr1>0 && dotpr2>0 && dotpr3>0 && dotpr4>0 && dotpr5>0 && dotpr6>0 && dotpr7>0 && dotpr8>0) &&
					  !(dotpr1<0 && dotpr2<0 && dotpr3<0 && dotpr4<0 && dotpr5<0 && dotpr6<0 && dotpr7<0 && dotpr8<0) ){
						/*	printf("no voxel\n");}
				else {*/
					//printf("voxel\n");

					//Point vp;				
					//vp.insert( int(abs((cp1.x+i*d)/d)), int(abs((cp1.y+j*d)/d)), int(abs((cp1.z-k*d)/d)) );		//voxel distance from (0,0,0) to my point 
					//vp.insert( vp.x*d, vp.y*d, vp.z*d);  // real distance of each voxel
					
					cp.insert( cp1.x+i*d, cp1.y+j*d, cp1.z-k*d);

					drawVoxel(cp);
					//placeVoxel( cp, cp);
				}
			}
		}
	}
}

//void barycentricCoord( Point p1, Point p2, Point p3, Point p){
//        // calculate the barycentric coordinates 
//
//        d = (p2y - p3y)*(p1x - p3x) + (p3x - p2x)*(p1y - p3y)
//        if d!=0:
//            alpha = ((p2y - p3y)*(px - p3x) + (p3x - p2x)*(py - p3y)) / ((p2y - p3y)*(p1x - p3x) + (p3x - p2x)*(p1y - p3y))
//            beta = ((p3y - p1y)*(px - p3x) + (p1x - p3x)*(py - p3y)) / ((p2y - p3y)*(p1x - p3x) + (p3x - p2x)*(p1y - p3y)) 
//            gamma = 1 - alpha - beta
//            return [alpha, beta, gamma]
//        else:
//            return None
//}

