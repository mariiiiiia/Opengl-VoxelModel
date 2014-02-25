#include <stdio.h>     // - Just for some ASCII messages
#include <string>
#include <array>
#include <vector>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "structs.h"
#include "voxelModel.h"
#include "counter.h"


Point bound_box1;		// points that define the bounding box, where i search for the voxels that should be drawn
static float width_x, width_y, width_z;        // widths of bounding box

void VoxelModel::setVoxels( const std::vector<Point> vert, const std::vector<Triangle> tr, const std::vector<Vector> normal){   
	std::vector< Point> bound_box;

	//voxels.clear();
	
	if (int(this->voxels.size())==0 ){
		if (this->voxel_width==NULL) this->voxel_width = this->setUpVoxelWidth();

		for (int k=0; k<int(tr.size()); k++){
			Point p1 = vert.at(tr.at(k).p1);
			Point p2 = vert.at(tr.at(k).p2);
			Point p3 = vert.at(tr.at(k).p3);

			this->boundingBoxOfTriangle(p1,p2,p3, this->voxel_width);
			this->triangleVoxelization(vert, tr.at(k), normal.at(k), this->voxel_width);
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
		printf("voxel quantity: %d\n", this->voxels.size());
	}
}

//#define GL_SQUARE(x, y, z, d)  do { \
//	glBegin(GL_QUADS); \
//			glVertex3f( x, y, z-d); \
//			glVertex3f( x, y+d, z-d); \
//			glVertex3f( x+d, y+d, z-d); \
//			glVertex3f( x+d, y, z-d); \
//			glEnd(); } while(0)

// GL_SQUARE(x, y, z, d);
void VoxelModel::drawVoxel(){
	glEnable(GL_CULL_FACE);
	float d = this->voxel_width;
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

		glColor4f(0.8,0.8,0.8,0.5);
		

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

    if (xmin>0) xmin = int( xmin/d ) *d;
	else if (xmin<0) xmin = -int( abs(xmin/d)) *d - d;

	if (ymin>0) ymin = int( ymin/d ) *d;
	else if (ymin<0) ymin = -int( abs(ymin/d) ) *d -d;

	if (zmin>0) zmin = int( zmin/d ) *d;
	else if (zmin<0) zmin = -int( abs(zmin/d) ) *d -d;


	if (xmax>0)	xmax = int( xmax/d ) *d + d;
	else if (xmax<0) xmax = -int( abs(xmax/d) ) *d;

	if (ymax>0) ymax = int( ymax/d ) *d + d;
	else if (ymax<0) ymax = -int( abs(ymax/d) ) *d;

	if (zmax>0) zmax = int( zmax/d ) *d + d;
	else if (zmax<0) zmax = -int( abs(zmax/d) ) *d;
	
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

	float dotpr1,dotpr2,dotpr3,dotpr4,dotpr5,dotpr6,dotpr7,dotpr8; // dot products between tp->cube(i) vectors (i=0,1,2,...,8) and normal
	Vector ap1,ap2,ap3,ap4,ap5,ap6,ap7,ap8;			// vectors from the triangle points to cube points that have to be checked if they're above or below the triangle)
	Point cube0,cube1,cube2,cube3,cube4,cube5,cube6,cube7;   // the point (xmin,ymin,zmax) of the voxel to be rendered
	Point cp;
	

	for (float i=0; i<=(int(width_x/d)); i++){
		for (float j=0; j<=(int(width_y/d)); j++){
			for (float k=0; k<=(int(width_z/d)); k++){	

				cube0.insert( bound_box1.x+i*d, bound_box1.y+j*d, bound_box1.z-k*d);
				ap1.insert( -tp1.x+cube0.x,-tp1.y+cube0.y,-tp1.z+cube0.z);  //vector a->p1
				dotpr1 = normal.dotproduct( ap1);

				cube1.insert( cube0.x+d, cube0.y, cube0.z);
				ap2.insert(-tp1.x+cube1.x,-tp1.y+cube1.y,-tp1.z+cube1.z);  //vector a->p2
				dotpr2 = normal.dotproduct( ap2);
				
				cube2.insert( cube1.x, cube1.y+d, cube1.z);
				ap3.insert(-tp1.x+cube2.x,-tp1.y+cube2.y,-tp1.z+cube2.z);  //vector a->p3
				dotpr3 = normal.dotproduct( ap3);
				
				cube3.insert( cube2.x-d, cube2.y, cube2.z);
				ap4.insert(-tp1.x+cube3.x,-tp1.y+cube3.y,-tp1.z+cube3.z);  //a->p4
				dotpr4 = normal.dotproduct( ap4);

				cube4.insert( cube3.x, cube3.y-d, cube3.z-d);					
				ap5.insert(-tp1.x+cube4.x,-tp1.y+cube4.y,-tp1.z+cube4.z);  //a->p5
				dotpr5 = normal.dotproduct( ap5);

				cube5.insert( cube4.x+d, cube4.y, cube4.z);
				ap6.insert(-tp1.x+cube5.x,-tp1.y+cube5.y,-tp1.z+cube5.z);  //a->p6
				dotpr6 = normal.dotproduct( ap6);

				cube6.insert( cube5.x, cube5.y+d, cube5.z);
				ap7.insert(-tp1.x+cube6.x,-tp1.y+cube6.y,-tp1.z+cube6.z);  //a->p7
				dotpr7 = normal.dotproduct( ap7);

				cube7.insert( cube6.x-d, cube6.y, cube6.z);
				ap8.insert(-tp1.x+cube7.x,-tp1.y+cube7.y,-tp1.z+cube7.z);  //a->p8
				dotpr8 = normal.dotproduct( ap8);			

				if (  !(dotpr1>0.0 && dotpr2>0.0 && dotpr3>0.0 && dotpr4>0.0 && dotpr5>0.0 && dotpr6>0.0 && dotpr7>0.0 && dotpr8>0.0) &&
					  !(dotpr1<0.0 && dotpr2<0.0 && dotpr3<0.0 && dotpr4<0.0 && dotpr5<0.0 && dotpr6<0.0 && dotpr7<0.0 && dotpr8<0.0) ){
					// NOW WE KNOW THE CUBE IS AT THE TRIANGLE'S PLANE
					// WE NEED TO CHECK IF THE CUBE IS INSIDE THE TRIANGLE 
					
					Point null;
					std::vector< Point> interpoints;

					cp.insert( cube0.x+d/2, cube0.y+d/2, cube0.z-d/2);
					null.insert( 100,100,100);
					for (int m=0; m<12; m++){
						interpoints.push_back( null);
					}


					bool b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12;
					bool t13,t14,t15;
					b1 = checkLineTriangleIntersection( tri, normal, cube0, cube1, interpoints.at(0));	//edge0
					b2 = checkLineTriangleIntersection( tri, normal, cube1, cube2, interpoints.at(1));	//edge1
					b3 = checkLineTriangleIntersection( tri, normal, cube2, cube3, interpoints.at(2));	//edge2
					b4 = checkLineTriangleIntersection( tri, normal, cube3, cube0, interpoints.at(3));	//edge3
					b5 = checkLineTriangleIntersection( tri, normal, cube4, cube5, interpoints.at(4));	//edge4
					b6 = checkLineTriangleIntersection( tri, normal, cube5, cube6, interpoints.at(5));	//edge5
					b7 = checkLineTriangleIntersection( tri, normal, cube6, cube7, interpoints.at(6));	//edge6 
					b8 = checkLineTriangleIntersection( tri, normal, cube7, cube4, interpoints.at(7));	//edge7
					b9 = checkLineTriangleIntersection( tri, normal, cube0, cube4, interpoints.at(8));	//edge8
					b10= checkLineTriangleIntersection( tri, normal, cube1, cube5, interpoints.at(9));	//edge9 
					b11= checkLineTriangleIntersection( tri, normal, cube2, cube6, interpoints.at(10));	//edge10
					b12= checkLineTriangleIntersection( tri, normal, cube3, cube7, interpoints.at(11));	//edge11
					t13= pointInsideCube( cube0, d, tp1);
					t14= pointInsideCube( cube0, d, tp2);
					t15= pointInsideCube( cube0, d, tp3);

					if ( b1 || b2 || b3 || b4 || b5 || b6 || b7 || b8 || b9 || b10 || b11 || b12 || t13 || t14 || t15){
						//------------ THE CUBE IS INSIDE THE TRIANGLE ---------------------------------
						//===================== INSERT VOXEL TO LIST =====================================//
							 Voxel vox;
							 vox.insert(cube0.x+d/2, cube0.y+d/2, cube0.z-d/2);

							 int vox_pos;
							 if ( !this->voxelAlreadyExists( vox, vox_pos)){
								 this->voxels.push_back(vox);
								 //==================== INSERT MARCHING CUBE TO LIST ================================//
								 MarchingCube march_cube;

								 march_cube.set( vox);
								 this->mc.push_back(march_cube);

								 // corner state is true if it is inside the surface
								 // dotpr(i)<=0 if point(i) is inside the surface
								 for (int g=0; g<8; g++){
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( g, false);
								 }
								 if (b1) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 0, dotpr1<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 1, dotpr2<=0);}
								 if (b2) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 1, dotpr2<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 2, dotpr3<=0);}
								 if (b3) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 2, dotpr3<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 3, dotpr4<=0);}
								 if (b4) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 0, dotpr1<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 3, dotpr4<=0);}
								 if (b5) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 4, dotpr5<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 5, dotpr6<=0);}
								 if (b6) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 5, dotpr6<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 6, dotpr7<=0);}
								 if (b7) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 6, dotpr7<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 7, dotpr8<=0);}
								 if (b8) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 7, dotpr8<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 4, dotpr5<=0);}
								 if (b9) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 0, dotpr1<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 4, dotpr5<=0);}
								 if (b10) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 1, dotpr2<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 5, dotpr6<=0);}
								 if (b11) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 2, dotpr3<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 6, dotpr7<=0);}
								 if (b12) { 
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 7, dotpr8<=0);
									 this->mc.at( voxels.size()-1).turnCornerOnOrOff( 3, dotpr4<=0);}

