#include <stdio.h>     // - Just for some ASCII messages
#include <vector>
#include <map>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "structs.h"
#include "FreeFall.h"
#include "counter.h"
#include <math.h>


static std::vector< std::vector<Voxel> > buckets;
static std::vector< std::vector<Vector>> buck_velocities;

static int voxel_quantity=0;



//================ free falling functions ==============

void drawSphereVoxels(std::vector<Voxel> voxels ){
	//StartCounter();

	glColor4f( 0.4, 0.1, 0.4, 0.9);
	for (int i=0;i<int(voxels.size()); i++){
		glPushMatrix();
		glTranslatef(voxels.at(i).x, voxels.at(i).y, voxels.at(i).z);
		glutSolidSphere(voxels.at(0).width()/2, 10, 10);
		glPopMatrix();
	}

	//printf(" %f\n", GetCounter());
}

void freeFallOfVoxels( std::vector<Voxel> &voxels, std::vector< std::vector<Voxel>> &voxel_data, float dt){
	//StartCounter();

	float g=25;  // gravity

	// U=Uo+a*dt,	x=xo+U*dt
	Vector voxel_transp;
	std::vector<Voxel> voxel_data_t0;
	if (voxel_quantity<int(voxels.size())) voxel_quantity+=5;

	for (int i=0; i<int(voxels.size()); i++){
		if (i<voxel_quantity){
			//for (int i=0; i<voxel_quantity; i++){
			if (voxel_quantity>int(voxels.size())) {
				voxel_quantity=int(voxels.size());
			}
			voxels.at(i).velocity.insert( voxels.at(i).velocity.x- 0.0001*dt, voxels.at(i).velocity.y- g*dt, voxels.at(i).velocity.z);
			//voxelVelocity.at(i).insert( voxelVelocity.at(i).x, voxelVelocity.at(i).y- g*dt, voxelVelocity.at(i).z);
			voxel_transp.insert(voxels.at(i).x+ voxels.at(i).velocity.x*dt, voxels.at(i).y+voxels.at(i).velocity.y*dt, voxels.at(i).z+voxels.at(i).velocity.z*dt);
			voxels.at(i).insert( voxel_transp.x, voxel_transp.y, voxel_transp.z);
		}
		
		voxel_data_t0.push_back( voxels.at(i));
	}

	voxel_data.push_back( voxel_data_t0);

	//printf("%f\n", GetCounter());
}

void checkFloorCollisions( std::vector<Voxel> &voxels, float tx, float ty, float tz, float dt ){
	float floor = FLOOR -ty, right_wall = RIGHT_WALL -tx, left_wall = LEFT_WALL -tx, far_wall = FAR_WALL -tz, near_wall = NEAR_WALL -tz; 

	float penetration;
	float n=2;  // force of friction
	
	//float e=1.0;   //this is factor of restitution
	//Vector normal;
	//float Vn, jf;
	//normal.insert( 0,-1,0);

	for (int i=0; i<int(voxels.size());i++){

		//============= check collisions with floor ===========================

		if ( (voxels.at(i).y- voxels.at(0).width()/2) <= floor){
			/*Vn = voxels.at(i).velocity.dotproduct( normal);
			jf= -(1+e)*Vn/2;*/
			//voxels.at(i).velocity.y = -voxels.at(i).velocity.y + jf;
			voxels.at(i).velocity.y= - voxels.at(i).velocity.y;

			penetration = (voxels.at(i).width()/2) + floor - voxels.at(i).y;
			voxels.at(i).y += penetration+ 0.001;
		}

		if ( (voxels.at(i).y- voxels.at(0).width()/2) > (floor - voxels.at(0).width()/10) &&
			 (voxels.at(i).y- voxels.at(0).width()/2) < (floor + voxels.at(0).width()/10) ){
			if (voxels.at(i).velocity.x< 0) {
				voxels.at(i).velocity.x = voxels.at(i).velocity.x + n*dt; }
			else  {
				voxels.at(i).velocity.x = voxels.at(i).velocity.x - n*dt;}
			if (voxels.at(i).velocity.z< 0) {
				voxels.at(i).velocity.z = voxels.at(i).velocity.z + n*dt; }
			else  {
				voxels.at(i).velocity.z = voxels.at(i).velocity.z - n*dt; }
		}

		//================== check collisions with walls =============
		if (voxels.at(i).x>=0 && (voxels.at(i).x+ voxels.at(0).width()/2)>= right_wall) {
			voxels.at(i).velocity.x = -voxels.at(i).velocity.x;
			penetration = voxels.at(0).width()/2 - (right_wall - voxels.at(i).x);
			voxels.at(i).x -= penetration +0.001; }
		else if (voxels.at(i).x<0 && (voxels.at(i).x - voxels.at(0).width()/2)<= left_wall) {
			voxels.at(i).velocity.x = -voxels.at(i).velocity.x;
			penetration = voxels.at(0).width()/2 - ( - left_wall + voxels.at(i).x);
			voxels.at(i).x += penetration +0.001; }

		if (voxels.at(i).z-40>=0 && (voxels.at(i).z-40 + voxels.at(0).width()/2)>= near_wall) {
			voxels.at(i).velocity.z = -voxels.at(i).velocity.z;
			penetration = voxels.at(0).width()/2 - (near_wall - voxels.at(i).z+40);
			voxels.at(i).z -= penetration +0.001; }
		else if (voxels.at(i).z-40<0 && (voxels.at(i).z-40- voxels.at(0).width()/2)<= far_wall) {
			voxels.at(i).velocity.z = -voxels.at(i).velocity.z;
			penetration = voxels.at(0).width()/2 - ( - far_wall + voxels.at(i).z-40);
			voxels.at(i).z += penetration +0.001; }
	}
}

