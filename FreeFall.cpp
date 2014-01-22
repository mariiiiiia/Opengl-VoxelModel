#include <stdio.h>     // - Just for some ASCII messages
#include <string>
#include <array>
#include <vector>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "structs.h"
#include "FreeFall.h"
#include <windows.h>
#include <math.h>
#include <stdexcept>      // std::out_of_range
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


static std::vector< std::vector<Voxel> > buckets;
static std::vector< std::vector<Vector>> buck_velocities;

static int voxel_quantity=0;

//---- run time calculations ---
double PCFreq = 0.0;
__int64 CounterStart = 0;


// ============ runtime counter =============

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li)) printf( "QueryPerformanceFrequency failed!\n");

    PCFreq = double(li.QuadPart)/1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}


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

void freeFallOfVoxels( std::vector<Voxel> &voxels, std::vector<Vector> &voxelVelocity, std::vector< std::vector<Voxel>> &voxel_data, float dt){
	//StartCounter();

	float g=20;  // gravity
	//FILE * voxels_coords_file;
	//voxels_coords_file = fopen("voxels_coords.txt", "a");
	//stringstream ss_voxel_coords;

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
			voxelVelocity.at(i).insert( voxelVelocity.at(i).x- 0.0001*dt, voxelVelocity.at(i).y- g*dt, voxelVelocity.at(i).z);
			voxel_transp.insert(voxels.at(i).x+ voxelVelocity.at(i).x*dt, voxels.at(i).y+voxelVelocity.at(i).y*dt, voxels.at(i).z+voxelVelocity.at(i).z*dt);
			voxels.at(i).insert( voxel_transp.x, voxel_transp.y, voxel_transp.z);
		}

		//fprintf( voxels_coords_file, "%d %f %f %f\n", i, voxels.at(i).x, voxels.at(i).y, voxels.at(i).z) ;
		//ss_voxel_coords << voxels.at(i).x << " " << voxels.at(i).y << " " << voxels.at(i).z << "\n";
		voxel_data_t0.push_back( voxels.at(i));
	}
	voxel_data.push_back( voxel_data_t0);
	//fclose( voxels_coords_file);
	//writeToFile( ss_voxel_coords);

	//printf("%f\n", GetCounter());
}

void checkFloorCollisions( std::vector<Voxel> &voxels, std::vector<Vector> &voxelVelocity, float floor_coord_y ){
	float e=1.0;   //this is factor of restitution
	Vector normal;
	float Vn, jf;
	
	normal.insert( 0,1,0);

	//check collisions with floor
	for (int i=0; i<int(voxels.size());i++){
		if (voxels.at(i).y- voxels.at(0).width()/2 <= floor_coord_y){
			Vn = voxelVelocity.at(i).dotproduct( normal);
			jf= -(1+e)*Vn/2;
			voxelVelocity.at(i).y= -voxelVelocity.at(i).y - jf;
			//voxelVelocity.at(i).y= - voxelVelocity.at(i).y;

			float penetration = -voxels.at(i).y + (voxels.at(i).width()/2) + floor_coord_y;
			voxels.at(i).y += penetration+ 0.001;
		}
	}
}