/*								 if (dotpr1<=0 && pointInsideSurface( tri, normal, cube0)) this->mc.at( voxels.size()-1).turnCornerOnOrOff( 0, true); 
								 else this->mc.at( voxels.size()-1).turnCornerOnOrOff( 0, false);

								 if (dotpr2<=0 && pointInsideSurface( tri, normal, cube1)) this->mc.at( voxels.size()-1).turnCornerOnOrOff( 1, true);
								 else this->mc.at( voxels.size()-1).turnCornerOnOrOff( 1, false);

								 if (dotpr3<=0 && pointInsideSurface( tri, normal, cube2)) this->mc.at( voxels.size()-1).turnCornerOnOrOff( 2, true);
								 else this->mc.at( voxels.size()-1).turnCornerOnOrOff( 2, false);

								 if (dotpr4<=0 && pointInsideSurface( tri, normal, cube3)) this->mc.at( voxels.size()-1).turnCornerOnOrOff( 3, true);
								 else this->mc.at( voxels.size()-1).turnCornerOnOrOff( 3, false);

								 if (dotpr5<=0 && pointInsideSurface( tri, normal, cube4)) this->mc.at( voxels.size()-1).turnCornerOnOrOff( 4, true);
								 else this->mc.at( voxels.size()-1).turnCornerOnOrOff( 4, false);	

								 if (dotpr6<=0 && pointInsideSurface( tri, normal, cube5)) this->mc.at( voxels.size()-1).turnCornerOnOrOff( 5, true);
								 else this->mc.at( voxels.size()-1).turnCornerOnOrOff( 5, false);

								 if (dotpr7<=0 && pointInsideSurface( tri, normal, cube6)) this->mc.at( voxels.size()-1).turnCornerOnOrOff( 6, true);
								 else this->mc.at( voxels.size()-1).turnCornerOnOrOff( 6, false);

								 if (dotpr8<=0 && pointInsideSurface( tri, normal, cube7)) this->mc.at( voxels.size()-1).turnCornerOnOrOff( 7, true);
								 else this->mc.at( voxels.size()-1).turnCornerOnOrOff( 7, false);*/							 
								 //this->mc.at( voxels.size()-1).turnCornerOnOrOff( 0, dotpr1<=0);		
								 //this->mc.at( voxels.size()-1).turnCornerOnOrOff( 1, dotpr2<=0);		
								 //this->mc.at( voxels.size()-1).turnCornerOnOrOff( 2, dotpr3<=0);
								 //this->mc.at( voxels.size()-1).turnCornerOnOrOff( 3, dotpr4<=0);
								 //this->mc.at( voxels.size()-1).turnCornerOnOrOff( 4, dotpr5<=0);
								 //this->mc.at( voxels.size()-1).turnCornerOnOrOff( 5, dotpr6<=0);
								 //this->mc.at( voxels.size()-1).turnCornerOnOrOff( 6, dotpr7<=0);
								 //this->mc.at( voxels.size()-1).turnCornerOnOrOff( 7, dotpr8<=0);



 								 this->mc.at( mc.size()-1).tr.push_back( tr); 

								 for (int m=0; m<12; m++){
									 this->mc.at( mc.size()-1).intersection_points.push_back( interpoints.at(m));
								 }
							 }
							 else {
								 if (b1) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 0, dotpr1<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 1, dotpr2<=0);}
								 if (b2) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 1, dotpr2<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 2, dotpr3<=0);}
								 if (b3) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 2, dotpr3<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 3, dotpr4<=0);}
								 if (b4) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 0, dotpr1<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 3, dotpr4<=0);}
								 if (b5) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 4, dotpr5<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 5, dotpr6<=0);}
								 if (b6) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 5, dotpr6<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 6, dotpr7<=0);}
								 if (b7) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 6, dotpr7<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 7, dotpr8<=0);}
								 if (b8) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 7, dotpr8<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 4, dotpr5<=0);}
								 if (b9) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 0, dotpr1<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 4, dotpr5<=0);}
								 if (b10) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 1, dotpr2<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 5, dotpr6<=0);}
								 if (b11) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 2, dotpr3<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 6, dotpr7<=0);}
								 if (b12) { 
									 this->mc.at( vox_pos).turnCornerOnOrOff( 7, dotpr8<=0);
									 this->mc.at( vox_pos).turnCornerOnOrOff( 3, dotpr4<=0);}
								 // corner state is false if it is outside the surface
								 //if ( !this->mc.at( vox_pos).corner_states.at(0) && dotpr1<=0 && pointInsideSurface( tri, normal, cube0)) {
									// this->mc.at( vox_pos).turnCornerOnOrOff( 0, true);}		
								 //if ( !this->mc.at( vox_pos).corner_states.at(1) && dotpr2<=0 && pointInsideSurface( tri, normal, cube1)) {
									// this->mc.at( vox_pos).turnCornerOnOrOff( 1, true);}
								 //if ( !this->mc.at( vox_pos).corner_states.at(2) && dotpr3<=0 && pointInsideSurface( tri, normal, cube2)){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 2, true);}								 
								 //if ( !this->mc.at( vox_pos).corner_states.at(3) && dotpr4<=0 && pointInsideSurface( tri, normal, cube3)){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 3, true);}								 
								 //if ( !this->mc.at( vox_pos).corner_states.at(4) && dotpr5<=0 && pointInsideSurface( tri, normal, cube4)){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 4, true);}
								 //if ( !this->mc.at( vox_pos).corner_states.at(5) && dotpr6<=0 && pointInsideSurface( tri, normal, cube5)){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 5, true);}
								 //if ( !this->mc.at( vox_pos).corner_states.at(6) && dotpr7<=0 && pointInsideSurface( tri, normal, cube6)){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 6, true);}
								 //if ( !this->mc.at( vox_pos).corner_states.at(7) && dotpr8<=0 && pointInsideSurface( tri, normal, cube7)){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 7, true);}

								 //if ( this->mc.at( vox_pos).corner_states.at(0) && dotpr1>0) {
									// this->mc.at( vox_pos).turnCornerOnOrOff( 0, false);}		
								 //if ( this->mc.at( vox_pos).corner_states.at(1) && dotpr2>0) {
									// this->mc.at( vox_pos).turnCornerOnOrOff( 1, false);}
								 //if ( this->mc.at( vox_pos).corner_states.at(2) && dotpr3>0){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 2, false);}								 
								 //if ( this->mc.at( vox_pos).corner_states.at(3) && dotpr4>0){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 3, false);}								 
								 //if ( this->mc.at( vox_pos).corner_states.at(4) && dotpr5>0){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 4, false);}
								 //if ( this->mc.at( vox_pos).corner_states.at(5) && dotpr6>0){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 5, false);}
								 //if ( this->mc.at( vox_pos).corner_states.at(6) && dotpr7>0){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 6, false);}
								 //if ( this->mc.at( vox_pos).corner_states.at(7) && dotpr8>0){
									// this->mc.at( vox_pos).turnCornerOnOrOff( 7, false);}




								 this->mc.at( vox_pos).tr.push_back( tr);

								 for (int m=0; m<12; m++){
									 if ( !interpoints.at(m).equals( null)){
										 this->mc.at( vox_pos).intersection_points.at(m).insert( interpoints.at(m));
									 }
								 }
							 }
					}
				}
			}
		}
	}
}

