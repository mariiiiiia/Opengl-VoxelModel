#include <stdio.h>     // - Just for some ASCII messages
#include <array>
#include <string>
#include <algorithm>
#include <vector>
#include <math.h>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "visuals.h"   // Header file for our OpenGL functions
#include "Load_obj.h"
#include "Show_Obj.h"


void showObj(std::vector<Point> &vert, std::vector<Triangle> &tr, bool solid,bool wireframe, std::vector<Point> normal, int texture)
{
	// variables for spherical transformations - for the body
	Point spheric_coord1;
	Point spheric_coord2;
	Point spheric_coord3;
	float theta1,phi1,theta2,phi2,theta3,phi3;

	if (solid){														// solid rendering
		for (int i=0; i<tr.size(); i++)								
		{
			if (texture!= NULL){				// body texture
				glColor4f(0.5,0.5,0.6,1.0);

				spheric_coord1 = sphericCoord(vert.at(tr.at(i).p1));
				theta1 = spheric_coord1.y; phi1 = spheric_coord1.z;
			
				spheric_coord2 = sphericCoord(vert.at(tr.at(i).p2));
				theta2 = spheric_coord2.y; phi2 = spheric_coord2.z;
			
				spheric_coord3 = sphericCoord(vert.at(tr.at(i).p3));
				theta3 = spheric_coord3.y; phi3 = spheric_coord3.z;

				glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glBindTexture(GL_TEXTURE_2D, texture);


				glBegin(GL_TRIANGLES);
					 if (obj_file=="objects/unicorn_low.obj" && tr.size()<300) glTexCoord2f( phi1/(2*3.14), -vert.at(tr.at(i).p1).z-4 );	// wrap around cone
					 else glTexCoord2f( theta1/(2*3.14), (vert.at(tr.at(i).p1).y+30)/60);				// wrap around a cylinder
				     glNormal3f(vertNormal.at(tr.at(i).p1).x, vertNormal.at(tr.at(i).p1).y, vertNormal.at(tr.at(i).p1).z);
// 					 glTexCoord2f( phi1/(2*3.14), theta1/3.14);											// wrap around a sphere
					 glVertex3f( vert.at(tr.at(i).p1).x, vert.at(tr.at(i).p1).y, vert.at(tr.at(i).p1).z);
				   
					 if (obj_file=="objects/unicorn_low.obj" && tr.size()<300) glTexCoord2f( phi2/(2*3.14), -vert.at(tr.at(i).p2).z-4 );	// wrap around cone
					 else glTexCoord2f( theta2/(2*3.14), (vert.at(tr.at(i).p2).y+30)/60);				// wrap around a cylinder
					 glNormal3f(vertNormal.at(tr.at(i).p2).x, vertNormal.at(tr.at(i).p2).y, vertNormal.at(tr.at(i).p2).z);			 					 
//					 glTexCoord2f( phi2/(2*3.14), theta2/3.14); 
					 glVertex3f( vert.at(tr.at(i).p2).x, vert.at(tr.at(i).p2).y, vert.at(tr.at(i).p2).z);

					 if (obj_file=="objects/unicorn_low.obj" && tr.size()<300) glTexCoord2f( phi3/(2*3.14), -vert.at(tr.at(i).p3).z-4 );	// wrap around cone
  					 else glTexCoord2f( theta3/(2*3.14), (vert.at(tr.at(i).p3).y+30)/60);				// wrap around a cylinder
					 glNormal3f(vertNormal.at(tr.at(i).p3).x, vertNormal.at(tr.at(i).p3).y, vertNormal.at(tr.at(i).p3).z); 
// 					 glTexCoord2f( phi3/(2*3.14),theta3/3.14); 
					 glVertex3f( vert.at(tr.at(i).p3).x, vert.at(tr.at(i).p3).y, vert.at(tr.at(i).p3).z);
				glEnd();

				glDisable(GL_TEXTURE_2D);
			}
			else{																	// without texture solid
				glBegin(GL_TRIANGLES);
					glColor4f(0.5,0.5,0.6,1.0);

					glNormal3f(vertNormal.at(tr.at(i).p1).x, vertNormal.at(tr.at(i).p1).y, vertNormal.at(tr.at(i).p1).z);
					glVertex3f( vert.at(tr.at(i).p1).x, vert.at(tr.at(i).p1).y, vert.at(tr.at(i).p1).z);

					glNormal3f(vertNormal.at(tr.at(i).p2).x, vertNormal.at(tr.at(i).p2).y, vertNormal.at(tr.at(i).p2).z);
					glVertex3f( vert.at(tr.at(i).p2).x, vert.at(tr.at(i).p2).y, vert.at(tr.at(i).p2).z);	

					glNormal3f(vertNormal.at(tr.at(i).p3).x, vertNormal.at(tr.at(i).p3).y, vertNormal.at(tr.at(i).p3).z);
					glVertex3f( vert.at(tr.at(i).p3).x, vert.at(tr.at(i).p3).y, vert.at(tr.at(i).p3).z);

				glEnd();
			}
		}
	}
	else if (wireframe){												// wireframe rendering
		for (int i=0; i<tr.size(); i++)
		{
			glBegin(GL_LINES);
				glNormal3f(normal.at(i).x, normal.at(i).y, normal.at(i).z);
				glVertex3f( vert.at(tr.at(i).p1).x, vert.at(tr.at(i).p1).y, vert.at(tr.at(i).p1).z);
				glNormal3f(normal.at(i).x, normal.at(i).y, normal.at(i).z);
				glVertex3f( vert.at(tr.at(i).p2).x, vert.at(tr.at(i).p2).y, vert.at(tr.at(i).p2).z);
				glNormal3f(normal.at(i).x, normal.at(i).y, normal.at(i).z);
				glVertex3f( vert.at(tr.at(i).p3).x, vert.at(tr.at(i).p3).y, vert.at(tr.at(i).p3).z);
			glEnd();
		}
	}
}

Point sphericCoord(Point v)
{
	float r, theta, phi;
	Point sph_coord;

	r = sqrt( pow(v.x,2) + pow(v.y,2) + pow(v.z,2));
	theta = acos(v.z/r);								// theta and phi
//	phi = atan(v.y/v.x);
	if (v.y>=0) phi = acos(v.x/(r * sin(theta))) ;
	else		phi = (3.14 + acos(v.x/(r * sin(theta)))) ;

	sph_coord.x=r, sph_coord.y=theta, sph_coord.z=phi;

	return sph_coord;
}