void checkVoxelCollisions( std::vector<Voxel> &voxels, std::vector<Vector> &voxelVelocity){
	//StartCounter();

	float e=1.0;    //this is factor of restitution
	float dist, Vn, penetration, sqr_dist, jf;
	Vector Vab, normal;

	//check collisions with each other
	for (int i=0; i<int(voxels.size()); i++){
		for (int j=i+1; j<int(voxels.size()); j++){
			if (i!=j && 
			    (voxels.at(i).x-voxels.at(j).x)<(3*voxels.at(i).width()/2) && 
			    (voxels.at(i).y-voxels.at(j).y)<(3*voxels.at(i).width()/2) && 
			    (voxels.at(i).z-voxels.at(j).z)<(3*voxels.at(i).width()/2) &&
				!voxelVelocity.at(i).equals( voxelVelocity.at(j))  ) 
			{
				sqr_dist = pow( voxels.at(i).x-voxels.at(j).x,2) + \
				     	   pow( voxels.at(i).y-voxels.at(j).y,2) + \
						   pow( voxels.at(i).z-voxels.at(j).z,2);

				// voxels.at(i)=p1, voxels.at(j)=p2
				if (sqr_dist<= (pow(voxels.at(0).width(),2)-0.0001)){
					dist = sqrt( sqr_dist);

					normal.insert( voxels.at(j).x-voxels.at(i).x, voxels.at(j).y-voxels.at(i).y, voxels.at(j).z-voxels.at(i).z); 
					normal.insert( normal.x/ dist, normal.y/ dist, normal.z/ dist);
						
					Vab.insert( voxelVelocity.at(i).x-voxelVelocity.at(j).x, voxelVelocity.at(i).y-voxelVelocity.at(j).y, voxelVelocity.at(i).z-voxelVelocity.at(j).z);
						
					Vn = Vab.dotproduct( normal);
					if (Vn>=0){
					    penetration = voxels.at(0).width()- dist;
						jf= -(1+e)*Vn/2;   // masses are unit

						voxelVelocity.at(i).insert( voxelVelocity.at(i).x+ jf*normal.x, voxelVelocity.at(i).y+ jf*normal.y, voxelVelocity.at(i).z+ jf*normal.z);
						voxelVelocity.at(j).insert( voxelVelocity.at(j).x- jf*normal.x, voxelVelocity.at(j).y- jf*normal.y, voxelVelocity.at(j).z- jf*normal.z);
												
						voxels.at(i).insert( voxels.at(i).x- normal.x*penetration, voxels.at(i).y- normal.y*penetration, voxels.at(i).z- normal.z*penetration); 
					}
				}
			}			
		}
	}
    //printf( " %f \n", GetCounter());
}

void initiateVelocities( std::vector<Voxel> voxels, std::vector<Vector> &voxelVelocity){
	voxelVelocity.clear();
	
	Vector zero;
	zero.insert( 0,0,0);
	for (int i=0; i<int(voxels.size());i++){
		voxelVelocity.push_back( zero);
	}
	voxel_quantity=0;
}

