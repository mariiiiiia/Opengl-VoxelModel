#include <stdio.h>     // - Just for some ASCII messages
#include <vector>
#include <math.h>
#include "gl/glut.h"   // - An interface and windows management library
#include "structs.h"
#include "histograms.h"
#include "voxelModel.h"
#include "visuals.h"


static std::vector< float> minAngles;
static std::vector< float> minNewAngles;

void triangleAngleHistogram( std::vector< Triangle> newTriangles, std::vector< Point> newVertices, std::vector< Triangle> triangles, std::vector< Point> vertices ){
	Vector ab,bc,ca, ac,ba,cb;		//triangle edge vectors
	Point a,b,c;					//triangle points

	for (int i=0; i<int(triangles.size()); i++){
		a.insert( vertices.at(triangles.at(i).p1));
		b.insert( vertices.at(triangles.at(i).p2));
		c.insert( vertices.at(triangles.at(i).p3));
		
		ab.insert( b.x-a.x, b.y-a.y, b.z-a.z);
		bc.insert( c.x-b.x, c.y-b.y, c.z-b.z);
		ca.insert( a.x-c.x, a.y-c.y, a.z-c.z);
		ba.insert( -ab.x, -ab.y, -ab.z);
		cb.insert( -bc.x, -bc.y, -bc.z);
		ac.insert( -ca.x, -ca.y, -ca.z);

		float distab = sqrt( pow( ab.x,2) + pow( ab.y,2) + pow(ab.z,2));
		float distbc = sqrt( pow( bc.x,2) + pow( bc.y,2) + pow(bc.z,2));
		float distca = sqrt( pow( ca.x,2) + pow( ca.y,2) + pow(ca.z,2));

		float abac = ab.dotproduct(ac);
		float cacb = ca.dotproduct(cb);
		float babc = ba.dotproduct(bc);

		float cos1 = abac/(distab*distca);
		float cos2 = cacb/(distca*distbc);
		float cos3 = babc/(distab*distbc);

		float cosangle = std::min( cos2, std::min( cos2, cos3));
		float tri_angle = acos(cosangle);
		minAngles.push_back( tri_angle);
	}

	for (int i=0; i<int(newTriangles.size()); i++){
		a.insert( newVertices.at(newTriangles.at(i).p1));
		b.insert( newVertices.at(newTriangles.at(i).p2));
		c.insert( newVertices.at(newTriangles.at(i).p3));
		
		ab.insert( b.x-a.x, b.y-a.y, b.z-a.z);
		bc.insert( c.x-b.x, c.y-b.y, c.z-b.z);
		ca.insert( a.x-c.x, a.y-c.y, a.z-c.z);
		ba.insert( -ab.x, -ab.y, -ab.z);
		cb.insert( -bc.x, -bc.y, -bc.z);
		ac.insert( -ca.x, -ca.y, -ca.z);

		float distab = sqrt( pow( ab.x,2) + pow( ab.y,2) + pow(ab.z,2));
		float distbc = sqrt( pow( bc.x,2) + pow( bc.y,2) + pow(bc.z,2));
		float distca = sqrt( pow( ca.x,2) + pow( ca.y,2) + pow(ca.z,2));

		float abac = ab.dotproduct(ac);
		float cacb = ca.dotproduct(cb);
		float babc = ba.dotproduct(bc);

		float cos1 = abac/(distab*distca);
		float cos2 = cacb/(distca*distbc);
		float cos3 = babc/(distab*distbc);

		float cosangle = std::min( cos2, std::min( cos2, cos3));
		float tri_angle = acos(cosangle);
		minNewAngles.push_back( tri_angle);
	}

}


void drawHistogram(){
	//CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
														   // and the depth buffer
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();

	// y axe
	glColor3f(0,0,1);
	
	glBegin(GL_LINES);
	glVertex3f( -40,-20,-40);
	glVertex3f( -40,20,-40);
	glEnd();

	glColor3f( 1,0,0);
	double x0=LEFT_WALL;
	double x1=RIGHT_WALL;

	double size = double(minAngles.size());
	double dx = (abs(x0)+abs(x1))/size;
	for(int i=0; i<int(minAngles.size()); i++){
		glBegin( GL_LINES);
		glVertex3f( x0, FLOOR-10, -40);
		glVertex3f( x0, FLOOR-10+abs(minAngles.at(i))*3, -40);
		glEnd();
		x0+=dx;
	}


	//glBegin( GL_LINE_LOOP);
	//for (int i=0; i<int(minAngles.size()); i++){
	//	glVertex3f( x0, FLOOR-10+abs(minAngles.at(i))*2, -40);
	//	x0+=dx;
	//}
	//glEnd();

	glColor3f(0,1,0); 

	x0=LEFT_WALL;
	size = double(minNewAngles.size());
	dx = (abs(x0)+abs(x1))/size;
	for(int i=0; i<int(minNewAngles.size()); i++){
		glBegin( GL_LINES);
		glVertex3f( x0, FLOOR+10, -40);
		glVertex3f( x0, FLOOR+10+abs(minNewAngles.at(i))*3, -40);
		glEnd();
		x0+=dx;
	}



	//glBegin( GL_LINE_LOOP);
	//for (int i=0; i<int(minNewAngles.size()); i++){
	//	glVertex3f( x0, FLOOR+10+abs(minNewAngles.at(i))*2, -40);
	//	x0+=dx;
	//}
	//glEnd();
	
	glColor3f( 1,1,1);
	float dy=0.0;
	for(int i=0; i<16; i++){
		glBegin( GL_LINES);
		glVertex3f( -40, -20+dy,-40);
		glVertex3f( -39.5, -20+dy,-40);
		glEnd();
		dy+=2.5;
	}

	glutSwapBuffers();
}