bool pointInsideSurface( TriangleCoord tri, Vector normal, Point p){
	Point tp1, tp2, tp3;
	tp1.insert(tri.p1);
	tp2.insert(tri.p2);
	tp3.insert(tri.p3);
	// compute triangle vectors
	Vector ab,bc,ca,ac, line_dir;
	ab.insert(tp2.x-tp1.x, tp2.y-tp1.y, tp2.z-tp1.z);  // vector tp1->tp2
	bc.insert(tp3.x-tp2.x, tp3.y-tp2.y, tp3.z-tp2.z);  // vector tp2->tp3
	ca.insert(tp1.x-tp3.x, tp1.y-tp3.y, tp1.z-tp3.z);  // vector tp3->tp1
	ac.insert(tp3.x-tp1.x, tp3.y-tp1.y, tp3.z-tp1.z);  // vector tp1->tp3
	// calculate line direction 
	line_dir.insert( normal.x, normal.y, normal.z);
	//float dist =  sqrt( pow(line_dir.x,2)+ pow(line_dir.y,2)+ pow(line_dir.z,2));
	//line_dir.insert( line_dir.x/dist, line_dir.y/ dist, line_dir.z/ dist);
	// check if line and triangle plane are parallel
	float NDotRayDir = normal.dotproduct( line_dir);
	// calculate D
	float D = -normal.dotproduct(tp1);
	// line parametric form p+n*t,where p=(p.x,p.y,p.z) and n=triangle normal
	// compute t
	float t= -( D + normal.dotproduct(p))/ NDotRayDir;
	// compute the intersection point 
	Point interp;
	interp.insert( p.x+t*line_dir.x, \
				   p.y+t*line_dir.y, \
				   p.z+t*line_dir.z);
	// check if the intersection point interp is inside or outside the triangle
	// that means we check if the point is at the right side of the triangle vectors 
	// that's legal if triangles are counter-clockwise
	//------------------------------------------------------
	Vector ap,bp,cp;
	ap.insert( interp.x-tp1.x, \
			   interp.y-tp1.y, \
			   interp.z-tp1.z);     //vector tp1->interp
	Vector n1;
	n1 = ab.crossProduct(ap);
	float dotpr0 = normal.dotproduct(n1);
	if (dotpr0<0) return false;  // interp is on the right side 

	bp.insert( interp.x-tp2.x, \
			   interp.y-tp2.y, \
			   interp.z-tp2.z);
	Vector n2;
	n2 = bc.crossProduct(bp);
	float dotpr1 = normal.dotproduct(n2);
	if (dotpr1<0) return false;  // interp is on the right side
		
	cp.insert( interp.x-tp3.x, \
			   interp.y-tp3.y, \
			   interp.z-tp3.z);
	Vector n3;
	n3 = ca.crossProduct(cp);
	float dotpr2 = normal.dotproduct(n3);
	if (dotpr2<0) return false;  // interp is on the right side

	return true;
}

