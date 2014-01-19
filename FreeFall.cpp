#include <stdio.h>     // - Just for some ASCII messages
#include <string>
#include <array>
#include <vector>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "structs.h"
#include "FreeFall.h"


static int voxel_quantity=0;

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
		if (voxel_quantity>int(voxels.size())) voxel_quantity=int(voxels.size());
		voxelVelocity.at(i).insert( voxelVelocity.at(i).x+ voxel_a.x*dt, voxelVelocity.at(i).y+ voxel_a.y*dt, voxelVelocity.at(i).z+ voxel_a.z*dt);
		voxel_transp.insert(voxels.at(i).x+ voxelVelocity.at(i).x*dt, voxels.at(i).y+voxelVelocity.at(i).y*dt, voxels.at(i).z+voxelVelocity.at(i).z*dt);
		voxels.at(i).insert( voxel_transp.x, voxel_transp.y, voxel_transp.z);
	}
}

void initiateVelocities( std::vector<Voxel> voxels, std::vector<Vector> &voxelVelocity){
	Vector zero;
	zero.insert( 0,0,0);
	for (int i=0; i<int(voxels.size());i++){
		voxelVelocity.push_back( zero);
	}
}