void checkVoxelCollisions( std::vector<Voxel> &voxels){
	//StartCounter();

	float e=1.0;    //this is factor of restitution
	float dist, Vn, penetration, sqr_dist, jf;
	Vector Vab, normal;

	//check collisions with each other
	for (int i=0; i<int(voxels.size()); i++){
		for (int j=i+1; j<int(voxels.size()); j++){
			if ((voxels.at(i).x-voxels.at(j).x)<(3*voxels.at(i).width()/2) && 
			    (voxels.at(i).y-voxels.at(j).y)<(3*voxels.at(i).width()/2) && 
			    (voxels.at(i).z-voxels.at(j).z)<(3*voxels.at(i).width()/2) &&
				!voxels.at(i).velocity.equals( voxels.at(j).velocity)  ) 
			{
				sqr_dist = pow(voxels.at(i).x-voxels.at(j).x,2) + \
				     	   pow(voxels.at(i).y-voxels.at(j).y,2) + \
						   pow(voxels.at(i).z-voxels.at(j).z,2);

				// voxels.at(i)=p1, voxels.at(j)=p2
				if (sqr_dist<= (pow(voxels.at(0).width(),2)-0.0001)){
					dist = sqrt( sqr_dist);

					normal.insert( voxels.at(j).x-voxels.at(i).x, voxels.at(j).y-voxels.at(i).y, voxels.at(j).z-voxels.at(i).z); 
					normal.insert( normal.x/ dist, normal.y/ dist, normal.z/ dist);
						
					Vab.insert( voxels.at(i).velocity.x-voxels.at(j).velocity.x, voxels.at(i).velocity.y-voxels.at(j).velocity.y, voxels.at(i).velocity.z-voxels.at(j).velocity.z);
						
					Vn = Vab.dotproduct( normal);
					if (Vn>=0){
					    penetration = voxels.at(0).width()- dist;
						jf= -(1+e)*Vn/2;   // masses are unit

						voxels.at(i).velocity.insert( voxels.at(i).velocity.x+ jf*normal.x, voxels.at(i).velocity.y+ jf*normal.y, voxels.at(i).velocity.z+ jf*normal.z);
						voxels.at(j).velocity.insert( voxels.at(j).velocity.x- jf*normal.x, voxels.at(j).velocity.y- jf*normal.y, voxels.at(j).velocity.z- jf*normal.z);
												
						voxels.at(i).insert( voxels.at(i).x- normal.x*penetration, voxels.at(i).y- normal.y*penetration, voxels.at(i).z- normal.z*penetration); 
					}
				}
			}			
		}
	}
    //printf( " %f \n", GetCounter());
}

void initiateVelocities( std::vector<Voxel> &voxels){
	//voxelVelocity.clear();
	
	/*Vector zero;
	zero.insert( 0,0,0);*/
	for (int i=0; i<int(voxels.size());i++){
		voxels.at(i).velocity.insert( 0,0,0);
		//voxelVelocity.push_back( zero);
	}
	voxel_quantity=0;
}