bool checkLineTriangleIntersection( TriangleCoord tri, Vector normal, Point lp1, Point lp2, Point &interpoint){
	Point tp1, tp2, tp3;
	tp1.insert(tri.p1);
	tp2.insert(tri.p2);
	tp3.insert(tri.p3);

	interpoint.insert( 100, 100, 100);
	// plane equation: Ax+By+Cz=D
	// compute triangle vectors
	Vector ab,bc,ca,ac, line_dir;
	ab.insert(tp2.x-tp1.x, tp2.y-tp1.y, tp2.z-tp1.z);  // vector tp1->tp2
	bc.insert(tp3.x-tp2.x, tp3.y-tp2.y, tp3.z-tp2.z);  // vector tp2->tp3
	ca.insert(tp1.x-tp3.x, tp1.y-tp3.y, tp1.z-tp3.z);  // vector tp3->tp1
	ac.insert(tp3.x-tp1.x, tp3.y-tp1.y, tp3.z-tp1.z);  // vector tp1->tp3
	// calculate NOT normalized normal
	//normal = ab.crossProduct(ac);
	// calculate line direction 
	line_dir.insert( lp2.x-lp1.x, \
					 lp2.y-lp1.y, \
					 lp2.z-lp1.z);
	//float dist =  sqrt( pow(line_dir.x,2)+ pow(line_dir.y,2)+ pow(line_dir.z,2));
	//line_dir.insert( line_dir.x/dist, line_dir.y/ dist, line_dir.z/ dist);
	// check if line and triangle plane are parallel
	float NDotRayDir = normal.dotproduct( line_dir);
	if (NDotRayDir==0) return false;	// line and triangle plane are parallel!
	// calculate D
	float D = -normal.dotproduct(tp1);
	// line parametric form p+n*t,where p=(p.x,p.y,p.z) and n=triangle normal
	// compute t
	float t= -( D + normal.dotproduct(lp1))/ NDotRayDir;
	// compute the intersection point 
	Point interp;
	interp.insert( lp1.x+t*line_dir.x, \
				   lp1.y+t*line_dir.y, \
				   lp1.z+t*line_dir.z);
	// check if the intersection point interp is inside or outside the triangle
	// that means we check if the point is at the right side of the triangle vectors 
	// that's legal if triangles are counter-clockwise
	//------------------------------------------------------
	Vector ap,bp,cp;
	ap.insert( interp.x-tp1.x, \
			   interp.y-tp1.y, \
			   interp.z-tp1.z);     //vector tp1->interp
	Vector n1;
	n1 = ab.crossProduct(ap);
	float dotpr0 = normal.dotproduct(n1);
	if (dotpr0<0) return false;  // interp is on the right side 

	bp.insert( interp.x-tp2.x, \
			   interp.y-tp2.y, \
			   interp.z-tp2.z);
	Vector n2;
	n2 = bc.crossProduct(bp);
	float dotpr1 = normal.dotproduct(n2);
	if (dotpr1<0) return false;  // interp is on the right side
		
	cp.insert( interp.x-tp3.x, \
			   interp.y-tp3.y, \
			   interp.z-tp3.z);
	Vector n3;
	n3 = ca.crossProduct(cp);
	float dotpr2 = normal.dotproduct(n3);
	if (dotpr2<0) return false;  // interp is on the right side


	if (line_dir.y!=0){
		if ( (interp.y<=lp1.y && interp.y>=lp2.y) || (interp.y>=lp1.y && interp.y<=lp2.y) ) {
			interpoint.insert( interp);
			return true;}
		else return false;		
	}
	if (line_dir.z!=0){
		if ( (interp.z<=lp1.z && interp.z>=lp2.z) || (interp.z>=lp1.z && interp.z<=lp2.z) ) {
			interpoint.insert( interp);
			return true;}
		else return false;		
	}
	if (line_dir.x!=0){
		if ( (interp.x<=lp1.x && interp.x>=lp2.x) || (interp.x>=lp1.x && interp.x<=lp2.x) ) {
			interpoint.insert( interp);
			return true;}
		else return false;		
	}

	//glColor3f( 1, 0.3,1);
	//glBegin( GL_POLYGON);
	//glVertex3f( interp.x, interp.y, interp.z);
	//glVertex3f( interp.x+0.1, interp.y, interp.z);
	//glVertex3f( interp.x, interp.y+0.1, interp.z);
	//glEnd();
	//glBegin( GL_POLYGON);
	//glVertex3f( interp.x, interp.y, interp.z);
	//glVertex3f( interp.x, interp.y+0.1, interp.z);
	//glVertex3f( interp.x+0.1, interp.y, interp.z);
	//glEnd();
	//return true;
	interpoint.insert(interp);
	return false;
}

