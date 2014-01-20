#include <stdio.h>     // - Just for some ASCII messages
#include <string>
#include <array>
#include <vector>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "structs.h"
#include "FreeFall.h"


static int voxel_quantity=0;
static bool fall_done=false;

void drawSphereVoxels(std::vector<Voxel> voxels, float voxel_width){
	
	glColor4f( 0.4, 0.1, 0.4, 0.9);
	for (int i=0;i<int(voxels.size()); i++){

		glPushMatrix();
		glTranslatef(voxels.at(i).x+voxel_width/2, voxels.at(i).y+voxel_width/2, voxels.at(i).z-voxel_width/2);
		glutSolidSphere(voxel_width/2, 10, 10);
		glPopMatrix();
	}
}

void freeFallOfVoxels( std::vector<Voxel> &voxels, std::vector<Vector> &voxelVelocity, Vector voxel_a, float dt){
	// U=Uo+a*dt,	x=xo+U*dt
	Vector voxel_transp;
	if (voxel_quantity<=int(voxels.size())) voxel_quantity+=10;

	for (int i=0; i<voxel_quantity; i++){
		if (voxel_quantity>int(voxels.size())) {
			voxel_quantity=int(voxels.size());
			fall_done=true;
		}
		voxelVelocity.at(i).insert( voxelVelocity.at(i).x+ voxel_a.x*dt, voxelVelocity.at(i).y+ voxel_a.y*dt, voxelVelocity.at(i).z+ voxel_a.z*dt);
		voxel_transp.insert(voxels.at(i).x+ voxelVelocity.at(i).x*dt, voxels.at(i).y+voxelVelocity.at(i).y*dt, voxels.at(i).z+voxelVelocity.at(i).z*dt);
		voxels.at(i).insert( voxel_transp.x, voxel_transp.y, voxel_transp.z);
	}

}

void checkFloorCollisions( std::vector<Voxel> &voxels, std::vector<Vector> &voxelVelocity, float floor_coord_y ){
	//check collisions with floor
	for (int i=0; i<int(voxels.size());i++){
		if (voxels.at(i).y-voxels.at(0).width()/2<= floor_coord_y){
			voxelVelocity.at(i).y= -voxelVelocity.at(i).y;
			voxels.at(i).y += voxels.at(0).width();
		}
	}
}

void checkVoxelCollisions( std::vector<Voxel> &voxels, std::vector<Vector> &voxelVelocity){
	float e=1.0;    //this is coefficient factor of restitution

	//check collisions with each other
	if (fall_done==true){
		for (int i=0; i<int(voxels.size()); i++){
			for (int j=0; j<int(voxels.size()); j++){
				float t=0;
				Vector vel_comp, transp_comp;
				std::vector<Vector> velocity_components, transp_components;

				if (i!=j){
					Vector Vab, normal;
					float Vn;

					float sqr_dist = pow( voxels.at(i).x-voxels.at(j).x,2) + \
									 pow( voxels.at(i).y-voxels.at(j).y,2) + \
									 pow( voxels.at(i).z-voxels.at(j).z,2);

					// voxels.at(i)=p1, voxels.at(j)=p2
					if (sqr_dist<= pow(voxels.at(0).width(),2)){
						normal.insert( voxels.at(j).x-voxels.at(i).x, voxels.at(j).y-voxels.at(i).y, voxels.at(j).z-voxels.at(i).z); 
						normal.insert( normal.x/ sqrt(sqr_dist), normal.y/sqrt(sqr_dist), normal.z/sqrt(sqr_dist));
						
						Vab.insert( voxelVelocity.at(i).x-voxelVelocity.at(j).x, voxelVelocity.at(i).y-voxelVelocity.at(j).y, voxelVelocity.at(i).z-voxelVelocity.at(j).z);
							
						Vn = Vab.dotproduct( normal);

						if (Vn>=0){
							float jf= -(1+e)*Vn/2;   // masses are unit
							voxelVelocity.at(i).insert( voxelVelocity.at(i).x+ jf*normal.x, voxelVelocity.at(i).y+ jf*normal.y, voxelVelocity.at(i).z+ jf*normal.z);
							voxelVelocity.at(j).insert( voxelVelocity.at(j).x- jf*normal.x, voxelVelocity.at(j).y- jf*normal.y, voxelVelocity.at(j).z- jf*normal.z);
							
							t = voxels.at(0).width()/ (sqrt( pow(normal.x,2)+ pow(normal.y,2)+ pow(normal.z,2)));
							
							voxels.at(i).insert( voxels.at(j).x- normal.x*t, voxels.at(j).y- normal.y*t, voxels.at(j).z- normal.z*t); 
						}
					}
				}			
			}
		}
	}
}

void initiateVelocities( std::vector<Voxel> voxels, std::vector<Vector> &voxelVelocity){
	Vector zero;
	zero.insert( 0,0,0);
	for (int i=0; i<int(voxels.size());i++){
		voxelVelocity.push_back( zero);
	}
	fall_done=false;
	voxel_quantity=0;
}