void checkVoxelCollisionsInBuckets( std::vector< Voxel> &voxels, std::vector< Vector> &voxelVelocity){
	//StartCounter();
	//============= setup Buckets ================


	// room_width = 200 (from -100 to 100), room_height = 80 (from -10 to 70)
	// bucket_width = 5
	// room_columns = room_width/bucket_width, room_rows = room_height/bucket_width
	
	int columns, rows, rwidth, rheight, bwidth;
	rwidth = 200; rheight = 80;
	bwidth = 5;
	columns = rwidth/bwidth; 
	rows = rheight/bwidth;

	
	buckets.clear();
	
	Voxel zero;
	zero.insert(0,0,0);
	std::vector<Voxel> zero_vec;
	Vector zer;
	zer.insert(0,0,0);
	std::vector<Vector> zer_vec;
	for (int k=0; k<(columns*rows); k++){
		buckets.push_back(zero_vec);
		buck_velocities.push_back(zer_vec);
	}
	

	int row, col;
	for (int i=0; i<int(voxels.size()); i++){
		row = (voxels.at(i).x+10)/bwidth-1;
		col = (voxels.at(i).y+100)/bwidth+1;

		int pos = row*columns+col;
		buckets.at( row*columns+col).insert( buckets.at( columns*row+col).end(), voxels.at(i));
		buck_velocities.at( row*columns+col).insert( buck_velocities.at( columns*row+col).end(), voxelVelocity.at(i));
		//buckets.insert( buckets.begin(), row*columns+col, voxels.at(i))
	}	


	//======= check which buckets have to be checked for collisions =======
	StartCounter();

	int buck_size = int(buckets.size());
	for (int i=0; i<int(buckets.size()); i++){
		int b=i;

		for (int k=0; k<int(buckets.at(i).size()); k++){
			
			row = (buckets.at(i).at(k).x+10)/bwidth;
			col = (buckets.at(i).at(k).y+1000)/bwidth;

			for (int v=k; v<int(buckets.at(i).size()); v++){
				checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(i).at(v), buckets.at(i).at(k), buckets.at(i).at(v) );	
			}
			b=i-1;
			if (b>=0) {
				for (int v=0; v<int(buckets.at(b).size()); v++){
					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
				}
			}
			b=i+1;
			if (b< int(buckets.size()) ){
				for (int v=0; v<int(buckets.at(b).size()); v++){
					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
				}
			}
			b=columns*(row-1)+col-1;
			if (b>=0) {
				for (int v=0; v<int(buckets.at(b).size()); v++){
					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
				}
			}
			b=columns*(row-1)+col;
			if (b>=0) {
				for (int v=0; v<int(buckets.at(b).size()); v++){
					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
				}
			}
			b=columns*(row-1)+col+1;
			if (b>=0) {
				for (int v=0; v<int(buckets.at(b).size()); v++){
					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
				}
			}
			b=columns*(row+1)+col-1;
			if (b< int(buckets.size())) {
				for (int v=0; v<int(buckets.at(b).size()); v++){
					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
				}
			}
			b=columns*(row+1)+col;
			if (b< int(buckets.size())) {
				for (int v=0; v<int(buckets.at(b).size()); v++){
					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
				}
			}
			b=columns*(row+1)+col+1;
			if (b< int(buckets.size())) {
				for (int v=0; v<int(buckets.at(b).size()); v++){
					checkVoxelCollision( buck_velocities.at(i).at(k), buck_velocities.at(b).at(v), buckets.at(i).at(k), buckets.at(b).at(v) );
				}
			}
		}
	}
    printf( " %f \n", GetCounter());
}

void checkVoxelCollision( Vector &vel1, Vector &vel2, Voxel &vox1, Voxel &vox2){
	float e=1.0;    //this is factor of restitution
	float dist, Vn, penetration, sqr_dist, jf;
	Vector Vab, normal;

	sqr_dist = pow( vox1.x-vox2.x,2) + \
			   pow( vox1.y-vox2.y,2) + \
			   pow( vox1.z-vox2.z,2);
					
	// voxels.at(i)=p1, voxels.at(j)=p2
	if (sqr_dist<= (pow(vox1.width(),2)-0.0001)){
		float dist = sqrt( sqr_dist);

		normal.insert( vox2.x-vox1.x, vox2.y-vox1.y, vox2.z-vox1.z); 
		normal.insert( normal.x/ dist, normal.y/ dist, normal.z/ dist);
						
		Vab.insert( vel1.x-vel2.x, vel1.y-vel2.y, vel1.z-vel2.z);
						
		Vn = Vab.dotproduct( normal);
		if (Vn>=0){
			penetration = vox1.width()- dist;
			jf= -(1+e)*Vn/2;   // masses are unit
			
			vel1.insert( vel1.x+ jf*normal.x, vel1.y+ jf*normal.y, vel1.z+ jf*normal.z);
			vel2.insert( vel2.x- jf*normal.x, vel2.y- jf*normal.y, vel2.z- jf*normal.z);
												
			vox1.insert( vox1.x- normal.x*penetration, vox1.y- normal.y*penetration, vox1.z- normal.z*penetration); 
		}
	}
}

//============ write to file ===========
//void writeToFile(std::stringstream& ss){  
//    using namespace std;  
//    string myString = ss.str();  
//    FILE * myfile;  
//	myfile = fopen("voxel_coords.txt", "a");  
//    //myfile << myString;  
//    fprintf( myfile, myString);
//	myfile.close();  
//}  