//------------ buckets ----------------
//void checkVoxelCollisionsInBuckets( std::vector< Voxel> &voxels, std::vector< Vector> &voxelVelocity){
//	//StartCounter();
//	//============= setup Buckets ================
//
//
//	// room_width = 200 (from -100 to 100), room_height = 80 (from -10 to 70)
//	// bucket_width = 5
//	// room_columns = room_width/bucket_width, room_rows = room_height/bucket_width
//	
//	int columns, rows, rwidth, rheight, bwidth;
//	rwidth = 200; rheight = 80;
//	bwidth = 5;
//	columns = rwidth/bwidth; 
//	rows = rheight/bwidth;
//
//	
//	buckets.clear();
//	
//	Voxel zero;
//	zero.insert(0,0,0);
//	std::vector<Voxel> zero_vec;
//	Vector zer;
//	zer.insert(0,0,0);
//	std::vector<Vector> zer_vec;
//	for (int k=0; k<(columns*rows); k++){
//		buckets.push_back(zero_vec);
//		buck_velocities.push_back(zer_vec);
//	}
//	
//
//	int row, col;
//	for (int i=0; i<int(voxels.size()); i++){
//		row = (voxels.at(i).x+10)/bwidth-1;
//		col = (voxels.at(i).y+100)/bwidth+1;
//
//		buckets.at( row*columns+col).insert( buckets.at( columns*row+col).end(), voxels.at(i));
//		buck_velocities.at( row*columns+col).insert( buck_velocities.at( columns*row+col).end(), voxelVelocity.at(i));
//		//buckets.insert( buckets.begin(), row*columns+col, voxels.at(i))
//	}	
//
//
//	//======= check which buckets have to be checked for collisions =======
//
//	for (int i=0; i<int(buckets.size()); i++){
//		int b=i;
//
//		for (int k=0; k<int(buckets.at(i).size()); k++){
//			
//			row = (buckets.at(i).at(k).x+10)/bwidth;
//			col = (buckets.at(i).at(k).y+1000)/bwidth;
//
//			for (int v=k; v<int(buckets.at(i).size()); v++){
//				checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(i).at(v), buckets.at(i).at(k), buckets.at(i).at(v) );	
//			}
//			b=i-1;
//			if (b>=0) {
//				for (int v=0; v<int(buckets.at(b).size()); v++){
//					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
//				}
//			}
//			b=i+1;
//			if (b< int(buckets.size()) ){
//				for (int v=0; v<int(buckets.at(b).size()); v++){
//					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
//				}
//			}
//			b=columns*(row-1)+col-1;
//			if (b>=0) {
//				for (int v=0; v<int(buckets.at(b).size()); v++){
//					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
//				}
//			}
//			b=columns*(row-1)+col;
//			if (b>=0) {
//				for (int v=0; v<int(buckets.at(b).size()); v++){
//					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
//				}
//			}
//			b=columns*(row-1)+col+1;
//			if (b>=0) {
//				for (int v=0; v<int(buckets.at(b).size()); v++){
//					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
//				}
//			}
//			b=columns*(row+1)+col-1;
//			if (b< int(buckets.size())) {
//				for (int v=0; v<int(buckets.at(b).size()); v++){
//					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
//				}
//			}
//			b=columns*(row+1)+col;
//			if (b< int(buckets.size())) {
//				for (int v=0; v<int(buckets.at(b).size()); v++){
//					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
//				}
//			}
//			b=columns*(row+1)+col+1;
//			if (b< int(buckets.size())) {
//				for (int v=0; v<int(buckets.at(b).size()); v++){
//					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
//				}
//			}
//		}
//	}
//    //printf( " %f \n", GetCounter());
//}
//
//void checkVoxelCollision( Vector &vel1, Vector &vel2, Voxel &vox1, Voxel &vox2){
//	float e=1.0;    //this is factor of restitution
//	float dist, Vn, penetration, sqr_dist, jf;
//	Vector Vab, normal;
//
//	sqr_dist = pow( vox1.x-vox2.x,2) + \
//			   pow( vox1.y-vox2.y,2) + \
//			   pow( vox1.z-vox2.z,2);
//					
//	// voxels.at(i)=p1, voxels.at(j)=p2
//	if (sqr_dist<= (pow(vox1.width(),2)-0.0001)){
//		dist = sqrt( sqr_dist);
//
//		normal.insert( vox2.x-vox1.x, vox2.y-vox1.y, vox2.z-vox1.z); 
//		normal.insert( normal.x/ dist, normal.y/ dist, normal.z/ dist);
//						
//		Vab.insert( vel1.x-vel2.x, vel1.y-vel2.y, vel1.z-vel2.z);
//						
//		Vn = Vab.dotproduct( normal);
//		if (Vn>=0){
//			penetration = vox1.width()- dist;
//			jf= -(1+e)*Vn/2;   // masses are unit
//			
//			vel1.insert( vel1.x+ jf*normal.x, vel1.y+ jf*normal.y, vel1.z+ jf*normal.z);
//			vel2.insert( vel2.x- jf*normal.x, vel2.y- jf*normal.y, vel2.z- jf*normal.z);
//												
//			vox1.insert( vox1.x- normal.x*penetration, vox1.y- normal.y*penetration, vox1.z- normal.z*penetration); 
//		}
//	}
//}