bool VoxelModel::voxelAlreadyExists(Voxel vox, int &voxpos){
	for (int i=0; i<int(this->voxels.size()); i++){
		if ( voxels.at(i).equals( vox )) {
			voxpos=i;
			return true;
		}
	}
	return false;
}

bool pointInsideCube( Point cube0, float d, Point p){		// d= cube_width 
	Point cube2,cube3,cube4,cube5,cube6,cube7,cube1;

	cube1.insert( cube0.x+d, cube0.y, cube0.z);
	cube2.insert( cube1.x, cube1.y+d, cube1.z);
	cube3.insert( cube2.x-d, cube2.y, cube2.z);
	cube4.insert( cube3.x, cube3.y-d, cube3.z-d);					
	cube5.insert( cube4.x+d, cube4.y, cube4.z);
	cube6.insert( cube5.x, cube5.y+d, cube5.z);
	cube7.insert( cube6.x-d, cube6.y, cube6.z);
	
	if (p>=cube4 && p<=cube2) return true;
	else return false;

}

float VoxelModel::setUpVoxelWidth(){
	float w = this->voxel_width;

	float w1=VOXEL_WIDTH1;
	float w2=VOXEL_WIDTH2;
	float w3=VOXEL_WIDTH3;
	float w4=VOXEL_WIDTH4;
	float w5=VOXEL_WIDTH5;

	if (w==w1) {
		w=w2;}
	else if (w==w2) {
		w=w3;}
	else if (w==w3) {
		w=w4;}
	else if (w==w4) {
		w=w5;}
	else if (w==w5) {
		w=w1;}
	else if (w==NULL) {
		w=w3;}

	return w;
}